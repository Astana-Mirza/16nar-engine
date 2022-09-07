/// @file
/// @brief Header file with FileSceneReader class definition.
#ifndef _16NAR_FILE_SCENE_READER_H
#define _16NAR_FILE_SCENE_READER_H

#include <abstract/scene_reader.h>
#include <system/dynamic_lib.h>
#include <constructor/states/scene_state.h>

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <numeric> 

namespace _16nar
{

class TilemapNode;

/// @brief Class for reading compiled scene from .nar file.
class ENGINE_API FileSceneReader : public SceneReader
{
public:

     /// @brief Set scene file name.
     /// @param name of scene file, absolute or relative to current path.
     void set_scene( const std::string& name );

     /// @brief Loads the scene from source.
     /// @world root node in node tree.
     /// @param setup_ptr pointer to the setup function of the scene.
     /// @param loop_ptr pointer to the loop function of the scene.
     void load_scene( WorldNode& world,
                      WorldNode::SetupFuncPtr& setup_ptr,
                      WorldNode::LoopFuncPtr& loop_ptr );

     /// @brief Gets texture with given id, throws runtime_error if no such texture.
     /// @param id id of a texture.
     Texture& get_texture( ResourceID id );

     /// @brief Gets sound buffer with given id, throws runtime_error if no such sound buffer.
     /// @param id id of a sound buffer.
     SoundBuffer& get_sound( ResourceID id );

     /// @brief Gets font with given id, throws runtime_error if no such font.
     /// @param id id of a font.
     Font& get_font( ResourceID id );

     /// @brief Gets shader with given id, throws runtime_error if no such shader.
     /// @param id id of a shader.
     Shader& get_shader( ResourceID id );

private:
     /// @brief RAII wrapper for stream exceptions.
     struct IOGuard
     {
          std::istream& s;
          std::ios_base::iostate old_exceptions = s.exceptions();
          IOGuard( std::istream& ss, std::ios_base::iostate e ) : s{ ss }
          {
               s.exceptions( s.exceptions() | e );
          }
          ~IOGuard() { s.exceptions( old_exceptions ); }
     };

     /// @brief Checks if given resource id is valid.
     /// @param id resource id to be checked.
     bool check_resource( ResourceID id )    // inline
     {
          return id.file_id != std::numeric_limits< uint16_t >::max()
              || id.rsrc_id != std::numeric_limits< uint16_t >::max();
     }

     /// @brief Loads all resource packages for the scene.
     /// @param hdr scene header structure.
     void load_packages( const SceneHeader& hdr );

     /// @brief Loads all needed resources from one package.
     /// @param filename name of package file.
     /// @param file_id id of a package in this scene.
     /// @param res_count number of resources needed from the package.
     /// @param red_ids array of resource ids needed from the package.
     void load_resources( const std::string& filename, uint16_t file_id, uint16_t res_count,
                          std::shared_ptr< uint16_t[] > res_ids );

     /// @brief Loads all dynamic libraries and fills the setup and loop function pointers.
     /// @param hdr scene header structure.
     /// @param setup_ptr pointer to the setup function of the scene.
     /// @param loop_ptr pointer to the loop function of the scene.
     void load_code( const SceneHeader& hdr,
                     WorldNode::SetupFuncPtr& setup_ptr,
                     WorldNode::LoopFuncPtr & loop_ptr);

     /// @brief Loads dynamic libraries.
     /// @param lib_offsets array of offsets of library names.
     /// @param count number of libraries to be loaded.
     void load_libs( std::shared_ptr< uint32_t[] > lib_offsets, uint32_t count );

     /// @brief Load all nodes for the state, seekg() must be at proper position before calling this method.
     /// @param count number of nodes to be loaded.
     /// @param quad root quadrant of the state.
     /// @param state scene state, for which nodes will be loaded.
     void load_state_nodes( uint32_t count, Quadrant& quad, SceneState& state );

     /// @brief Creates one node using record.
     /// @param info record structure.
     /// @param offset offset of the record in file.
     /// @param quad quadrant where the node will be placed.
     /// @param state scene state, in which this node is contained.
     void create_node( const NodeInfo& info, uint32_t offset, Quadrant& quad, SceneState& state );

     /// @brief Makes quadrant tree for space partitioning with combining smaller quadrants into bigger ones.
     /// @param parent root quadrant of the tree.
     /// @param start coordinates of upper left corner of root quadrant.
     /// @param q_width width of one quadrant.
     /// @param q_height height of one quadrant.
     /// @param wcount number of smallest quadrants in width.
     /// @param hcount number of smallest quadrants in height.
     void make_quadrants( Quadrant& parent, const Vector2f& start,
                          float q_width, float q_height,
                          uint32_t wcount, uint32_t hcount );

     /// @brief Makes tilemap node with all its tiles.
     /// @param info record structure.
     /// @param offset offset of the record in file.
     /// @param quad quadrant where the node will be placed.
     TilemapNode *make_tilemap( const NodeInfo& info, uint32_t offset, Quadrant& quad );

     /// @brief Reads ASCIIZ-string from file at given offset.
     /// @param offset offset of the string.
     std::string read_string( uint32_t offset );

     std::ifstream file_stream_;                            ///< input stream associated with scene file.
     std::map< uint32_t, Node * > node_offsets_;            ///< offsets of nodes' records and pointers to their nodes.
     std::map< ResourceID, Texture > textures_;             ///< textures with their IDs.
     std::map< ResourceID, SoundBuffer > sounds_;           ///< sound buffers with their IDs.
     std::map< ResourceID, Font > fonts_;                   ///< fonts with their IDs.
     std::map< ResourceID, Shader > shaders_;               ///< shaders with their IDs.
     std::vector< DynamicLib > libs_;                       ///< dynamic libraries with code for the scene.
};

} // namespace _16nar

#endif // #ifndef _16NAR_FILE_SCENE_READER_H
