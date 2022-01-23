#include "../../../inc/constructor/abstract/node.h"
#include "../../../inc/game.h"

using namespace _16nar;

Node::~Node() {
	if (name_ptr)
		Game::get_game().delete_node_name(*name_ptr);
	for (auto child : children)
		delete child;
}

void Node::setup_call() {
	setup();
	for (auto ch : children)
		ch->setup_call();
}

void Node::loop_call(bool upd, float delta) {
	loop(delta);
	upd = upd || transformed;
	transformed = false;
	for (auto ch : get_children())
		ch->loop_call(upd, delta);
}

void Node::set_parent(Node *par) {
	if (!par)
		throw std::logic_error{"parent is nullptr"};
	if (parent)
		parent->remove_child(this);
	parent = par;
	parent->add_child(this);
}

const Node *Node::get_parent() const {
	return parent;
}

const std::set<Node *>& Node::get_children() const {
	return children;
}

void Node::add_child(Node *ch) {
	children.insert(ch);
}

void Node::remove_child(Node *ch) {
	children.erase(ch);
}

Transform Node::get_global_transform() const {
	Transform transform = get_transform();
	for (const Node* n = get_parent(); n != nullptr; n = n->get_parent()) {
		transform = n->get_transform() * transform;
	}
	return transform;
}
