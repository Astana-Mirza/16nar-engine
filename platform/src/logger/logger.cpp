#include <16nar/platform/logger/logger.h>

#include <16nar/platform/logger/ilog_writer.h>

#include <cstdarg>
#include <ctime>
#include <cstdio>

namespace _16nar::logger
{
namespace
{

constexpr const char *log_level_name( Logger::LogLevel level ) noexcept
{
     switch ( level )
     {
          case Logger::LogLevel::Error:      return "ERROR";
          case Logger::LogLevel::Warning:    return "WARNING";
          case Logger::LogLevel::Info:       return "INFO";
          case Logger::LogLevel::Debug:      return "DEBUG";
          default: break;
     }
     return "???";
}

} // anonymous namespace


Logger& Logger::instance()
{
     static Logger logger{};
     return logger;
}


Logger::Logger() = default;
Logger::~Logger() = default;


void Logger::log( LogLevel level, const char *format, ... )
{
     if ( level > log_level_ || !format )
     {
          return;
     }

     constexpr static std::size_t buffer_size{ 4096 };
     char buffer[ buffer_size ];

     std::time_t curr_time = std::time( nullptr );
     std::tm time_struct{};
#if defined( _POSIX_C_SOURCE ) || defined( _BSD_SOURCE ) || defined( _SVID_SOURCE )
     ( void ) localtime_r( &curr_time, &time_struct );
#elif defined( _WIN32 ) || defined( _WIN64 )
     ( void ) localtime_s( &time_struct, &curr_time );
#else
     // thread-unsafe
     time_struct = *std::localtime( &curr_time );
#endif

     std::size_t offset = std::strftime( buffer, buffer_size, "%F %T ", &time_struct );
     offset += std::snprintf( buffer + offset, buffer_size - offset, "[%s]: ", log_level_name( level ) );

     va_list args;
     va_start( args, format );
     std::vsnprintf( buffer + offset, buffer_size - offset, format, args );
     va_end( args );

     std::lock_guard< std::mutex > lock{ mutex_ };
     for ( const auto& writer : writers_ )
     {
          writer->write( level, buffer );
     }
}


void Logger::set_log_level( LogLevel level )
{
     log_level_ = level;
}


void Logger::add_writer( WriterPtr writer )
{
     writers_.emplace_back( std::move( writer ) );
}

} // namespace _16nar::logger
