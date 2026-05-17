/// @file
/// @brief File with DynamicLib class definition.
#ifndef _16NAR_PLATFORM_SYSTEM_DYNAMIC_LIB_H
#define _16NAR_PLATFORM_SYSTEM_DYNAMIC_LIB_H

#include <16nar/platform/defs.h>

#include <string_view>

namespace _16nar::system
{

/// @brief Class for work with dynamically loaded libraries.
class NARENGINE_PLATFORM_API DynamicLib
{
public:
     /// @brief Constructor.
     /// @warning created object may be invalid if the library is not loaded.
     /// Caller should check it via @b is_loaded() member function.
     /// @param[in] name relative path to the library.
     DynamicLib( std::string_view name );

     /// @brief Move constructor.
     /// @param[in] lib rvalue reference to DynamicLib.
     DynamicLib( DynamicLib&& lib ) noexcept;

     /// @brief Move assignment.
     /// @param[in] lib rvalue reference to DynamicLib.
     /// @return current object.
     DynamicLib& operator=( DynamicLib&& lib ) noexcept;

     /// @brief Destructor, closes the handle.
     ~DynamicLib() noexcept;

     /// @brief Check if the library is loaded.
     /// @return true if the library is loaded, false otherwise.
     bool is_loaded() const noexcept;

     /// @brief Get the symbol with given name.
     /// @param[in] name name of the symbol.
     /// @return loaded symbol, nullptr in case of error.
     void *get_symbol( std::string_view name ) const;

private:
     DynamicLib( const DynamicLib& )             = delete;
     DynamicLib& operator= ( const DynamicLib& ) = delete;

     void *handle_; ///< handle of the library.
};

} // namespace _16nar::system

#endif // #ifndef _16NAR_PLATFORM_SYSTEM_DYNAMIC_LIB_H
