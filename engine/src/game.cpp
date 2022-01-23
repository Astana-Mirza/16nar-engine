#include "../inc/game.h"

#include <iostream>
#include "../inc/system/dynamic_resource_manager.h"
#include "../inc/constructor/sprite_node.h"

using namespace _16nar;

void Game::run() {
	Clock clock;
	Time last_update = Time::Zero;
	while (current_state != State::Exit) {
		if (current_state == State::Load) {
			load();
			clock.restart();
			last_update = Time::Zero;
		}
		last_update += clock.restart();
		while (last_update > time_per_frame) {
			last_update -= time_per_frame;
			read_events();
			update(time_per_frame.asSeconds());
		}
		render();
	}
}

void Game::exit() {
	current_state = State::Exit;
}

InputMapper& Game::get_input_mapper() {
	return input;
}

std::vector<Event>& Game::get_events() {
	return events;
}

void Game::load_scene(const std::string& name) {
	scene_name = name;
	current_state = State::Load;
}

Node *Game::get_node(const std::string& name) const {
	auto iter = node_names.find(name);
	if (iter == node_names.cend())
		return nullptr;
	return iter->second;
}

void Game::set_node_name(Node *node, const std::string& name) {
	auto ret = node_names.insert({name, node});
	if (!ret.second)
		throw std::runtime_error{"name \"" + name
					+ "\" already exists"};
	if (node->name_ptr)
		node_names.erase(*(node->name_ptr));	// erase old name
	node->name_ptr = const_cast<std::string*>(&(ret.first->first));
}

void Game::delete_node_name(const std::string& name) {
	auto iter = node_names.find(name);
	if (iter != node_names.end()) {
		iter->second->name_ptr = nullptr;
		node_names.erase(iter);
	}
}

void Game::update(float delta) {
	loop_func(delta);
	world.loop(current_state == State::Pause, delta);
}

void Game::load() {
	// use scene_reader
	if (setup_func)
		setup_func();
	world.setup();
	current_state = State::Run;
}

void Game::render() {
	window.clear();
	window.draw(root_quad);
	window.display();
}

void Game::read_events() {
	events.clear();
	Event event;
	while (window.pollEvent(event))
		events.push_back(event);
}
