#ifndef _16NAR_NODE_H
#define _16NAR_NODE_H

#include <set>
#include <stdexcept>
#include <string>
#include "../../abstract/transformable.h"
#include "../../abstract/signallable.h"

namespace _16nar {

class Game;

class Node : public Signallable, public _16nar::Transformable {
public:
	virtual ~Node();
	virtual void setup() {}
	virtual void loop(float delta) {}
	virtual void loop_call(bool upd, float delta);
	void set_parent(Node *par);
	const Node *get_parent() const;
	const std::set<Node *>& get_children() const;
	Transform get_global_transform() const;

private:
	friend class Game;
	void setup_call();
	void add_child(Node *ch);
	void remove_child(Node *ch);

	std::set<Node *> children;
	std::string *name_ptr = nullptr;
	Node *parent = nullptr;
};

}

#endif
