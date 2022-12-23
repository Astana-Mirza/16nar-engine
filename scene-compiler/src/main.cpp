#include <iostream>
#include <string>
#include <cstdlib>

#include <compiler.h>

void usage( const std::string& prog_name )
{
     std::cout << "Usage:\n" << prog_name
                           << " OPTION <input_file> <output_file>\n\n"
                           << "Options:\n\t-s\tcompile a scene,\n"
                           << "\t-r\tcompile a resource package." << std::endl;
}


int main( int argc, char *argv[] )
{
     if ( argc != 4 )
     {
          usage( argv[ 0 ] );
          return EXIT_FAILURE;
     }
     try
     {
          Compiler compiler( argv[ 2 ] );
          if ( std::string( argv[ 1 ] ) == "-r" )
          {
               compiler.compile_package( argv[ 3 ] );
          }
          else if ( std::string( argv[ 1 ] ) == "-s" )
          {
               compiler.compile_scene( argv[ 3 ] );
          }
          else
          {
               usage( argv[ 0 ] );
               return EXIT_FAILURE;
          }
     }
     catch ( std::exception& ex )
     {
          std::cerr << "Error: " << ex.what() << std::endl;
          return EXIT_FAILURE;
     }

     return EXIT_SUCCESS;
}
