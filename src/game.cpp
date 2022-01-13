#include "../inc/game.h"

#include <iostream>
#include "../inc/system/dynamic_resource_manager.h"
#include "../inc/constructor/sprite_node.h"

using namespace _16nar;

#define SPEED 100
enum Action { UP, DOWN, LEFT, RIGHT };

void _loop(float dt) {
	auto ship = Game::get_game().get_node("ship");
	if (Game::get_game().get_input_mapper().is_action_pressed(UP))
		ship->move(0, -SPEED*dt);
	if (Game::get_game().get_input_mapper().is_action_pressed(DOWN))
		ship->move(0, SPEED*dt);
	if (Game::get_game().get_input_mapper().is_action_pressed(LEFT))
		ship->move(-SPEED*dt, 0);
	if (Game::get_game().get_input_mapper().is_action_pressed(RIGHT))
		ship->move(SPEED*dt, 0);
	for (const auto& event : Game::get_game().get_events()) {
		if (event.type == Event::Closed)
			Game::get_game().exit();
	}
}

void Game::_start() {
	window.create(sf::VideoMode(800, 600), "Test");
	Quadrant* q1 = new Quadrant({0, 0, 400, 300});
	Quadrant* q2 = new Quadrant({400, 0, 400, 300});
	Quadrant* q3 = new Quadrant({0, 300, 400, 300});
	Quadrant* q4 = new Quadrant({400, 300, 400, 300});
	root_quad = Quadrant({0, 0, 800, 600});
	root_quad.add_child(q1, 0);
	root_quad.add_child(q2, 1);
	root_quad.add_child(q3, 2);
	root_quad.add_child(q4, 3);

	const auto& tex = DynamicResourceManager<Texture>::instance()
						.get("res/Enterprise.png");
	SpriteNode *ship = new SpriteNode(&root_quad, tex);
	ship->set_parent(&world);
	set_node_name(ship, "ship");
	get_input_mapper().set_key_binding(UP, Keyboard::W);
	get_input_mapper().set_key_binding(DOWN, Keyboard::S);
	get_input_mapper().set_key_binding(LEFT, Keyboard::A);
	get_input_mapper().set_key_binding(RIGHT, Keyboard::D);
	loop_func = _loop;
}

void Game::run() {
	Clock clock;
	Time last_update = Time::Zero;
	while (current_state != State::Exit) {
		if (current_state == State::Load)
			load();
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
	_start();
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
	if (current_state == State::Pause) {
		// update pause node
	} else {
		loop_func(delta);
	}
}

void Game::load() {
	// use scene_reader
	if (setup_func)
		setup_func();
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
