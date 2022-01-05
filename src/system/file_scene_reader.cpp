#include "../../inc/system/file_scene_reader.h"

using namespace _16nar;

void FileSceneReader::set_scene(const std::string& name) {
	is.open(name, std::ios::in | std::ios::binary);
}

void FileSceneReader::load_scene(Quadrant& quad,
		Node& world,
		setup_func_ptr& setup_ptr,
		loop_func_ptr& loop_ptr) {
	SceneHeader hdr;
	is.read(reinterpret_cast<char*>(&hdr), sizeof(SceneHeader));
	FloatRect area({hdr.q_start_x, hdr.q_start_y,
		hdr.q_start_x+hdr.load_width*hdr.q_width,
		hdr.q_start_y+hdr.load_height*hdr.q_height});
	quad = Quadrant(area);
	make_quadrants(quad, Vector2f{area.left, area.top},
			hdr.q_width, hdr.q_height,
			hdr.load_width, hdr.load_height);
	load_packages(hdr);
	load_code(hdr, setup_ptr, loop_ptr);
	load_nodes(hdr, quad, world);
	offsets = {};
}

void FileSceneReader::load_packages(const SceneHeader& hdr) {
	is.seekg(hdr.sections.rsrc_off);
	ResourceTableEntry *res =
			new ResourceTableEntry[hdr.resource_file_count];
	is.read(reinterpret_cast<char*>(res),
		sizeof(ResourceTableEntry)*hdr.resource_file_count);
	for (uint32_t i = 0; i < hdr.resource_file_count; i++) {
		uint16_t *res_ids = new uint16_t[res[i].res_count];
		is.seekg(res[i].res_array_off);
		is.read(reinterpret_cast<char*>(res_ids),
			sizeof(uint16_t)*res[i].res_count);
		load_resources(read_string(res[i].filename_off) + ".narres",
				res[i].file_id, res[i].res_count, res_ids);
		delete[] res_ids;
	}
	delete[] res;
}

void FileSceneReader::load_code(const SceneHeader& hdr,
				setup_func_ptr& setup_ptr,
				loop_func_ptr& loop_ptr) {
	if (!hdr.sections.code_off)
		return;
	is.seekg(hdr.sections.code_off);
	uint32_t *lib_offsets = new uint32_t[hdr.code_file_count];
	is.read(reinterpret_cast<char*>(lib_offsets),
			sizeof(uint32_t)*hdr.code_file_count);
	load_libs(lib_offsets, hdr.code_file_count);
	delete[] lib_offsets;

	// fill game's function pointers
	if (hdr.setup_off) {
		auto s = read_string(hdr.setup_off);
		setup_ptr = reinterpret_cast<setup_func_ptr>(
			libs.at(hdr.main_code_file).get_symbol(s));
	}
	if (hdr.loop_off) {
		auto s = read_string(hdr.loop_off);
		loop_ptr = reinterpret_cast<loop_func_ptr>(
			libs.at(hdr.main_code_file).get_symbol(s));
	}
}

void FileSceneReader::load_nodes(const SceneHeader& hdr,
				Quadrant& quad, Node& world) {
	is.seekg(hdr.sections.tree_off);
	NodeInfo *nodes = new NodeInfo[hdr.node_count];
	is.read(reinterpret_cast<char*>(nodes),
		sizeof(NodeInfo)*hdr.node_count);
	for (uint32_t i = 0; i < hdr.node_count; i++) {
		create_node(nodes[i],
			hdr.sections.tree_off+i*sizeof(NodeInfo),
			read_string(nodes[i].name_off), quad, world);
	}
	delete[] nodes;
}

void FileSceneReader::load_resources(const std::string& filename,
		uint16_t file_id, uint16_t res_count, uint16_t *res_ids) {
	std::ifstream res_file;
	ResourceFileHeader hdr;
	res_file.open(filename, std::ios::in | std::ios::binary);
	res_file.read(reinterpret_cast<char*>(&hdr),
			sizeof(ResourceFileHeader));
	ResourceSign *signs = new ResourceSign[hdr.res_count];
	res_file.read(reinterpret_cast<char*>(signs),
			sizeof(ResourceSign)*hdr.res_count);
	for (uint16_t i = 0; i < res_count; i++) {
		char *raw = new char[signs[res_ids[i]].size];
		res_file.seekg(signs[res_ids[i]].offset);
		res_file.read(reinterpret_cast<char*>(raw),
			sizeof(char)*signs[res_ids[i]].size);
		switch (signs[res_ids[i]].type) {
			case ResourceType::Texture:
			{
				textures[{file_id, res_ids[i]}] = Texture{};
				if (!textures[{file_id, res_ids[i]}]
					.loadFromMemory(raw,
						signs[res_ids[i]].size))
					throw std::runtime_error{
					"Error loading resource from file "
					+ filename
					};
			}
			break;
			default:
			break;
		}
		delete[] raw;
	}
	delete[] signs;
}


void FileSceneReader::make_quadrants(Quadrant& parent,
			const Vector2f& start,
			uint32_t q_width, uint32_t q_height,
			uint32_t wcount, uint32_t hcount) {
	if (wcount == 1 && hcount == 1) return;
	Quadrant ***quads = new Quadrant**[hcount];
	for (uint32_t i = 0; i < hcount; i++) {
		quads[i] = new Quadrant*[wcount];
		for (uint32_t j = 0; j < wcount; j++) {
			quads[i][j] = new Quadrant(
				FloatRect{start.x+j*q_width,
				start.y+i*q_height,
				(float)q_width, (float)q_height});
		}
	}

	uint32_t current_wcount = wcount, current_hcount = hcount;
	while (current_wcount > 2 || current_hcount > 2) {
		// indexes of bigger quadrants in 2-dim. array
		uint32_t x = 0, y = 0;
		uint32_t new_wcount = (current_wcount+1)/2;
		uint32_t new_hcount = (current_hcount+1)/2;
		uint32_t enlarge = 0;
		for (uint32_t i = 0; i < current_hcount; i += 2) {
			for (uint32_t j = 0; j < current_wcount; j += 2) {
				Quadrant *bigger = new Quadrant(FloatRect{
					start.x+x*(float)q_width*(2 << enlarge),
					start.y+y*(float)q_height*(2 << enlarge),
					(float)q_width*(2 << enlarge),
					(float)q_height*(2 << enlarge)});
				bigger->add_child(quads[i][j], 0);
				if (j+1 < current_wcount)
					bigger->add_child(quads[i][j+1], 1);
				if (i+1 < current_hcount)
					bigger->add_child(quads[i+1][j], 2);
				if (i+1 < current_hcount && j+1 < current_wcount)
					bigger->add_child(quads[i+1][j+1], 3);
				quads[x][y] = bigger;
			}
			x++;
			if (x == new_wcount) {
				x = 0;
				y++;
			}
		}
		enlarge++;
		current_wcount = new_wcount;
		current_hcount = new_hcount;
	}
	parent.add_child(quads[0][0], 0);
	if (1 < current_wcount)
		parent.add_child(quads[0][1], 1);
	if (1 < current_hcount)
		parent.add_child(quads[1][0], 2);
	if (1 < current_hcount && 1 < current_wcount)
		parent.add_child(quads[1][1], 3);

	for (uint32_t i = 0; i < hcount; i++)
		delete[] quads[i];
	delete[] quads;
}

void FileSceneReader::create_node(const NodeInfo& info, uint32_t offset,
		const std::string& name, Quadrant& quad, Node& world) {

}

void FileSceneReader::load_libs(uint32_t *lib_offsets, uint32_t count) {
	libs.reserve(count);
	for (uint32_t i = 0; i < count; i++) {
		libs.push_back(std::move(DynamicLib(
			read_string(lib_offsets[i]))));
	}
}

std::string FileSceneReader::read_string(uint32_t offset) {
	auto pos = is.tellg();
	is.seekg(offset);
	std::string s;
	is >> s;
	is.seekg(pos);
	return s;
}
