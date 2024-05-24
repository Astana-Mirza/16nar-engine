#include <16nar/logger/terminal_log_writer.h>

#include <iostream>
#include <unordered_map>

namespace _16nar
{

TerminalLogWriter::TerminalLogWriter( bool colorized ):
     colorized_{ colorized }
{}


void TerminalLogWriter::write( ILogWriter::LogLevel level, std::string_view str )
{
     std::cerr << get_timestamp() << ' ' << log_level_to_string( level ) << ": " << str << std::endl;
}


std::string TerminalLogWriter::log_level_to_string( ILogWriter::LogLevel level ) const
{
     static constexpr char color_remover[] = "\033[0m";
     static const std::unordered_map< ILogWriter::LogLevel, std::string > colors{
          { ILogWriter::LogLevel::Error,     "\x1B[31m" },
          { ILogWriter::LogLevel::Warning,   "\x1B[33m" },
          { ILogWriter::LogLevel::Info,      "\x1B[94m" },
          { ILogWriter::LogLevel::Debug,     "\x1B[90m" },
     };
     std::string level_str = BaseLogWriter::log_level_to_string( level );
     if ( colorized_ )
     {
          level_str = colors.at( level ) + level_str + color_remover;
     }
     return level_str;
}

} // namespace _16nar
