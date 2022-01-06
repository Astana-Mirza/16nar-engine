#ifndef _16NAR_NODE_H
#define _16NAR_NODE_H

#include <set>
#include <stdexcept>
#include "../../abstract/transformable.h"
#include "../../abstract/signallable.h"

namespace _16nar {

class Node : public Signallable, public _16nar::Transformable {
public:
	virtual ~Node();
	virtual void setup() {}
	virtual void update() {}
	void set_parent(Node *par);
	const Node *get_parent() const;
	const std::set<Node *>& get_children() const;
	Transform get_global_transform() const;

private:
	void add_child(Node *ch);
	void remove_child(Node *ch);

	std::set<Node *> children;
	Node *parent = nullptr;
};

}

#endif
