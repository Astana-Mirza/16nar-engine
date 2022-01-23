#include "compiler.h"

static void _compile_node(_16nar::NodeInfo& inf, std::ofstream& os,
			  json& node);
static void _fill_node_info(_16nar::NodeInfo& inf, const json& node);
static _16nar::ResourceType
	_get_res_type(const std::string& res_name);
// file_id : { scene resource_id's }
static std::map<uint16_t, std::set<uint16_t>>
	_get_resources(const json& scene);
static long _get_file_size(const std::string& filename);
static void _fill_header(_16nar::SceneHeader& hdr, json& scene,
			std::map<uint16_t, std::set<uint16_t>>& resources);
// {strings offset, resource array counter}
static void _fill_sections(_16nar::SceneHeader& hdr, json& scene,
			std::map<uint16_t, std::set<uint16_t>>& resources,
			uint32_t& offset_counter, uint32_t& arr_counter);
static void _write_resource_table(const json& scene, std::ofstream& os,
			std::map<uint16_t, std::set<uint16_t>>& resources,
			uint32_t& offset_counter, uint32_t& arr_counter);
static void _write_node_tree(json& scene, std::ofstream& os,
			uint32_t& offset_counter, uint32_t& arr_counter);
static void _write_strings(const json& scene, std::ofstream& os,
			std::map<uint16_t, std::set<uint16_t>>& resources);

/******************************************************************************
** Compiles scene to .nar file and places it to the build directory
******************************************************************************/
void compile_scene(const std::string& scene_filename,
		const std::string& out_filename) {
	uint32_t offset_counter, arr_counter;
	_16nar::SceneHeader hdr;
	std::ifstream is(scene_filename);
	json scene;
	is >> scene;
	is.close();
	auto resources = _get_resources(scene);

	_fill_header(hdr, scene, resources);
	_fill_sections(hdr, scene, resources, offset_counter, arr_counter);

	std::ofstream os(out_filename, std::ios_base::binary);
	os.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));

	_write_resource_table(scene, os, resources, offset_counter, arr_counter);
	_write_node_tree(scene, os, offset_counter, arr_counter);
	for (uint32_t i = 0; i < hdr.code_file_count; i++) { // write lib names
		os.write(reinterpret_cast<char*>(&offset_counter),
			sizeof(uint32_t));
		offset_counter += sizeof(char)*(static_cast<std::string>(
			scene["libraries"][i]).size()+1); // +1 for \0
	}
	_write_strings(scene, os, resources);
}

/******************************************************************************
** Compiles a package from json file pck_filename to out_filename
******************************************************************************/
void compile_package(const std::string& pck_filename,
			const std::string& out_filename) {
	std::ifstream is(pck_filename);			// read json file
	json pck;
	is >> pck;
	is.close();

	_16nar::ResourceFileHeader hdr;
	std::ofstream os(out_filename, std::ios_base::binary);
	hdr.res_count = pck["resources"].size();	// write header
	os.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));

	uint32_t offset_counter = sizeof(hdr)
			+ hdr.res_count * sizeof(_16nar::ResourceSign);
							// write signatures
	for (const std::string& res : pck["resources"]) {
		_16nar::ResourceSign sign;
		sign.offset = offset_counter;
		sign.type = _get_res_type(res);
		sign.size = _get_file_size(res);

		offset_counter += sign.size;
		os.write(reinterpret_cast<char*>(&sign), sizeof(sign));
	}
							// pack files
	for (const std::string& res : pck["resources"]) {
		uint32_t size = _get_file_size(res);
		char *mem = new char[size];
		std::ifstream is(res, std::ios_base::binary);
		is.read(mem, size);
		os.write(mem, size);
		delete[] mem;
	}
}

/******************************************************************************
** Write node and its extra info to file
******************************************************************************/
void _compile_node(_16nar::NodeInfo& inf, std::ofstream& os,
						json& node) {
	_fill_node_info(inf, node);
	switch (inf.node_type) {
		case _16nar::NodeType::SpriteNode:
			inf.node_type = _16nar::NodeType::SpriteNode;
			inf.sprite_inf.res.file_id = node["resource_file"];
			inf.sprite_inf.res.rsrc_id = node["resource_id"];
			inf.sprite_inf.layer = node["layer"];
			break;
		default:
			break;
	}
	os.write(reinterpret_cast<char*>(&inf), sizeof(inf));
}

/******************************************************************************
** Returns a filled NodeInfo structure, except that offsets to nodes and
** strings are not written
******************************************************************************/
void _fill_node_info(_16nar::NodeInfo& inf, const json& node) {
	inf.x = node["pos"][0];
	inf.y = node["pos"][1];
	inf.scale_x = node["scale"][0];
	inf.scale_y = node["scale"][1];
	inf.origin_x = node["origin"][0];
	inf.origin_y = node["origin"][1];
	inf.rotation = node["rotation"];
	inf.code_file_num = node["code_file"];
	inf.node_type = static_cast<_16nar::NodeType>((uint8_t)node["type"]);
}

/******************************************************************************
** Returns a type of resource, based on file extension
******************************************************************************/
_16nar::ResourceType _get_res_type(const std::string& res_name) {
	if (res_name.empty())
		return _16nar::ResourceType::Texture; // throw
	return _16nar::ResourceType::Texture;
}

/******************************************************************************
** Converts all scene's resources to useful form (file IDs mapped to sets of
** this file's resource IDs)
******************************************************************************/
std::map<uint16_t, std::set<uint16_t>> _get_resources(const json& scene) {
	std::map<uint16_t, std::set<uint16_t>> resources;
	for (uint32_t id : scene["resources"])
		resources[id >> 16].insert(id & 0xFFFF);
	return resources;
}

/******************************************************************************
** Get file size, based on C functions (fseek, ftell)
******************************************************************************/
long _get_file_size(const std::string& filename) {
	long filesize = -1;
	std::FILE *fp = std::fopen(filename.c_str(), "rb");
	std::fseek(fp, 0L, SEEK_END);
	filesize = std::ftell(fp);
	std::fclose(fp);
	return filesize;
}

/******************************************************************************
** Fill scene header's main fields
******************************************************************************/
void _fill_header(_16nar::SceneHeader& hdr, json& scene,
			std::map<uint16_t, std::set<uint16_t>>& resources) {
	hdr.gravity_vec[0] = scene["gravity_vec"][0];
	hdr.gravity_vec[1] = scene["gravity_vec"][1];
	hdr.pixels_per_meter = scene["pixels_per_meter"];
	hdr.q_start_x = scene["space_bounds"][0];
	hdr.q_start_y = scene["space_bounds"][1];
	hdr.q_width  = scene["quadrant_size"][0];
	hdr.q_height = scene["quadrant_size"][1];
	hdr.load_width  = ceil(static_cast<float>(scene["space_bounds"][2])
			/ hdr.q_width);
	hdr.load_height = ceil(static_cast<float>(scene["space_bounds"][3])
			/ hdr.q_height);
	hdr.node_count = scene["nodes"].size();
	hdr.main_code_file = scene["main_lib"];
	hdr.code_file_count = scene["libraries"].size();
	hdr.resource_file_count = resources.size();
}

/******************************************************************************
** Fill scene header's section pointers
******************************************************************************/
void _fill_sections( _16nar::SceneHeader& hdr, json& scene,
			std::map<uint16_t, std::set<uint16_t>>& resources,
			uint32_t& offset_counter, uint32_t& arr_counter) {
	hdr.sections.rsrc_off = sizeof(hdr);

	offset_counter = sizeof(hdr)
		+ sizeof(_16nar::ResourceTableEntry)*resources.size();
	// offset of the first array of resource IDs
	arr_counter = offset_counter;
	for (auto& p : resources)
		offset_counter += p.second.size()*sizeof(uint16_t);
	hdr.sections.tree_off = offset_counter;

	offset_counter += sizeof(_16nar::NodeInfo)*hdr.node_count;
	for (auto& node : scene["nodes"])
		offset_counter += static_cast<size_t>(node["data_size"]);
	// offset to the array of libraries
	hdr.sections.code_off = offset_counter;
	// now it's offset of the first string
	offset_counter += hdr.code_file_count*sizeof(uint32_t);
	if (!scene["setup_func"].empty()) {
		hdr.setup_off = offset_counter;
		offset_counter += sizeof(char)*static_cast<std::string>(
				scene["setup_func"]).size()+1;  // +1 for \0
	} else {
		hdr.setup_off = 0;
	}
	if (!scene["loop_func"].empty()) {
		hdr.loop_off = offset_counter;
		offset_counter += sizeof(char)*static_cast<std::string>(
				scene["loop_func"]).size()+1;   // +1 for \0
	} else {
		hdr.loop_off = 0;
	}
}

/******************************************************************************
** Write all resource table entries
******************************************************************************/
void _write_resource_table(const json& scene, std::ofstream& os,
			std::map<uint16_t, std::set<uint16_t>>& resources,
			uint32_t& offset_counter, uint32_t& arr_counter) {
	for (auto& p : resources) {
		_16nar::ResourceTableEntry rte;
		rte.file_id = p.first;
		rte.res_count = p.second.size();
		rte.filename_off = offset_counter;
		rte.res_array_off = arr_counter;

		offset_counter += sizeof(char)*
			(static_cast<std::string>(
			 scene["pck_names"][p.first]).size()+1);
		arr_counter += sizeof(uint16_t)*rte.res_count;
		os.write(reinterpret_cast<char*>(&rte), sizeof(rte));
	}
	for (auto& p : resources) {
		for (auto& res_id : p.second) {
			os.write(reinterpret_cast<const char*>(&res_id),
				sizeof(uint16_t));
		}
	}
}

/******************************************************************************
** Write all scene's nodes
******************************************************************************/
void _write_node_tree(json& scene, std::ofstream& os,
		uint32_t& offset_counter, uint32_t& arr_counter) {
	// names mapped to offsets
	std::map<std::string, uint32_t> names = {{"", 0}};
	for (auto& node : scene["nodes"]) {
		_16nar::NodeInfo inf;
		if (!node["name"].empty()) {
			inf.name_off = offset_counter;
			offset_counter += sizeof(char)*(
				static_cast<std::string>(
				node["name"]).size()+1);
			names[static_cast<std::string>(node["name"])] =
								arr_counter;
		} else
			inf.name_off = 0;
		inf.parent = names[static_cast<std::string>(node["parent"])];
		if (!node["create_func"].empty()) {
			inf.creator_name_off = offset_counter;
			offset_counter += sizeof(char)*(
				static_cast<std::string>(
				node["create_func"]).size()+1);
		}
		_compile_node(inf, os, node);
		arr_counter += sizeof(_16nar::NodeInfo)
			+ static_cast<size_t>(node["data_size"]);
	}
}

/******************************************************************************
** Write all scene's strings
******************************************************************************/
void _write_strings(const json& scene, std::ofstream& os,
		std::map<uint16_t, std::set<uint16_t>>& resources) {
	if (!scene["setup_func"].empty()) {
		os << static_cast<std::string>(scene["setup_func"]);
		os.put('\0');
	}
	if (!scene["loop_func"].empty()) {
		os << static_cast<std::string>(scene["loop_func"]);
		os.put('\0');
	}
	for (auto& p : resources) {
		os << static_cast<std::string>(scene["pck_names"][p.first]);
		os.put('\0');
	}
	for (auto& node : scene["nodes"]) {
		os << static_cast<std::string>(node["name"]);
		os.put('\0');
		if (!node["create_func"].empty()) {
			os << static_cast<std::string>(node["create_func"]);
			os.put('\0');
		}
	}
	for (const std::string& lib_name : scene["libraries"]) {
		os << lib_name;
		os.put('\0');
	}
}
