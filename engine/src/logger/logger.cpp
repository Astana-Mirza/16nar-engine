#include <16nar/logger/logger.h>

namespace _16nar
{

Logger& Logger::instance()
{
     static Logger logger;
     return logger;
}


void Logger::log( ILogWriter::LogLevel level, std::string_view str )
{
     std::lock_guard< std::mutex > lock{ mutex_ };
     if ( level <= log_level_ )
     {
          for ( const auto& writer : writers_ )
          {
               writer->write( level, str );
          }
     }
}


void Logger::set_log_level( ILogWriter::LogLevel level )
{
     std::lock_guard< std::mutex > lock{ mutex_ };
     log_level_ = level;
}


void Logger::add_writer( WriterPtr&& writer )
{
     std::lock_guard< std::mutex > lock{ mutex_ };
     writers_.emplace_back( std::move( writer ) );
}

} // namespace _16nar
