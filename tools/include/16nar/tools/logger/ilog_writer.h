/// @file Header file with ILogWriter interface declaration.
#ifndef _16NAR_TOOLS_ILOG_WRITER_H
#define _16NAR_TOOLS_ILOG_WRITER_H

#include <16nar/tools/defs.h>

#include <16nar/tools/logger/logger.h>

#include <string_view>

namespace _16nar::tools
{

/// @brief Interface for log writing.
class ENGINE_API ILogWriter
{
public:
     /// @brief Virtual default destructor.
     virtual ~ILogWriter() = default;

     /// @brief Write string to log.
     /// @param[in] level level of logging verbosity.
     /// @param[in] str string to be written to log.
     virtual void write( Logger::LogLevel level, std::string_view str ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_ILOG_WRITER_H
