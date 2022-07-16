/// @file
/// Header file with SceneReader abstract class definition.
#ifndef _16NAR_SCENE_READER_H
#define _16NAR_SCENE_READER_H

#include <constructor/world_node.h>
#include <render/quadrant.h>

namespace _16nar
{

/// Abstract class for reading scene.
class SceneReader
{
public:
     using SetupFuncPtr = void ( * )();
     using LoopFuncPtr = void ( * )( float );

     /// Sets scene name.
     /// @name name of the scene.
     virtual void set_scene( const std::string& name ) = 0;

     /// Loads the scene from source.
     /// @world root node in node tree.
     /// @setup_func double pointer to setup function.
     /// @loop_func double pointer to loop function.
     virtual void load_scene( WorldNode& world, SetupFuncPtr *setup_func, LoopFuncPtr *loop_func ) = 0;
	virtual ~SceneReader() = default;
};

} // namespace _16nar

#endif // #ifndef _16NAR_SCENE_READER_H
