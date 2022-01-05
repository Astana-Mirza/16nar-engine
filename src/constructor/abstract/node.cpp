#include "../../../inc/constructor/abstract/node.h"

using namespace _16nar;

Node::~Node() {
	for (auto child : children)
		delete child;
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
