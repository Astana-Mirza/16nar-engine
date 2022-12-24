/// @file
/// @brief File with Compiler class definition.
#ifndef _16NAR_COMPILER_H
#define _16NAR_COMPILER_H

#include <16nar/scene_file.h>
#include <scene_data.h>

#include <fstream>
#include <string>
#include <functional>
#include <list>

#include <nlohmann/json.hpp>

namespace nl = nlohmann;

/// @brief Class for main functionality of scene compiler.
class Compiler
{
public:
     /// @brief Constructor.
     /// @param input_file path to the input JSON file
     Compiler( const std::string& input_file );

     Compiler( const Compiler& ) = delete;

     /// @brief Make a resource package.
     /// @param output_file path to output file.
     void compile_package( const std::string& output_file );

     /// @brief Make a scene package.
     /// @param output_file path to output file.
     void compile_scene( const std::string& output_file );

private:
     using CompileNodeFunction = std::function< void( _16nar::NodeInfo&, const nl::json& ) >;

     /// @brief Count offsets of all sections in file.
     /// @param hdr header where offsets will be filled.
     void count_offsets( _16nar::SceneHeader& hdr );

     /// @brief Write all scene states information to file.
     void write_states();

     /// @brief Write all scene nodes information to file.
     void write_nodes();

     /// @brief Write all scene code files information to file.
     void write_code_files();

     /// @brief Write all scene resources information to file.
     void write_resources();

     /// @brief Write all data to file.
     void write_data();

     /// @brief Writes tiles from json object.
     /// @param node object with tiles information.
     /// @return offset of written array.
     uint32_t save_tiles( const nl::json& node );

     /// @brief Saves data which will be written to the file.
     /// @param data data to be saved.
     /// @return offset of data in file.
     template < typename T >
     uint32_t save_data( const T& data );

     /// @brief Saves array which will be written to the file.
     /// @param count count of elements in array.
     /// @return pointer to array and offset of data in file.
     template < typename T >
     std::pair< std::shared_ptr< T[] >, uint32_t > save_array( uint32_t count );

     /// @brief Calculates size of the file in bytes.
     /// @param[in] filename name of the file.
     /// @return size of the file in bytes.
     uint32_t get_file_size( const std::string& filename ) const;

     /// @brief Fills Node2D information structure using given JSON object.
     /// @param info node information structure.
     /// @param node JSON object with node information.
     void fill_node2d( _16nar::NodeInfo& info, const nl::json& node );

     /// @brief Fills DrawableNode information structure using given JSON object.
     /// @param info node information structure.
     /// @param node JSON object with node information.
     void fill_drawable_node( _16nar::NodeInfo& info, const nl::json& node );

     /// @brief Fills SpriteNode information structure using given JSON object.
     /// @param info node information structure.
     /// @param node JSON object with node information.
     void fill_sprite_node( _16nar::NodeInfo& info, const nl::json& node );

     /// @brief Fills SoundNode information structure using given JSON object.
     /// @param info node information structure.
     /// @param node JSON object with node information.
     void fill_sound_node( _16nar::NodeInfo& info, const nl::json& node );

     /// @brief Fills TextNode information structure using given JSON object.
     /// @param info node information structure.
     /// @param node JSON object with node information.
     void fill_text_node( _16nar::NodeInfo& info, const nl::json& node );

     /// @brief Fills TilemapNode information structure using given JSON object.
     /// @param info node information structure.
     /// @param node JSON object with node information.
     void fill_tilemap_node( _16nar::NodeInfo& info, const nl::json& node );

     nl::json main_object_;                                          ///< object that is read from input file.
     std::ofstream out_file_;                                        ///< compiled object file.
     std::map< std::string, CompileNodeFunction > node_compilers_;   ///< node type names and compiling functions.
     std::list< std::unique_ptr< BaseData > > data_;                 ///< all data that will be saved to file, in proper order.
     uint32_t data_pos_;                                             ///< offset of the next data structure to be written.
};


template < typename T >
uint32_t Compiler::save_data( const T& data )
{
     uint32_t ret = data_pos_;
     data_.push_back( std::make_unique< SceneData< T > >( data ) );
     data_pos_ += data_.back()->size();
     return ret;
}


template < typename T >
std::pair< std::shared_ptr< T[] >, uint32_t > Compiler::save_array( uint32_t count )
{
     uint32_t ret = data_pos_;
     auto ptr = std::make_unique< SceneData< T[] > >( count );
     auto array = ptr->get_data();
     data_.push_back( std::move( ptr ) );
     data_pos_ += data_.back()->size();
     return std::make_pair( array, ret );
}


template <>
uint32_t Compiler::save_data< std::string >( const std::string& data );

#endif // #ifndef _16NAR_COMPILER_H
