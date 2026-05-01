#include <16nar/tools/logger/terminal_log_writer.h>

#include <cstdio>

namespace _16nar::tools
{
namespace
{

constexpr const char *log_level_color( Logger::LogLevel level ) noexcept
{
     switch ( level )
     {
          case Logger::LogLevel::Error:      return "\x1B[31m";
          case Logger::LogLevel::Warning:    return "\x1B[33m";
          case Logger::LogLevel::Info:       return "\x1B[94m";
          case Logger::LogLevel::Debug:      return "\x1B[90m";
          default: break;
     }
     return "";
}

} // anonymous namespace


TerminalLogWriter::TerminalLogWriter( bool colorized ) noexcept:
     colorized_{ colorized }
{}


void TerminalLogWriter::write( Logger::LogLevel level, std::string_view str )
{
     FILE *out = ( level <= Logger::LogLevel::Warning ) ? stderr : stdout;
     if ( colorized_ )
     {
          constexpr static const char *color_remover = "\033[0m";
          std::fprintf( out, "%s%s%s\n", log_level_color( level ), str.data(), color_remover );
          return;
     }
     std::fprintf( out, "%s\n", str.data() );
}

} // namespace _16nar::tools
