#include "../../inc/system/input_mapper.h"

using namespace _16nar;

void InputMapper::set_key_binding(int action, Keyboard::Key key) {
	for (auto it : key_bindings) {
		if (it.second == key)
			key_bindings.erase(it.first);
	}
	key_bindings[action] = key;
}

void InputMapper::set_joystick_binding(int action, unsigned int joystick,
					unsigned int button) {
	std::pair<unsigned int, unsigned int> pair{joystick, button};
	for (auto it : joystick_bindings) {
		if (it.second == pair)
			joystick_bindings.erase(it.first);
	}
	joystick_bindings[action] = pair;
}

bool InputMapper::is_action_pressed(int action) {
	auto key_it = key_bindings.find(action);
	if (key_it != key_bindings.end())
		return Keyboard::isKeyPressed(key_it->second);
	auto joy_it = joystick_bindings.find(action);
	if (joy_it != joystick_bindings.end())
		return Joystick::isButtonPressed(joy_it->second.first,
						joy_it->second.second);
	throw std::runtime_error{"No button assigned for action " +
						std::to_string(action)};
}
