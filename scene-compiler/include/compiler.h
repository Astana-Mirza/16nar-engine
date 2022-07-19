/// @file
/// File with Compiler class definition.
#ifndef _16NAR_COMPILER_H
#define _16NAR_COMPILER_H

#include <scene_file.h>

#include <QJsonObject>
#include <QFile>
#include <QList>

/// Class for main functionality of scene compiler.
class Compiler
{
public:
     /// Constructor.
     /// @param input_file path to the input JSON file
     Compiler( const QString& input_file );

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

     /// Write all strings information to file.
     void write_strings();

     /// Fills node information structure using given JSON object.
     /// @param info node information structure.
     /// @param json JSON object with node information.
     void fill_node_by_type( _16nar::NodeInfo& info, QJsonObject& json );

     /// Save string which will be written to the file.
     /// @param str string to be saved.
     /// @return offset of string in file.
     uint32_t save_string( const QString& str );

     QJsonObject main_object_;          ///< object that is read from input file.
     QFile out_file_;                   ///< compiled object file.
     QList< QString > strings_;         ///< all strings that will be saved to file, in proper order.
     uint32_t str_pos_;                 ///< offset of the next strig to be written.
};

#endif // #ifndef _16NAR_COMPILER_H