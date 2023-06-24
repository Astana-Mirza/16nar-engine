/// @file
/// @brief Header file with FileSceneReader class definition.
#ifndef _16NAR_FILE_SCENE_READER_H
#define _16NAR_FILE_SCENE_READER_H

#include <16nar/abstract/scene_reader.h>
#include <16nar/system/dynamic_lib.h>
#include <16nar/constructor/states/scene_state.h>

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
     /// @param[in] name of scene file, absolute or relative to current path.
     void set_scene( const std::string& name ) override;

     /// @brief Loads the scene from source.
     /// @param[in] world root node in node tree.
     /// @param[in] setup_ptr pointer to the setup function of the scene.
     /// @param[in] loop_ptr pointer to the loop function of the scene.
     void load_scene( WorldNode& world,
                      WorldNode::SetupFuncPtr& setup_ptr,
                      WorldNode::LoopFuncPtr& loop_ptr ) override;

     /// @brief Gets texture with given id, throws runtime_error if no such texture.
     /// @param[in] id id of a texture.
     Texture& get_texture( ResourceID id ) override;

     /// @brief Gets sound buffer with given id, throws runtime_error if no such sound buffer.
     /// @param[in] id id of a sound buffer.
     SoundBuffer& get_sound( ResourceID id ) override;

     /// @brief Gets font with given id, throws runtime_error if no such font.
     /// @param[in] id id of a font.
     Font& get_font( ResourceID id ) override;

     /// @brief Gets shader with given id, throws runtime_error if no such shader.
     /// @param[in] id id of a shader.
     Shader& get_shader( ResourceID id ) override;

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
     /// @param[in] id resource id to be checked.
     inline bool check_resource( ResourceID id )
     {
          return id.file_id != std::numeric_limits< uint16_t >::max()
              || id.rsrc_id != std::numeric_limits< uint16_t >::max();
     }

     /// @brief Loads all resource packages for the scene.
     /// @param[in] hdr scene header structure.
     void load_packages( const SceneHeader& hdr );

     /// @brief Loads all needed resources from one package.
     /// @param[in] filename name of package file.
     /// @param[in] file_id id of a package in this scene.
     /// @param[in] res_count number of resources needed from the package.
     /// @param[in] red_ids array of resource ids needed from the package.
     void load_resources( const std::string& filename, uint16_t file_id, uint16_t res_count,
                          std::shared_ptr< uint16_t[] > res_ids );

     /// @brief Loads all dynamic libraries and fills the setup and loop function pointers.
     /// @param[in] hdr scene header structure.
     /// @param[out] setup_ptr pointer to the setup function of the scene.
     /// @param[out] loop_ptr pointer to the loop function of the scene.
     void load_code( const SceneHeader& hdr,
                     WorldNode::SetupFuncPtr& setup_ptr,
                     WorldNode::LoopFuncPtr & loop_ptr);

     /// @brief Loads dynamic libraries.
     /// @param[in] lib_offsets array of offsets of library names.
     /// @param[in] count number of libraries to be loaded.
     void load_libs( std::shared_ptr< uint32_t[] > lib_offsets, uint32_t count );

     /// @brief Load all nodes for the state, seekg() must be at proper position before calling this method.
     /// @param[in] count number of nodes to be loaded.
     /// @param[in] render_system render system for drawable nodes.
     /// @param[in] state scene state, for which nodes will be loaded.
     void load_state_nodes( uint32_t count, RenderSystem *render_system, SceneState& state );

     /// @brief Creates one node using record.
     /// @param[in] info record structure.
     /// @param[in] offset offset of the record in file.
     /// @param[in] render_system render system for drawable nodes.
     /// @param[in] state scene state, in which this node is contained.
     void create_node( const NodeInfo& info, uint32_t offset, RenderSystem *render_system, SceneState& state );

     /// @brief Makes quadrant tree for space partitioning with combining smaller quadrants into bigger ones.
     /// @param[in] parent root quadrant of the tree.
     /// @param[in] start coordinates of upper left corner of root quadrant.
     /// @param[in] q_width width of one quadrant.
     /// @param[in] q_height height of one quadrant.
     /// @param[in] wcount number of smallest quadrants in width.
     /// @param[in] hcount number of smallest quadrants in height.
     void make_quadrants( Quadrant& parent, const Vector2f& start,
                          float q_width, float q_height,
                          uint32_t wcount, uint32_t hcount );

     /// @brief Makes tilemap node with all its tiles.
     /// @param[in] info record structure.
     /// @param[in] offset offset of the record in file.
     /// @param[in] render_system render system for drawing tiles.
     TilemapNode *make_tilemap( const NodeInfo& info, uint32_t offset, RenderSystem *render_system );

     /// @brief Reads ASCIIZ-string from file at given offset.
     /// @param[in] offset offset of the string.
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
