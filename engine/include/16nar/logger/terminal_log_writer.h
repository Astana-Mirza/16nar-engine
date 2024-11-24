/// @file Header file with TerminalLogWriter class declaration.
#ifndef _16NAR_TERMINAL_LOG_WRITER_H
#define _16NAR_TERMINAL_LOG_WRITER_H

#include <16nar/16nardefs.h>
#include <16nar/logger/base_log_writer.h>

namespace _16nar
{

/// @brief Log writer which writes log to standard error of current terminal.
class ENGINE_API TerminalLogWriter : public BaseLogWriter
{
public:
     /// @brief Constructor.
     /// @param[in] colorized should the output be colorized.
     TerminalLogWriter( bool colorized = true ) noexcept;

     /// @copydoc ILogWriter::write(ILogWriter::LogLevel,std::string_view)
     virtual void write( ILogWriter::LogLevel level, std::string_view str ) override;

     /// @copydoc BaseLogWriter::log_level_to_string(ILogWriter::LogLevel)
     virtual std::string log_level_to_string( ILogWriter::LogLevel level ) const override; 

private:
     bool colorized_;    ///< should the output be colorized.
};

} // namespace _16nar

#endif // #ifndef _16NAR_TERMINAL_LOG_WRITER_H
