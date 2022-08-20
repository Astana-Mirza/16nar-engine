/// @file
/// @brief Header file with SceneReader abstract class definition.
#ifndef _16NAR_SCENE_READER_H
#define _16NAR_SCENE_READER_H

#include <constructor/world_node.h>
#include <render/quadrant.h>

#include <scene_file.h>

namespace _16nar
{

/// @brief Abstract class for reading scene.
class ENGINE_API SceneReader
{
public:
     virtual ~SceneReader() = default;

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

     /// @brief Gets texture with given id, throws runtime_error if no such texture.
     /// @param id id of a texture.
     virtual Texture& get_texture( ResourceID id ) = 0;

     /// @brief Gets sound buffer with given id, throws runtime_error if no such sound buffer.
     /// @param id id of a sound buffer.
     virtual SoundBuffer& get_sound( ResourceID id ) = 0;

     /// @brief Gets font with given id, throws runtime_error if no such font.
     /// @param id id of a font.
     virtual Font& get_font( ResourceID id ) = 0;

     /// @brief Gets shader with given id, throws runtime_error if no such shader.
     /// @param id id of a shader.
     virtual Shader& get_shader( ResourceID id ) = 0;

};

} // namespace _16nar

#endif // #ifndef _16NAR_SCENE_READER_H
