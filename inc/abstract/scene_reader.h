#ifndef _16NAR_SCENE_READER_H
#define _16NAR_SCENE_READER_H

#include "../constructor/abstract/node.h"
#include "../render/quadrant.h"

namespace _16nar {

class SceneReader {
public:
	virtual void set_scene(const std::string& name) =0;
	virtual void load_scene(Quadrant& quad,
				Node& world,
				void (**setup_ptr)(),
				void (**loop_ptr)(Time)) =0;
	virtual ~SceneReader() =default;
};

}

#endif
