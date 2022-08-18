/// @file
/// @brief A file with definition of structures for serialization of a scene.
#ifndef _16NAR_SCENE_FILE_H
#define _16NAR_SCENE_FILE_H

#include <cstdint>

namespace _16nar
{

#pragma pack ( push, 1 )

/// @brief Type of a resource packed in a file.
enum class ResourceType : uint8_t
{
     Texture,
     Sound,
     Font,
     VertexShader,
     GeometryShader,
     FragmentShader,
     VGShader,
     VGFShader
};


/// @brief Value indicating a type of a node
enum class NodeType : uint8_t
{
     Node2D,
     SpriteNode,
     SoundNode,
     TextNode
};


/// @brief Table with offsets to different sections.
struct SectionTable
{
     uint32_t state_off;           ///< offset of scene state structures.
     uint32_t tree_off;            ///< offset of node signatures.
     uint32_t code_off;            ///< offset of dynamic library name table.
     uint32_t rsrc_off;            ///< offset of resource file table.
};


/// @brief Header of a scene file.
struct SceneHeader
{
     uint32_t main_code_file;      ///< number of dynamic library with setup and update functions.
     uint32_t setup_off;           ///< offset of the setup function's name.
     uint32_t loop_off;            ///< offset of the loop function's name.
     uint32_t code_file_count;	///< count of user's dynamic libraries used.
     uint32_t resource_file_count;	///< count of files with resources used in this scene.
     uint32_t state_count;         ///< count of states of the scene.
     SectionTable sections;        ///< section table of the scene.
};


/// @brief Header of a resource file.
struct ResourceFileHeader
{
     uint16_t res_count;           ///< number of resources in the file.
};


/// @brief Entry of a resource table.
struct ResourceTableEntry
{
     uint32_t filename_off;        ///< offset of the name of the file.
     uint32_t res_array_off;       ///< offset of the array of IDs of resources from this file.
     uint16_t res_count;           ///< count of resources requested from this file.
     uint16_t file_id;             ///< file identifier, which is a part resource ID.
};


/// @brief Signature of a resource.
struct ResourceSign
{
     uint32_t offset;              ///< offset of a packed resource.
     uint32_t size;                ///< size of a packed resource.
     ResourceType type;            ///< type of a resource (texture, sound, etc).
};


/// @brief ID of a resource, which is stored for every node with resource.
struct ResourceID
{
     uint16_t file_id;             ///< identifier of resource file.
     uint16_t rsrc_id;             ///< identifier of a resource in the file.

     bool operator < ( const ResourceID& r ) const
     {
          if ( file_id != r.file_id )
          {
               return file_id < r.file_id;
          }
          return rsrc_id < r.rsrc_id;
     }
};

// ------------------------ NODE-SPECIFIC STRUCTS ------------------------------

/// @brief Base structure for nodes that need resources.
struct ResourceNodeRecord
{
     ResourceID res;               ///< identifier of a resource.
};


/// @brief Base structure for drawable nodes.
struct DrawableNodeRecord : public ResourceNodeRecord
{
     uint32_t color;               ///< color of the node.
     int layer;                    ///< draw layer.
     uint8_t visible;              ///< 1 if it is visible, 0 otherwise.
};


/// @brief Record about sprite node.
struct SpriteNodeInfo : public DrawableNodeRecord
{
     int rect_coords[ 2 ];         ///< coordinates of sprite rectangle in the texture.
     int rect_size[ 2 ];           ///< size of sprite rectangle in the texture.
};


/// @brief Record about sound node.
struct SoundNodeInfo : public ResourceNodeRecord
{
     float z_coord;                ///< z coordinate for 3D sound.
     float offset;                 ///< playing offset.
     float min_distance;           ///< minimal distance of the sound.
     float volume;                 ///< volume of the sound.
     float pitch;                  ///< pitch of the sound.
     float attenuation;            ///< attenuation of the sound.
     uint8_t relative_to_listener; ///< should the sound be absolute or relative to listener.
     uint8_t loop;                 ///< should the sound be played in a loop.
};


/// @brief Record about text node.
struct TextNodeInfo : public DrawableNodeRecord
{
     uint32_t string_offset;       ///< offset of a string of the text.
     uint32_t char_size;           ///< size of a character.
     uint32_t style;               ///< style of the text.
     uint32_t outline_color;       ///< outline color.
     float outline_thickness;      ///< thickness of the outline.
     float line_spacing;           ///< line spacing of the text.
     float letter_spacing;         ///< letter spacing of the text.
};

// -----------------------------------------------------------------------------

/// @brief Record about one node.
struct NodeInfo
{
     uint32_t parent;              ///< offset to a parent (parent is scene state if offset is zero).
     uint32_t name_off;            ///< offset to the node's name.
     float pos[ 2 ];               ///< position of the node.
     float scale[ 2 ];             ///< scale of the node.
     float origin[ 2 ];            ///< transform origin of the node.
     float rotation;               ///< rotation of the node, in degrees.
     uint32_t code_file_num;       ///< number (in a table) of dynamic library for code for this node.
     uint32_t creator_name_off;    ///< offset to a name of function that makes the node (nullptr if not used).
     NodeType node_type;           ///< type of the node.
     union                         ///< node-specific structures.
     {
          SpriteNodeInfo sprite_inf;    ///< sprite node record.
          SoundNodeInfo sound_inf;      ///< sound node record.
          TextNodeInfo text_inf;        ///< text node record.
     };
};


/// @brief Record about one state of the scene.
struct StateInfo
{
     float q_start[ 2 ];           ///< coordinates of a starting quadrant.
     float gravity_vec[ 2 ];       ///< gravity vector.
     float q_size[ 2 ];            ///< size of one quadrant (in pixels).
     float view_rect_pos[ 2 ];     ///< position of a view rectangle.
     float view_rect_size[ 2 ];    ///< size of a view rectangle.
     float pixels_per_meter;       ///< how many pixels are considered as one meter.
     uint32_t scene_size[ 2 ];     ///< number of quadrants that should be made in width and in height.
     uint32_t node_count;          ///< count of nodes in this state.
     int order;                    ///< order of this state.
     uint8_t updating;             ///< is this state is initially updating.
     uint8_t rendering;            ///< is this state is initially rendering.
};


} // namespace _16nar

#pragma pack ( pop )

#endif    // #ifndef _16NAR_SCENE_FILE_H
