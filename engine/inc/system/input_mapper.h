#ifndef _16NAR_INPUT_MAPPER_H
#define _16NAR_INPUT_MAPPER_H

#include <unordered_map>
#include "../16nardefs.h"

namespace _16nar {

class InputMapper {
public:
	void set_key_binding(int action, Keyboard::Key key);
	void set_joystick_binding(int action, unsigned int joystick,
						unsigned int button);
	bool is_action_pressed(int action);

private:
	std::unordered_map<int, Keyboard::Key> key_bindings;
	std::unordered_map<int,
		std::pair<unsigned int, unsigned int>> joystick_bindings;
};

}

#endif
