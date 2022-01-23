#include "../../inc/constructor/world_node.h"

using namespace _16nar;

WorldNode::~WorldNode() {
	for (const auto ptr : pause_nodes)
		delete ptr;
	for (const auto ptr : nodes)
		delete ptr;
}

void WorldNode::add_node(Node *node) {
	nodes.insert(node);
}

void WorldNode::add_pause_node(Node *node) {
	pause_nodes.insert(node);
}

void WorldNode::setup() {
	for (auto ptr : pause_nodes)
		ptr->setup_call();
	for (auto ptr : nodes)
		ptr->setup_call();
}

void WorldNode::loop(bool in_pause, float delta) {
	if (in_pause) {
		for (auto ptr : pause_nodes)
			ptr->loop_call(false, delta);
	}
	else {
		for (auto ptr : nodes)
			ptr->loop_call(false, delta);
	}
}
