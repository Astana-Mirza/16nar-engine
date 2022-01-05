#ifndef _16NAR_GAME_H
#define _16NAR_GAME_H

#include <memory>
#include <string>
#include "constructor/node_2d.h"
#include "system/file_scene_reader.h"

namespace _16nar {

class Game {
public:
	void load_scene(const std::string& name);
	void start_scene();
	void load_resource(ResourceType type, const std::string& path);
	void free_resource(const std::string& path);

private:
	Quadrant root_quad;
	Node2D world;
	std::unique_ptr<SceneReader> scene_reader;
	void (*setup_func)() = nullptr;
	void (*loop_func)(Time) = nullptr;
};

}

#endif
