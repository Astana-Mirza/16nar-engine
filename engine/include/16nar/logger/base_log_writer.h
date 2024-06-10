/// @file Header file with BaseLogWriter class declaration.
#ifndef _16NAR_BASE_LOG_WRITER_H
#define _16NAR_BASE_LOG_WRITER_H

#include <16nar/logger/ilog_writer.h>

#include <string>

namespace _16nar
{

/// @brief Base class for log writing.
class ENGINE_API BaseLogWriter : public ILogWriter
{
public:
     /// @brief Convert log level to string.
     /// @param[in] level log level.
     /// @return string representation of log level.
     virtual std::string log_level_to_string( ILogWriter::LogLevel level ) const;

     /// @brief Get string representation of current time.
     /// @return string representation of current time.
     virtual std::string get_timestamp() const;
};

}

#endif // #ifndef _16NAR_BASE_LOG_WRITER_H
