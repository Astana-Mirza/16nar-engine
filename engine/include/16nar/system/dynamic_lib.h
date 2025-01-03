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
     /// @param[in] name relative path to the library.
     /// @throws std::runtime_error.
     DynamicLib( const std::string& name );

     /// @brief Move constructor.
     /// @param[in] lib rvalue reference to DynamicLib.
     DynamicLib( DynamicLib&& lib ) noexcept;

     /// @brief Move assignment.
     /// @param[in] lib rvalue reference to DynamicLib.
     /// @return Reference to *this.
     DynamicLib& operator= ( DynamicLib&& lib ) noexcept;

     /// @brief Destructor, closes the handle.
     ~DynamicLib();

     /// @brief Get the symbol with given name.
     /// @param[in] name name of the symbol.
     /// @return loaded symbol, nullptr in case of error.
     /// @throws std::runtime_error.
     void *get_symbol( const std::string& name ) const;

private:
     // no copy construction and assignment
     DynamicLib( const DynamicLib& )             = delete;
     DynamicLib& operator= ( const DynamicLib& ) = delete;

     std::string name_;                 ///< name of the library.
     void *handle_ = nullptr;           ///< pointer to a handle of the library.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DYNAMIC_LIB_H
