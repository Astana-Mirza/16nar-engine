#include <16nar/core/defs.h>

#include <16nar/platform/strings/hash.h>

#include <16nar/core/assets/flatbuffers/flatbuffers_asset_file_processor.h>
#include <16nar/core/assets/flatbuffers/flatbuffers_asset_reader.h>
#include <16nar/core/assets/flatbuffers/flatbuffers_asset_writer.h>
#if defined( NARENGINE_ASSETS_JSON )
#    include <16nar/core/assets/json/json_asset_file_processor.h>
#    include <16nar/core/assets/json/json_asset_reader.h>
#    include <16nar/core/assets/json/json_asset_writer.h>
#    include <16nar/core/assets/json/json_to_flatbuffers_convertor.h>
#endif // defined( NARENGINE_ASSETS_JSON )
#include <16nar/core/assets/iasset_data_convertor.h>

#include <cxxopts.hpp>

#include <string>
#include <set>
#include <unordered_map>
#include <filesystem>
#include <stdexcept>
#include <memory_resource>
#include <cassert>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace _16nar::assets
{
namespace
{

struct AssetToolParams
{
     std::string output_dir{};
     std::string src_format{};
     std::string dst_format{};
     std::vector< std::string > types{};
     std::vector< std::string > schemas{};
     std::vector< std::string > include_paths{};
     bool quiet{};
};


void check_format( const std::string& format )
{
#if defined( NARENGINE_ASSETS_JSON )
     if ( format == "json" )
     {
          return;
     }
#endif // defined( NARENGINE_ASSETS_JSON )
     if ( format == "flatbuffers" )
     {
          return;
     }
     throw std::runtime_error{ "unknown format '" + format + "'" };
}


IAssetFileProcessorPtr make_file_processor( const std::string& format )
{
     auto* resource = std::pmr::get_default_resource();
     assert( resource );
#if defined( NARENGINE_ASSETS_JSON )
     if ( format == "json" )
     {
          return std::make_shared< JsonAssetFileProcessor >( *resource );
     }
#endif // defined( NARENGINE_ASSETS_JSON )
     if ( format == "flatbuffers" )
     {
          return std::make_shared< FlatBuffersAssetFileProcessor >( *resource );
     }
     throw std::runtime_error{ "unknown format '" + format + "'" };
}


IAssetDataConvertorPtr make_convertor( const AssetToolParams& params, bool& backward )
{
#if defined( NARENGINE_ASSETS_JSON )
     std::shared_ptr< JsonToFlatBuffersConvertor > json_to_fb{};
     if ( params.src_format == "json" && params.dst_format == "flatbuffers" )
     {
          backward = false;
          json_to_fb = std::make_shared< JsonToFlatBuffersConvertor >();
     }
     if ( params.src_format == "flatbuffers" && params.dst_format == "json" )
     {
          backward = true;
          json_to_fb = std::make_shared< JsonToFlatBuffersConvertor >();
     }
     if ( json_to_fb )
     {
          for ( const auto& schema_path : params.schemas )
          {
               std::ifstream schema_input{ schema_path };
               const std::string schema_data{
                    std::istreambuf_iterator< char >( schema_input ),
                    std::istreambuf_iterator< char >() };
               if ( !json_to_fb->add_schema( schema_data, params.include_paths, schema_path ) )
               {
                    throw std::runtime_error{ "cannot register schema " + schema_path };
               }
          }
          std::unordered_map< std::uint32_t, std::string > hashes{};
          for ( const auto& type : params.types )
          {
               std::string_view type_sv{ type.c_str() };
               const auto hash = strings::str_hash32( type_sv );
               if ( !hashes.insert( { hash, type } ).second )
               {
                    throw std::runtime_error{ "duplicate hashes found for types '"
                         + hashes[ hash ] + "' and '" + type + "'" };
               }
               json_to_fb->set_type_name( hash, type_sv );
          }
          return json_to_fb;
     }
#endif // defined( NARENGINE_ASSETS_JSON )
     throw std::runtime_error{ "cannot convert format '"
          + params.src_format + "' to '" + params.dst_format + "'" };
}


std::uint32_t convert_recursive( IAssetReader& reader, IAssetWriter& writer,
     IAssetDataConvertor& convertor, bool backward )
{
     std::vector< std::uint32_t > children_ids{};
     const bool is_array = reader.is_array();
     if ( is_array )
     {
          const auto count = reader.get_children_count();
          children_ids.reserve( count );
          for ( std::size_t i = 0; i < count; ++i )
          {
               if ( !reader.to_child_index( i ) )
               {
                    throw std::runtime_error{ "cannot go to asset child #" + std::to_string( i ) };
               }
               children_ids.emplace_back( convert_recursive( reader, writer, convertor, backward ) );
               if ( !reader.to_parent() )
               {
                    throw std::runtime_error{ "cannot go to asset parent" };
               }
          }
     }
     else
     {
          const auto names = reader.get_children_names();
          children_ids.reserve( names.size() );
          for ( const auto& name : names )
          {
               if ( !reader.to_child( name.c_str() ) )
               {
                    throw std::runtime_error{ "cannot go to asset child '" + name + "'" };
               }
               children_ids.emplace_back( convert_recursive( reader, writer, convertor, backward ) );
               if ( !reader.to_parent() )
               {
                    throw std::runtime_error{ "cannot go to asset parent" };
               }
          }
     }
     const auto name = reader.get_name();
     AssetData out_content{};
     const auto in_content = reader.get_content();
     if ( in_content.data )
     {
          out_content = backward ?
               convertor.convert_backward( in_content ) : convertor.convert_forward( in_content );
          if ( !out_content.data )
          {
               throw std::runtime_error{ convertor.get_error_description() };
          }
     }
     const auto ret = writer.write_asset( name, out_content, children_ids, is_array );
     if ( !ret )
     {
          throw std::runtime_error{ "cannot write asset " + std::string{ name.cbegin(), name.cend() } };
     }
     return ret;
}


void process( const std::vector< std::string >& paths, const AssetToolParams& params )
{
     bool backward{};
     auto convertor = make_convertor( params, backward );
     auto src_processor = make_file_processor( params.src_format );
     auto dst_processor = make_file_processor( params.dst_format );

     assert( convertor );
     assert( src_processor );
     assert( dst_processor );

     auto writer = dst_processor->make_asset_writer();
     assert( writer );
     auto reader = src_processor->make_asset_reader();
     assert( reader );

     for ( const auto& path : paths )
     {
          if ( !params.quiet )
          {
               std::cout << "Processing file " << path << "..." << std::endl;
          }
          system::File file{};
          if ( !file.open( path ) )
          {
               throw std::runtime_error{ "unable to open file '" + path + "'" };
          }

          const auto out_path = ( fs::path( params.output_dir ) / fs::path( path ).filename() ).string() + ".out";
          system::File out_file{};
          if ( !out_file.open( out_path, true ) )
          {
               throw std::runtime_error{ "unable to open file '" + out_path + "'" };
          }
          auto buffer_ptr = src_processor->read_asset_data( file );
          if ( !buffer_ptr || !buffer_ptr.get_const_view() )
          {
               throw std::runtime_error{ "cannot read asset data from file '" + path + "'" };
          }
          reader->reset( buffer_ptr.get_const_view() );

          const auto root_id = convert_recursive( *reader, *writer, *convertor, backward );
          const auto out_buffer = writer->finish( root_id );
          if ( !params.quiet )
          {
               std::cout << "Writing output to file " << out_path << "...";
          }
          if ( !dst_processor->write_asset_data( out_buffer, out_file ) )
          {
               throw std::runtime_error{ "cannot write asset data to file '" + out_path + "'" };
          }
          if ( !params.quiet )
          {
               std::cout << " done" << std::endl;
          }

          writer->reset();
     }
}

} // anonymous namespace
} // namespace _16nar::assets


int main( int argc, char *argv[] )
{
     try
     {
          cxxopts::Options options(
               "16nar_asset_tool",
               "Utility for asset format conversions."
          );
          options.allow_unrecognised_options();
          options.add_options()
               ( "h,help", "Print usage" )
               ( "O,output-dir", "Output directory of the utility",
                    cxxopts::value< std::string >()->default_value( fs::current_path().string() ) )
               ( "s,src-format", "Source file format",
                    cxxopts::value< std::string >() )
               ( "d,dst-format", "Target file format",
                    cxxopts::value< std::string >() )
               ( "T,types", "Type names of processed assets",
                    cxxopts::value< std::vector< std::string > >() )
               ( "S,schemas", "Paths to schemas used in conversion",
                    cxxopts::value< std::vector< std::string > >() )
               ( "I,include-paths", "Include paths for resolving schemas",
                    cxxopts::value< std::vector< std::string > >() )
               ( "q,quiet", "Disable text output to terminal", cxxopts::value< bool >() )
               ;
          _16nar::assets::AssetToolParams params{};
          auto args = options.parse( argc, argv );
          if ( args.count( "help" ) )
          {
               std::cout << options.help() << std::endl;
               return EXIT_SUCCESS;
          }
          params.output_dir = args[ "output-dir" ].as< std::string >();
          params.src_format = args[ "src-format" ].as< std::string >();
          params.dst_format = args[ "dst-format" ].as< std::string >();

          _16nar::assets::check_format( params.src_format );
          _16nar::assets::check_format( params.dst_format );

          auto normalize_paths = []( const std::vector< std::string >& paths )
               -> std::vector< std::string >
          {
               std::set< std::string > unique_paths{};
               for ( const auto& path : paths )
               {
                    unique_paths.emplace( fs::canonical( path ).string() );
               }
               return std::vector< std::string >( unique_paths.cbegin(), unique_paths.cend() );
          };

          params.types = args[ "types" ].as< std::vector< std::string > >();
          params.schemas = normalize_paths( args[ "schemas" ].as< std::vector< std::string > >() );
          if ( args.count( "include-paths" ) )
          {
               params.include_paths = normalize_paths(
                    args[ "include-paths" ].as< std::vector< std::string > >() );
          }
          if ( args.count( "quiet" ) )
          {
               params.quiet = args[ "quiet" ].as< bool >();
          }

          if ( params.src_format == params.dst_format )
          {
               throw std::logic_error{ "source and destinations are the same" };
          }
          if ( args.unmatched().empty() )
          {
               throw std::logic_error{ "no input files specified" };
          }

          for ( const auto& schema_file : params.schemas )
          {
               if ( !fs::exists( schema_file ) )
               {
                    throw std::runtime_error{ "schema file '" + schema_file + "' does not exist" };
               }
          }

          const auto files = normalize_paths( args.unmatched() );
          for ( const auto& file : files )
          {
               if ( !fs::exists( file ) )
               {
                    throw std::runtime_error{ "file '" + file + "' does not exist" };
               }
          }

          fs::create_directories( params.output_dir );
          _16nar::assets::process( files, params );
     }
     catch ( const std::exception& ex )
     {
          std::cerr << "Error: " << ex.what() << std::endl;
          return EXIT_FAILURE;
     }
     catch ( ... )
     {
          std::cerr << "Unknown error" << std::endl;
          return EXIT_FAILURE;
     }

     return EXIT_SUCCESS;
}
