/// @file Header file with Logger class declaration.
#ifndef _16NAR_LOGGER_H
#define _16NAR_LOGGER_H

#include <16nar/16nardefs.h>
#include <16nar/logger/ilog_writer.h>

#include <string_view>
#include <memory>
#include <vector>
#include <sstream>
#include <mutex>

namespace _16nar
{

/// @brief Class-singleton for writing logs of an application.
/// @details This class is just a storage of log writers, each writing
/// log its own way. To actually write log, one needs to add at least one
/// log writer to the logger.
class ENGINE_API Logger
{
public:
     using WriterPtr = std::unique_ptr< ILogWriter >;

     /// @brief Get single instance of logger.
     /// @return Single instance of logger.
     static Logger& instance();

     /// @brief Write string to log.
     /// @throws May throw std::out_of_range, std::bad_alloc,
     /// std::system_error and std::ios_base::failure.
     /// @param[in] level log level.
     /// @param[in] str string to be written to log.
     void log( ILogWriter::LogLevel level, std::string_view str );

     /// @brief Set current log level.
     /// @param[in] level new log level.
     /// @throws std::system_error.
     void set_log_level( ILogWriter::LogLevel level );

     /// @brief Add log writer to the storage.
     /// @param[in] writer log writer to be added.
     /// @throws std::bad_alloc, std::system_error.
     void add_writer( WriterPtr&& writer );

private:
     Logger() = default;
     Logger( const Logger& ) = delete;
     Logger& operator=( const Logger& ) = delete;

private:
     std::vector< WriterPtr > writers_;                               ///< all added log writers.
     std::mutex mutex_;                                               ///< mutex for thread-safe logging.
     ILogWriter::LogLevel log_level_ = ILogWriter::LogLevel::Info;    ///< current log level.
};

} // namespace _16nar


#define __NARENGINE_LOG_IMPL( LEVEL, EXPR ) \
     {    \
          std::stringstream __narengine_ss;  \
          __narengine_ss << EXPR;  \
          _16nar::Logger::instance().log( _16nar::ILogWriter::LogLevel::LEVEL, __narengine_ss.str() );    \
     }


#if NARENGINE_LOG_LEVEL >= 3
#    define LOG_16NAR_ERROR( EXPR ) __NARENGINE_LOG_IMPL( Error, EXPR )
#else
#    define LOG_16NAR_ERROR( EXPR )
#endif
#if NARENGINE_LOG_LEVEL >= 5
#    define LOG_16NAR_WARNING( EXPR ) __NARENGINE_LOG_IMPL( Warning, EXPR )
#else
#    define LOG_16NAR_WARNING( EXPR )
#endif
#if NARENGINE_LOG_LEVEL >= 7
#    define LOG_16NAR_INFO( EXPR ) __NARENGINE_LOG_IMPL( Info, EXPR )
#else
#    define LOG_16NAR_INFO( EXPR )
#endif
#if NARENGINE_LOG_LEVEL >= 9
#    define LOG_16NAR_DEBUG( EXPR ) __NARENGINE_LOG_IMPL( Debug, EXPR )
#else
#    define LOG_16NAR_DEBUG( EXPR )
#endif

#endif // #ifndef _16NAR_LOGGER_H
