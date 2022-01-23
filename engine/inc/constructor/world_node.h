#ifndef _16NAR_WORLD_NODE_H
#define _16NAR_WORLD_NODE_H

#include <set>
#include "abstract/node.h"

namespace _16nar {

class WorldNode {
public:
	~WorldNode();
	void add_node(Node *node);
	void add_pause_node(Node *node);
	void setup();
	void loop(bool in_pause, float delta);

private:
	friend class Game;
	std::set<Node*> nodes;
	std::set<Node*> pause_nodes;	// MUST NOT INVOLVE PHYSICS

	// float pixels_in_one_meter; - set in constructor
	// b2World *get_world();
};

}



#endif
