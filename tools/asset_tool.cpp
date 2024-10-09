/// @file
/// @brief Utility for engine resource management.

#include <json_asset_reader.h>
#include <flatbuffers_asset_writer.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <cstdlib>

namespace
{

constexpr char help_long[]        = "--help";
constexpr char help_short[]       = "-h";
constexpr char out_dir_long[]     = "--out-dir";
constexpr char out_dir_short[]    = "-o";
constexpr char src_format_long[]  = "--src-format";
constexpr char src_format_short[] = "-s";
constexpr char dst_format_long[]  = "--dst-format";
constexpr char dst_format_short[] = "-d";
constexpr char pack_long[]        = "--pack";
constexpr char pack_short[]       = "-p";
constexpr char unpack_long[]      = "--unpack";
constexpr char unpack_short[]     = "-u";
constexpr char quiet_long[]       = "--quiet";
constexpr char quiet_short[]      = "-q";


bool str_to_format( const std::string& str, _16nar::tools::PackageFormat& format )
{
     static const std::unordered_map< std::string, _16nar::tools::PackageFormat > formats = {
          { "json",        _16nar::tools::PackageFormat::Json },
          { "flatbuffers", _16nar::tools::PackageFormat::FlatBuffers }
     };
     auto iter = formats.find( str );
     if ( iter == formats.cend() )
     {
          return false;
     }
     format = iter->second;
     return true;
}


std::unique_ptr< _16nar::tools::IAssetReader > create_reader( _16nar::tools::PackageFormat format )
{
     switch ( format )
     {
          case _16nar::tools::PackageFormat::Json:
               return std::make_unique< _16nar::tools::JsonAssetReader >();
          default:
               std::cerr << "Error: wrong reader format (" << static_cast< std::size_t >( format ) << ")\n";
     }
     return {};
}


std::unique_ptr< _16nar::tools::IAssetWriter > create_writer( _16nar::tools::PackageFormat format )
{
     switch ( format )
     {
          case _16nar::tools::PackageFormat::FlatBuffers:
               return std::make_unique< _16nar::tools::FlatBuffersAssetWriter >();
          default:
               std::cerr << "Error: wrong writer format (" << static_cast< std::size_t >( format ) << ")\n";
     }
     return {};
}


void print_usage( std::ostream& out )
{
     out << "Usage: 16nar_asset_tool OPTIONS... [ ARGS ] FILES...\n"
          << "\nUtility for various resource format conversions and packing/unpacking.\n"
          << "\tOPTIONS:\n"
          << "\t\t--help, -h\n\t\tDisplay this message and exit.\n"
          << "\n\t\t--out-dir DIR, -o DIR\n\t\tOutput directory of the utility (it must exist), current directory is default.\n"
          << "\n\t\t--src-format FORMAT, -s FORMAT\n\t\tFormat of input file(s), default is json.\n"
          << "\n\t\t--dst-format FORMAT, -d FORMAT\n\t\tFormat of output file(s), default is flatbuffers.\n"
          << "\n\t\t--pack PACKAGE_NAME, -p PACKAGE_NAME\n\t\tCreate a package with given name from input files."
          << " File extension of the package will be set depending on output format. PACKAGE_NAME is relative to output directory.\n"
          << "\n\t\t--unpack, -u\n\t\tUnpack package(s) and place all resource files to output directory.\n"
          << "\n\t\t--quiet, -q\n\t\tDisable text output to terminal.\n"
          << "\n\tFORMATS:\n"
          << "\t\tjson\n\t\tJSON format. When used as output format, will generate binary assets without converting"
          << " to any specific format. For example, PNG file will not be created when unpacking texture asset from"
          << " flatbuffer format. Instead, there will be JSON file with description and BIN file with raw texture data.\n"
          << "\n\t\tflatbuffers\n\t\tFlatBuffers buffer format, but size of the buffer (uint32_t) will be prepended to buffer.\n";
}

} // anonymous namespace

int main( int argc, char *argv[] )
{
     std::vector< std::string > files;
     std::string out_dir = ".";
     std::string package_name;
     bool pack = false;
     bool unpack = false;
     bool quiet = false;
     auto src_format = _16nar::tools::PackageFormat::Json;
     auto dst_format = _16nar::tools::PackageFormat::FlatBuffers;

     if ( argc < 2 )
     {
          std::cerr << "Too little arguments\n";
          print_usage( std::cerr );
          return EXIT_FAILURE;
     }

     // read arguments
     for ( int i = 1; i < argc; i++ )
     {
          std::string arg{ argv[ i ] };
          if ( arg == help_long || arg == help_short )
          {
               print_usage( std::cout );
               return EXIT_SUCCESS;
          }
          else if ( arg == quiet_long || arg == quiet_short )
          {
               quiet = true;
          }
          else if ( arg == unpack_long || arg == unpack_short )
          {
               unpack = true;
          }
          else if ( arg == out_dir_long || arg == out_dir_short )
          {
               if( i + 1 >= argc )
               {
                    std::cerr << "Error: option " << arg << " requires an argument\n";
                    print_usage( std::cerr );
                    return EXIT_FAILURE;
               }
               out_dir = argv[ i + 1 ];
               std::error_code ec;
               if ( !std::filesystem::is_directory( out_dir, ec ) || ec )
               {
                    std::cerr << "Error: directory " << out_dir << " does not exist\n";
                    return EXIT_FAILURE;
               }
               i++;
          }
          else if ( arg == src_format_long || arg == src_format_short )
          {
               if( i + 1 >= argc )
               {
                    std::cerr << "Error: option " << arg << " requires an argument\n";
                    print_usage( std::cerr );
                    return EXIT_FAILURE;
               }
               if ( !str_to_format( std::string{ argv[ i + 1 ] }, src_format ) )
               {
                    std::cerr << "Error: incorrect source format '" << argv[ i + 1 ] << "'\n";
                    print_usage( std::cerr );
                    return EXIT_FAILURE;
               }
               i++;
          }
          else if ( arg == dst_format_long || arg == dst_format_short )
          {
               if( i + 1 >= argc )
               {
                    std::cerr << "Error: option " << arg << " requires an argument\n";
                    print_usage( std::cerr );
                    return EXIT_FAILURE;
               }
               if ( !str_to_format( std::string{ argv[ i + 1 ] }, dst_format ) )
               {
                    std::cerr << "Error: incorrect destination format '" << argv[ i + 1 ] << "'\n";
                    print_usage( std::cerr );
                    return EXIT_FAILURE;
               }
               i++;
          }
          else if ( arg == pack_long || arg == pack_short )
          {
               if ( i + 1 >= argc )
               {
                    std::cerr << "Error: option " << arg << " requires an argument\n";
                    print_usage( std::cerr );
                    return EXIT_FAILURE;
               }
               package_name = argv[ i + 1 ];
               pack = true;
               i++;
          }
          else
          {
               files.push_back( arg );
          }
     }

     if ( files.empty() )
     {
          std::cerr << "Error: no input files specified\n";
          print_usage( std::cerr );
          return EXIT_FAILURE;
     }

     // convert assets
     auto reader = create_reader( src_format );
     auto writer = create_writer( dst_format );
     if ( !reader || !writer )
     {
          return EXIT_FAILURE;
     }

     for ( const auto& filename : files )
     {
          if ( !quiet )
          {
               std::cout << "Reading asset " << filename << "...";
          }
          _16nar::tools::ResourceData data;
          try
          {
               std::ifstream ifs{ filename };
               reader->read_asset( ifs, data );
               if ( !quiet )
               {
                    std::cout << "done\n\tWriting converted asset...";
               }
               writer->write_asset( data, out_dir );
               if ( !quiet )
               {
                    std::cout << "done\n";
               }
          }
          catch ( const std::exception& ex )
          {
               std::cout << "error: " << ex.what();
               return EXIT_FAILURE;
          }
     }

     return EXIT_SUCCESS;
}
