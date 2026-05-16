/// @file Header file with Logger class declaration.
#ifndef _16NAR_PLATFORM_LOGGER_LOGGER_H
#define _16NAR_PLATFORM_LOGGER_LOGGER_H

#include <16nar/platform/defs.h>

#include <memory>
#include <vector>
#include <mutex>

namespace _16nar::logger
{

class ILogWriter;

/// @brief Class-singleton for writing logs of an application.
/// @details This class is just a storage of log writers, each writing
/// log its own way. To actually write log, one needs to add at least one
/// log writer to the logger.
class NARENGINE_PLATFORM_API Logger
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

     using WriterPtr = std::unique_ptr< ILogWriter >;

     /// @brief Get single instance of logger.
     /// @return single instance of logger.
     static Logger& instance();

     /// @brief Destructor.
     ~Logger();

     /// @brief Write formatted string to log.
     /// @details Text line written to log has the following pattern:
     /// "YYYY-MM-DD HH:MM:ss [LEVEL]: formatted message". Maximum length of
     /// the text line is 4096 characters.
     /// @param[in] level log level.
     /// @param[in] format format string to be written to log.
     /// @param[in] ... format string arguments
     void log( LogLevel level, const char *format, ... );

     /// @brief Set current log level.
     /// @warning The function is not thread-safe.
     /// @param[in] level new log level.
     void set_log_level( LogLevel level );

     /// @brief Add log writer to the storage.
     /// @warning The function is not thread-safe.
     /// @param[in] writer log writer to be added.
     void add_writer( WriterPtr writer );

private:
     /// @brief Default constructor.
     Logger();

     Logger( const Logger& ) = delete;
     Logger& operator=( const Logger& ) = delete;

private:
     std::vector< WriterPtr > writers_;      ///< all added log writers.
     std::mutex mutex_;                      ///< mutex for thread-safe logging.
     LogLevel log_level_ = LogLevel::Info;   ///< current log level.
};

} // namespace _16nar::logger


#define __NARENGINE_LOG_IMPL( LEVEL, FORMAT, ... ) \
     ::_16nar::logger::Logger::instance().log( \
          ::_16nar::logger::Logger::LogLevel::LEVEL, ( FORMAT ), ##__VA_ARGS__ )


#if NARENGINE_LOG_LEVEL >= 3
#    define LOG_16NAR_ERROR( FORMAT, ... ) __NARENGINE_LOG_IMPL( Error, ( FORMAT ), ##__VA_ARGS__ )
#else
#    define LOG_16NAR_ERROR( FORMAT, ... )
#endif
#if NARENGINE_LOG_LEVEL >= 5
#    define LOG_16NAR_WARNING( FORMAT, ... ) __NARENGINE_LOG_IMPL( Warning, ( FORMAT ), ##__VA_ARGS__ )
#else
#    define LOG_16NAR_WARNING( FORMAT, ... )
#endif
#if NARENGINE_LOG_LEVEL >= 7
#    define LOG_16NAR_INFO( FORMAT, ... ) __NARENGINE_LOG_IMPL( Info, ( FORMAT ), ##__VA_ARGS__ )
#else
#    define LOG_16NAR_INFO( FORMAT, ... )
#endif
#if NARENGINE_LOG_LEVEL >= 9
#    define LOG_16NAR_DEBUG( FORMAT, ... ) __NARENGINE_LOG_IMPL( Debug, ( FORMAT ), ##__VA_ARGS__ )
#else
#    define LOG_16NAR_DEBUG( FORMAT, ... )
#endif

#endif // #ifndef _16NAR_PLATFORM_LOGGER_LOGGER_H
