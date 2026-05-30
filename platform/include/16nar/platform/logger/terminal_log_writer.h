/// @file
/// @brief File with TerminalLogWriter class definition.
#ifndef _16NAR_PLATFORM_LOGGER_TERMINAL_LOG_WRITER_H
#define _16NAR_PLATFORM_LOGGER_TERMINAL_LOG_WRITER_H

#include <16nar/platform/defs.h>

#include <16nar/platform/logger/ilog_writer.h>

namespace _16nar::logger
{

/// @brief Log writer which writes log the current terminal.
/// @details Wranings and errors are written to standard error output,
// less critical messages are written to standard output.
class NARENGINE_PLATFORM_API TerminalLogWriter : public ILogWriter
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

} // namespace _16nar::logger

#endif // #ifndef _16NAR_PLATFORM_LOGGER_TERMINAL_LOG_WRITER_H
