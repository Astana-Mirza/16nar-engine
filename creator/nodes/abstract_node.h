#ifndef ABSTRACT_NODE_H
#define ABSTRACT_NODE_H

#include <nlohmann/json.hpp>
#include "../../scene_file.h"
#include "../controls/controllable.h"

using json = nlohmann::json;

class AbstractNode : public Controllable {
public:
	json node_info;
};

#endif
