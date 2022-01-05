#ifndef _16NAR_SCENE_FILE_H
#define _16NAR_SCENE_FILE_H

#include <cstdint>

#define CHILD_RECORD_SIZE 10

namespace _16nar {


enum class ResourceType : uint8_t { Texture, Sound };

enum class NodeType : uint8_t { Node, SpriteNode };


struct SectionTable {
	uint32_t tree_off;		// all node signatures
	uint32_t code_off;		// dynamic library name table
	uint32_t rsrc_off;		// resource file table
};


struct SceneHeader {
	float q_start_x;		// coordinates of a starting quadrant
	float q_start_y;
	uint32_t load_width;		// how many quadrants should be loaded in width
	uint32_t load_height;		// how many quadrants should be loaded in height
	uint32_t q_width;		// width of one quadrant (in pixels)
	uint32_t q_height;		// height of one quadrant (in pixels)
	uint32_t main_code_file;	// number of dynamic library
					// containing setup and update functions
	uint32_t setup_off;		// offset of the setup function's name
	uint32_t loop_off;		// offset of the loop function's name
	uint32_t node_count;		// count of nodes in this scene
	uint32_t code_file_count;	// count of user's dynamic libraries used
	uint32_t resource_file_count;	// count of files with resources used in this scene
	SectionTable sections;
	uint8_t multifile;		// boolean value, if 1, then scene can
					// be expanded with other files
};


/*struct ChildRecord {	// one block of a container, that keeps chidren in a quadrant
	uint32_t child_count;
	uint32_t child_off[CHILD_RECORD_SIZE];
	uint8_t is_valid[CHILD_RECORD_SIZE];
	uint32_t next_off;
};


struct QuadrantSign {
	uint32_t top, right, bottom, left;	// offsets to bordering quadrants
	uint32_t rec;				// offset to record of this quadrant's children
};*/


struct ResourceFileHeader {
	uint16_t res_count;
};


struct ResourceTableEntry {
	uint32_t filename_off;
	uint32_t res_array_off;	// offset to the array of IDs of
				// needed resources from this file
	uint16_t res_count;	// count of resources requested from this file
	uint16_t file_id;
};


struct ResourceSign {
	uint32_t offset;	// offset to a packed resource
	uint32_t size;		// size of a packed resource
	ResourceType type;	// type of a resource (texture, sound, etc.)
};


struct ResourceID {	// ID of a resource, which is stored in every object
	uint16_t file_id;
	uint16_t rsrc_id;

	bool operator < (const ResourceID& r) const {
		if (file_id != r.file_id)
			return file_id < r.file_id;
		return rsrc_id < r.rsrc_id;
	}
};


// ------------------------ NODE-SPECIFIC STRUCTS ------------------------------


struct SpriteNodeInfo {
	int layer;
	ResourceID res;
};


// -----------------------------------------------------------------------------


struct NodeInfo {
	uint32_t parent;	// offset to a parent (0 if parent is a world node)
	uint32_t name_off;	// offset to the node's name
	float x, y, scale_x, scale_y,
		rotation, origin_x, origin_y;
	uint32_t code_file_num;		// number of dynamic library in the table
	uint32_t creator_name_off;	// offset to a name of function
					// that makes the node (nullptr if not used)
	NodeType node_type;		// type of a node
	union {				// node-specific structs
		SpriteNodeInfo sprite_inf;
	};
};


}


#endif	// #ifndef _16NAR_SCENE_FILE_H
