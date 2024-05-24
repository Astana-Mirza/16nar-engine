/// @file Header file with ILogWriter interface declaration.
#ifndef _16NAR_ILOG_WRITER_H
#define _16NAR_ILOG_WRITER_H

#include <16nar/16nardefs.h>
#include <string_view>

namespace _16nar
{

/// @brief Interface for log writing.
class ENGINE_API ILogWriter
{
public:
     /// @brief Level of log verbosity.
     enum LogLevel
     {
          Error    = 3,  ///< Log only error messages.
          Warning  = 5,  ///< Log warning messages and more critical.
          Info     = 7,  ///< Log information messages and more critical.
          Debug    = 9   ///< Log debug messages and more critical.
     };

     /// @brief Default virtual destructor.
     virtual ~ILogWriter() = default;

     /// @brief Write string to log.
     /// @param[in] level level of logging verbosity.
     /// @param[in] str string to be written to log.
     virtual void write( LogLevel level, std::string_view str ) = 0;
};

} // namespace _16nar

#endif // #ifndef _16NAR_ILOG_WRITER_H
