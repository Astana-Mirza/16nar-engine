/// @file
/// @brief File with DynamicLib class definition.
#ifndef _16NAR_DYNAMIC_LIB_H
#define _16NAR_DYNAMIC_LIB_H

#include <16nar/16nardefs.h>

#include <string>

namespace _16nar
{

/// @brief Class for work with dynamically loaded libraries.
class ENGINE_API DynamicLib
{
public:
     /// @brief Constructor.
     /// @param name relative path to the library.
     DynamicLib( const std::string& name );

     // no copy construction and assignment
     DynamicLib( const DynamicLib& )             = delete;
     DynamicLib& operator= ( const DynamicLib& ) = delete;

     /// @brief Move constructor.
     /// @param lib rvalue reference to DynamicLib.
     DynamicLib( DynamicLib&& lib ) noexcept;

     /// @brief Move assignment.
     /// @param lib rvalue reference to DynamicLib.
     DynamicLib& operator= ( DynamicLib&& lib ) noexcept;

     /// @brief Destructor, which closes the handle.
     ~DynamicLib();

     /// @brief Get the symbol with given name.
     /// @param name name of the symbol.
     void *get_symbol( const std::string& name ) const;

private:
     std::string name_;                 ///< name of the library.
     void *handle_ = nullptr;           ///< pointer to a handle of the library.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DYNAMIC_LIB_H
