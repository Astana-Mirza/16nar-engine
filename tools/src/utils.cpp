#include <16nar/tools/utils.h>

#if defined( NARENGINE_TOOLS_JSON )
#    include <16nar/tools/json_asset_reader.h>
#    include <16nar/tools/json_asset_writer.h>
#endif // NARENGINE_TOOLS_JSON
#if defined( NARENGINE_TOOLS_FLATBUFFERS )
#    include <16nar/tools/flatbuffers_asset_reader.h>
#    include <16nar/tools/flatbuffers_asset_writer.h>
#endif // NARENGINE_TOOLS_FLATBUFFERS

#include <fstream>
#include <filesystem>
#include <stdexcept>

namespace _16nar::tools
{

DataSharedPtr read_binary( const std::string& path, std::size_t& data_size, bool zero_terminate )
{
     std::ifstream bin_file( path, std::ios_base::binary );
     if ( !bin_file )
     {
          throw std::runtime_error{ "cannot open file " + path };
     }
     bin_file.seekg( 0, std::ios_base::end );
     data_size = bin_file.tellg();
     bin_file.seekg( 0, std::ios_base::beg );
     std::size_t actual_size = data_size + ( zero_terminate ? 1 : 0 );     // +1 byte for zero termination
     _16nar::DataSharedPtr data{ new std::byte[ actual_size ], std::default_delete< std::byte[] >() };
     bin_file.read( reinterpret_cast< char * >( data.get() ), data_size );
     if ( zero_terminate )
     {
          *( data.get() + ( actual_size - 1 ) ) = std::byte{ 0 };
          data_size = actual_size;
     }
     return data;
}


void write_binary( const std::string& path, DataSharedPtr data, std::size_t data_size, bool zero_terminated )
{
     std::filesystem::path base_path = std::filesystem::path{ path }.remove_filename();
     if ( !std::filesystem::exists( base_path ) && !std::filesystem::create_directories( base_path ) )
     {
          throw std::runtime_error{ "cannot create directory " + base_path.string() };
     }
     std::ofstream bin_file( path, std::ios_base::binary );
     if ( !bin_file )
     {
          throw std::runtime_error{ "cannot open file " + path };
     }
     if ( zero_terminated )   // remove terminating zero byte
     {
          --data_size;
     }
     bin_file.write( reinterpret_cast< char * >( data.get() ), data_size );
}


std::string correct_path( const std::string& base_dir, const std::string& path )
{
     std::filesystem::path result{ path };
     if ( !base_dir.empty() && result.is_relative() )
     {
          result = std::filesystem::path{ base_dir } / result;
     }
     return result.string();
}


int get_channel_count( BufferDataFormat format )
{
     switch ( format )
     {
          case BufferDataFormat::Rgb:
          case BufferDataFormat::Srgb:
               return 3;
          case BufferDataFormat::Rgba:
          case BufferDataFormat::Srgba:
               return 4;
          default:
               throw std::runtime_error{ "wrong buffer data format: "
                    + std::to_string( static_cast< std::size_t >( format ) ) };
     }
     return 3;
}


std::unique_ptr< IAssetReader > create_asset_reader( const std::string& in_dir,
     PackageFormat format )
{
     switch ( format )
     {
#if defined( NARENGINE_TOOLS_FLATBUFFERS )
          case PackageFormat::FlatBuffers:
               return std::make_unique< FlatBuffersAssetReader >();
#endif // NARENGINE_TOOLS_FLATBUFFERS
#if defined( NARENGINE_TOOLS_JSON )
          case PackageFormat::Json:
               return std::make_unique< JsonAssetReader >( in_dir );
#endif // NARENGINE_TOOLS_JSON
          default:
               throw std::runtime_error{ "wrong asset reader format: "
                    + std::to_string( static_cast< std::size_t >( format ) ) };
     }
     return {};
}


std::unique_ptr< IAssetWriter > create_asset_writer( const std::string& out_dir,
     PackageFormat format )
{
     switch ( format )
     {
#if defined( NARENGINE_TOOLS_FLATBUFFERS )
          case PackageFormat::FlatBuffers:
               return std::make_unique< FlatBuffersAssetWriter >();
#endif // NARENGINE_TOOLS_FLATBUFFERS
#if defined( NARENGINE_TOOLS_JSON )
          case PackageFormat::Json:
               return std::make_unique< JsonAssetWriter >( out_dir );
#endif // NARENGINE_TOOLS_JSON
          default:
               throw std::runtime_error{ "wrong asset writer format: "
                    + std::to_string( static_cast< std::size_t >( format ) ) };
     }
     return {};
}

} // namespace _16nar::tools
