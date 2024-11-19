/// @file
/// @brief Utility for engine resource management.

#include <16nar/tools/json_asset_reader.h>
#include <16nar/tools/json_asset_writer.h>
#include <16nar/tools/flatbuffers_asset_reader.h>
#include <16nar/tools/flatbuffers_asset_writer.h>

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
constexpr char base_dir_long[]    = "--base-dir";
constexpr char base_dir_short[]   = "-b";
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


std::vector< std::string > files;
std::string out_dir = ".";
std::string base_dir = ".";
std::string package_name;
bool quiet = false;


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


std::unique_ptr< _16nar::tools::IAssetReader > create_reader( const std::string& in_dir,
     _16nar::tools::PackageFormat format )
{
     switch ( format )
     {
          case _16nar::tools::PackageFormat::FlatBuffers:
               return std::make_unique< _16nar::tools::FlatBuffersAssetReader >();
          case _16nar::tools::PackageFormat::Json:
               return std::make_unique< _16nar::tools::JsonAssetReader >( in_dir );
          default:
               std::cerr << "Error: wrong reader format (" << static_cast< std::size_t >( format ) << ")\n";
     }
     return {};
}


std::unique_ptr< _16nar::tools::IAssetWriter > create_writer( const std::string& out_dir,
     _16nar::tools::PackageFormat format )
{
     switch ( format )
     {
          case _16nar::tools::PackageFormat::FlatBuffers:
               return std::make_unique< _16nar::tools::FlatBuffersAssetWriter >();
          case _16nar::tools::PackageFormat::Json:
               return std::make_unique< _16nar::tools::JsonAssetWriter >( out_dir );
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
          << "\n\t\t--base-dir DIR, -b DIR\n\t\tDirectory (it must exist) which will be used as base for input files, current directory is default.\n"
          << "\n\t\t--out-dir DIR, -o DIR\n\t\tOutput directory of the utility, current directory is default.\n"
          << "\n\t\t--src-format FORMAT, -s FORMAT\n\t\tFormat of input file(s), default is json.\n"
          << "\n\t\t--dst-format FORMAT, -d FORMAT\n\t\tFormat of output file(s), default is flatbuffers.\n"
          << "\n\t\t--pack PACKAGE_NAME, -p PACKAGE_NAME\n\t\tCreate a package with given name from input files."
          << " File extension of the package will be set depending on output format. PACKAGE_NAME is treated relative to output directory.\n"
          << "\n\t\t--unpack PACKAGE_NAME, -u PACKAGE_NAME\n\t\tUnpack package and place all resource files to output directory.\n"
          << "\n\t\t--quiet, -q\n\t\tDisable text output to terminal.\n"
          << "\n\tFORMATS:\n"
          << "\t\tjson\n\t\tJSON format. When used as output format, will generate binary assets without converting"
          << " to any specific format. For example, PNG file will not be created when unpacking texture asset from"
          << " flatbuffer format. Instead, there will be JSON file with description and BIN file with raw texture data.\n"
          << "\n\t\tflatbuffers\n\t\tFlatBuffers buffer format, but size of the buffer (uint32_t) will be prepended to buffer.\n";
}


int simple_convert( _16nar::tools::IAssetReader& reader, _16nar::tools::IAssetWriter& writer )
{
     for ( const auto& filename : files )
     {
          if ( filename.empty() )
          {
               continue;
          }

          _16nar::tools::ResourceData data{};
          try
          {
               if ( !quiet )
               {
                    std::cout << "Reading asset " << filename << "...";
               }
               std::ifstream ifs{ filename, std::ios::in | std::ios::binary };
               data = reader.read_asset( ifs );
          }
          catch ( const std::exception& ex )
          {
               std::cerr << "error reading asset: " << ex.what() << "\n";
               return EXIT_FAILURE;
          }

          try
          {
               std::string out_file = ( std::filesystem::path{ out_dir } /
                    ( data.name + "." + writer.get_file_ext() ) ).string();
               if ( !quiet )
               {
                    std::cout << "done\n\tWriting converted asset " << out_file << "...";
               }
               std::ofstream ofs{ out_file, std::ios::out | std::ios::binary };
               writer.write_asset( ofs, data );
               if ( !quiet )
               {
                    std::cout << "done\n";
               }
          }
          catch ( const std::exception& ex )
          {
               std::cerr << "error writing asset: " << ex.what() << "\n";
               return EXIT_FAILURE;
          }
     }
     return EXIT_SUCCESS;
}


int pack_convert( _16nar::tools::IAssetReader& reader, _16nar::tools::IAssetWriter& writer )
{
     _16nar::tools::PackageData package{};
     for ( const auto& filename : files )
     {
          if ( filename.empty() )
          {
               continue;
          }

          try
          {
               if ( !quiet )
               {
                    std::cout << "Reading asset " << filename << "...";
               }
               std::ifstream ifs{ filename, std::ios::in | std::ios::binary };
               package.resources.emplace_back( reader.read_asset( ifs ) );
               if ( !quiet )
               {
                    std::cout << "done\n";
               }
          }
          catch ( const std::exception& ex )
          {
               std::cerr << "error reading asset: " << ex.what() << "\n";
               return EXIT_FAILURE;
          }
     }

     try
     {
          std::string file = ( std::filesystem::path{ out_dir } /
                    ( package_name + "." + writer.get_pkg_ext() ) ).string();
          if ( !quiet )
          {
               std::cout << "\tWriting package " << file << "...";
          }
          std::ofstream ofs{ file, std::ios::out | std::ios::binary };
          writer.write_package( ofs, package );
          if ( !quiet )
          {
               std::cout << "done\n";
          }
     }
     catch ( const std::exception& ex )
     {
          std::cerr << "error writing package: " << ex.what() << "\n";
          return EXIT_FAILURE;
     }
     return EXIT_SUCCESS;
}


int unpack_convert( _16nar::tools::IAssetReader& reader, _16nar::tools::IAssetWriter& writer )
{
     _16nar::tools::PackageData package{};
     try
     {
          if ( !quiet )
          {
               std::cout << "Reading package " << package_name << "...";
          }
          std::ifstream ifs{ package_name, std::ios::in | std::ios::binary };
          package = reader.read_package( ifs );
          if ( !quiet )
          {
               std::cout << "done\n";
          }
     }
     catch ( const std::exception& ex )
     {
          std::cerr << "error reading package: " << ex.what() << "\n";
          return EXIT_FAILURE;
     }

     try
     {
          for ( const auto& resource : package.resources )
          {
               std::string file = ( std::filesystem::path{ out_dir } /
                    ( resource.name + "." + writer.get_file_ext() ) ).string();
               if ( !quiet )
               {
                    std::cout << "\tWriting asset " << file << "...";
               }
               std::ofstream ofs{ file, std::ios::out | std::ios::binary };
               writer.write_asset( ofs, resource );
               if ( !quiet )
               {
                    std::cout << "done\n";
               }
          }
     }
     catch ( const std::exception& ex )
     {
          std::cerr << "error writing asset: " << ex.what() << "\n";
          return EXIT_FAILURE;
     }
     return EXIT_SUCCESS;
}

} // anonymous namespace

int main( int argc, char *argv[] )
{
     bool pack = false;
     bool unpack = false;
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
               if ( i + 1 >= argc )
               {
                    std::cerr << "Error: option " << arg << " requires an argument\n";
                    print_usage( std::cerr );
                    return EXIT_FAILURE;
               }
               package_name = argv[ i + 1 ];
               unpack = true;
               i++;
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
                    if ( !std::filesystem::create_directories( out_dir, ec ) || ec )
                    {
                         std::cerr << "Error: cannot create directory " << out_dir << "\n";
                         return EXIT_FAILURE;
                    }
               }
               i++;
          }
          else if ( arg == base_dir_long || arg == base_dir_short )
          {
               if( i + 1 >= argc )
               {
                    std::cerr << "Error: option " << arg << " requires an argument\n";
                    print_usage( std::cerr );
                    return EXIT_FAILURE;
               }
               base_dir = argv[ i + 1 ];
               std::error_code ec;
               if ( !std::filesystem::is_directory( base_dir, ec ) || ec )
               {
                    std::cerr << "Error: directory " << base_dir << " does not exist\n";
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

     bool error = false;
     if ( pack && unpack )
     {
          std::cerr << "Error: cannot both pack and unpack simultaneously\n";
          error = true;
     }

     if ( ( pack || unpack ) && package_name.empty() )
     {
          std::cerr << "Error: package name is empty\n";
          error = true;
     }

     if ( out_dir.empty() || base_dir.empty() )
     {
          std::cerr << "Error: wrong directory specified\n";
          error = true;
     }

     if ( files.empty() && !unpack )
     {
          std::cerr << "Error: no input files specified\n";
          error = true;
     }
     else if ( unpack && !files.empty() )
     {
          std::cerr << "Error: excess input files specified when unpacking (only package name expected)\n";
          error = true;
     }

     if ( error )
     {
          print_usage( std::cerr );
          return EXIT_FAILURE;
     }

     // convert assets
     auto reader = create_reader( base_dir, src_format );
     auto writer = create_writer( out_dir, dst_format );
     if ( !reader || !writer )
     {
          return EXIT_FAILURE;
     }

     int exit_code = EXIT_SUCCESS;
     if ( pack )
     {
          exit_code = pack_convert( *reader, *writer );
     }
     else if ( unpack )
     {
          exit_code = unpack_convert( *reader, *writer );
     }
     else
     {
          exit_code = simple_convert( *reader, *writer );
     }
     return exit_code;
}
