/// @file
/// @brief Header file with SceneReader abstract class definition.
#ifndef _16NAR_SCENE_READER_H
#define _16NAR_SCENE_READER_H

#include <constructor/world_node.h>
#include <render/quadrant.h>

namespace _16nar
{

/// @brief Abstract class for reading scene.
class ENGINE_API SceneReader
{
public:
     /// @brief Sets scene name.
     /// @name name of the scene.
     virtual void set_scene( const std::string& name ) = 0;

     /// @brief Loads the scene from source.
     /// @world root node in node tree.
     /// @setup_func pointer to setup function.
     /// @loop_func pointer to loop function.
     virtual void load_scene( WorldNode& world,
                              WorldNode::SetupFuncPtr& setup_func,
                              WorldNode::LoopFuncPtr& loop_func ) = 0;
	virtual ~SceneReader() = default;
};

} // namespace _16nar

#endif // #ifndef _16NAR_SCENE_READER_H
