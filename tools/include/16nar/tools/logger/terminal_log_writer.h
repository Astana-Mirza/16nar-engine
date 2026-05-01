/// @file Header file with TerminalLogWriter class declaration.
#ifndef _16NAR_TOOLS_TERMINAL_LOG_WRITER_H
#define _16NAR_TOOLS_TERMINAL_LOG_WRITER_H

#include <16nar/tools/defs.h>

#include <16nar/tools/logger/ilog_writer.h>

namespace _16nar::tools
{

/// @brief Log writer which writes log the current terminal.
/// @details Wranings and errors are written to standard error output,
// less critical messages are written to standard output.
class ENGINE_API TerminalLogWriter : public ILogWriter
{
public:
     /// @brief Constructor.
     /// @param[in] colorized should the output be colorized.
     explicit TerminalLogWriter( bool colorized = true ) noexcept;

     /// @copydoc ILogWriter::write(Logger::LogLevel, std::string_view)
     virtual void write( Logger::LogLevel level, std::string_view str ) override;

private:
     bool colorized_;    ///< should the output be colorized.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_TERMINAL_LOG_WRITER_H
