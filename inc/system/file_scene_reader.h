#ifndef _16NAR_FILE_SCENE_READER_H
#define _16NAR_FILE_SCENE_READER_H

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "../abstract/scene_reader.h"
#include "../constructor/node_2d.h"
#include "dynamic_lib.h"
#include "scene_file.h"

namespace _16nar {

class FileSceneReader : public SceneReader {
public:
	using setup_func_ptr = void (*)();
	using loop_func_ptr = void (*)(Time);

	void set_scene(const std::string& name);
	void load_scene(Quadrant& quad,
			Node& world,
			setup_func_ptr& setup_ptr,
			loop_func_ptr& loop_ptr);

private:
	std::ifstream is;
	std::map<uint32_t, Node*> offsets;
	std::map<ResourceID, Texture> textures;
	std::vector<DynamicLib> libs;

	void make_quadrants(Quadrant& parent,
		const Vector2f& start,
		uint32_t q_width, uint32_t q_height,
		uint32_t wcount, uint32_t hcount);
	void load_packages(const SceneHeader& hdr);
	void load_code(const SceneHeader& hdr,
		setup_func_ptr& setup_ptr,
		loop_func_ptr& loop_ptr);
	void load_nodes(const SceneHeader& hdr, Quadrant& quad, Node& world);
	void load_resources(const std::string& filename, uint16_t file_id,
		uint16_t res_count, uint16_t *res_ids);
	void create_node(const NodeInfo& info, uint32_t offset,
		const std::string& name, Quadrant& quad, Node& world);
	void load_libs(uint32_t *lib_offsets, uint32_t count);
	std::string read_string(uint32_t offset);
};

}

#endif
