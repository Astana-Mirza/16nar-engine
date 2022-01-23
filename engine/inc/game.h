#ifndef _16NAR_GAME_H
#define _16NAR_GAME_H

#include <memory>
#include <vector>
#include <string>
#include "render/quadrant.h"
#include "constructor/world_node.h"
#include "system/file_scene_reader.h"
#include "system/input_mapper.h"

namespace _16nar {

class Node;

class Game {
public:
	static Game& get_game() {
		static Game instance;
		return instance;
	}

	Game(Game const&)		= delete;
	void operator=(Game const&)	= delete;

	void run();
	void exit();
	InputMapper& get_input_mapper();
	std::vector<Event>& get_events();
	void load_scene(const std::string& name);
	Node *get_node(const std::string& name) const;
	void set_node_name(Node *node, const std::string& name);
	void delete_node_name(const std::string& name);

private:
	enum class State { Exit, Run, Pause, Load };
	InputMapper input;
	RenderWindow window;
	std::string scene_name;
	std::unordered_map<std::string, Node*> node_names;
	std::vector<Event> events;
	Quadrant root_quad;
	WorldNode world;
	std::unique_ptr<SceneReader> scene_reader;
	Time time_per_frame = sf::seconds(1.f / 60.f);
	void (*setup_func)() = nullptr;
	void (*loop_func)(float) = nullptr;
	State current_state = State::Exit;

	Game() =default;
	void update(float delta);
	void load();
	void render();
	void read_events();
};

}

#endif
