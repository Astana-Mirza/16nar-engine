#include <16nar/platform/logger/logger.h>

#include <16nar/platform/logger/terminal_log_writer.h>

using namespace _16nar;

int main( int argc, char *argv[] )
{
     auto& logger = logger::Logger::instance();
     logger.set_log_level( logger::Logger::Info );
     logger.add_writer( std::make_unique< logger::TerminalLogWriter >() );

     LOG_16NAR_INFO( "Hello, 16nar!" );

     return 0;
}
