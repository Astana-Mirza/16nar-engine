/// @file
/// @brief File with definitions of different exceptions in the engine.
#ifndef _16NAR_EXCEPTIONS_H
#define _16NAR_EXCEPTIONS_H

#include <16nar/16nardefs.h>
#include <stdexcept>

namespace _16nar
{

/// @brief Exception for resource load/unload error.
class ENGINE_API ResourceException : public std::runtime_error
{
public:
     /// @brief Constructor.
     /// @param[in] id ID of the resource.
     explicit ResourceException( ResID id );

     /// @brief Constructor.
     /// @param[in] msg message to be displayed.
     /// @param[in] id ID of the resource, appended to message if non-zero.
     explicit ResourceException( const std::string& msg, ResID id = 0 );
};


/// @brief Exception thrown when IDs for resources are exceeded.
class ENGINE_API ExceededIdException : public ResourceException
{
public:
     /// @brief Constructor.
     ExceededIdException();
};

} // namespace _16nar

#endif // _16NAR_EXCEPTIONS_H
