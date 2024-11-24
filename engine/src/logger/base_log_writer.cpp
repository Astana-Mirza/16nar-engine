#include <16nar/logger/base_log_writer.h>

#include <unordered_map>
#include <ctime>

namespace _16nar
{

std::string BaseLogWriter::log_level_to_string( ILogWriter::LogLevel level ) const
{
     static const std::unordered_map< ILogWriter::LogLevel, std::string > levels{
          { ILogWriter::LogLevel::Error,     "[ERROR]"   },
          { ILogWriter::LogLevel::Warning,   "[WARNING]" },
          { ILogWriter::LogLevel::Info,      "[INFO]"    },
          { ILogWriter::LogLevel::Debug,     "[DEBUG]"   },
     };
     return levels.at( level );
}


std::string BaseLogWriter::get_timestamp() const
{
     std::time_t curr_time = std::time( nullptr );
     char strbuf[ std::size( "YYYY-MM-DD HH:MM:ss" ) + 1 ];
     struct tm time_struct;
#if defined( _POSIX_C_SOURCE ) || defined( _BSD_SOURCE ) || defined( _SVID_SOURCE )
     ( void ) localtime_r( &curr_time, &time_struct );
#elif defined( _WIN32 ) || defined( _WIN64 )
     ( void ) localtime_s( &time_struct, &curr_time );
#else
     // thread-unsafe
     time_struct = *std::localtime( &curr_time );
#endif
     std::strftime( std::data( strbuf ), std::size( strbuf ), "%F %T", &time_struct );
     return std::string{ strbuf };
}

} // namespace _16nar
