/// @file
/// File with Compiler class definition.
#ifndef _16NAR_COMPILER_H
#define _16NAR_COMPILER_H

#include <scene_file.h>
#include <scene_data.h>

#include <QJsonObject>
#include <QFile>
#include <QList>
#include <QSharedPointer>

/// Class for main functionality of scene compiler.
class Compiler
{
public:
     /// Constructor.
     /// @param input_file path to the input JSON file
     Compiler( const QString& input_file );

     Compiler( const Compiler& ) = delete;

     /// Make a resource package.
     /// @param output_file path to output file.
     void compile_package( const QString& output_file );

     /// Make a scene package.
     /// @param output_file path to output file.
     void compile_scene( const QString& output_file );

private:
     /// Count offsets of all sections in file.
     /// @param hdr header where offsets will be filled.
     void count_offsets( _16nar::SceneHeader& hdr );

     /// Write all scene states information to file.
     void write_states();

     /// Write all scene nodes information to file.
     void write_nodes();

     /// Write all scene code files information to file.
     void write_code_files();

     /// Write all scene resources information to file.
     void write_resources();

     /// Write all data to file.
     void write_data();

     /// Fills node information structure using given JSON object.
     /// @param info node information structure.
     /// @param json JSON object with node information.
     void fill_node_by_type( _16nar::NodeInfo& info, QJsonObject& json );

     /// Saves data which will be written to the file.
     /// @param data data to be saved.
     /// @return offset of data in file.
     template < typename T >
     uint32_t save_data( const T& data );

     QJsonObject main_object_;                    ///< object that is read from input file.
     QFile out_file_;                             ///< compiled object file.
     QList< QSharedPointer< BaseData > > data_;   ///< all data that will be saved to file, in proper order.
     uint32_t data_pos_;                          ///< offset of the next data structure to be written.
};


template < typename T >
uint32_t Compiler::save_data( const T& data )
{
     uint32_t ret = data_pos_;
     data_.push_back( { new SceneData< T >( data ) } );
     data_pos_ += data_.constLast()->size();
     return ret;
}


template <>
uint32_t Compiler::save_data< QString >( const QString& data );

#endif // #ifndef _16NAR_COMPILER_H
