/// @file
/// @brief File with NameTable class definition.
#ifndef _16NAR_TOOLS_NAME_TABLE_H
#define _16NAR_TOOLS_NAME_TABLE_H

#include <16nar/tools/defs.h>
#include <16nar/tools/misc/static_name.h>

#include <unordered_map>
#include <string>
#include <string_view>

namespace _16nar::tools
{

/// @brief Table of strings of static names.
class ENGINE_API NameTable
{
public:
     /// @brief Get full string by static name.
     /// @details Pretty printed name is allocated in thread-local static buffer.
     /// @param[in] name name object.
     /// @param[in] pretty get pretty printed name in case of unknonwn hash.
     /// @return string with the name data, empty string if data does not exist.
     std::string_view get_name( StaticName name, bool pretty = false ) const noexcept;

     /// @brief Add name to the table.
     /// @param[in] name name to be added.
     /// @return static name object, empty object if the name already exists or if the hash collision happens.
     StaticName add_name( std::string_view name );

     /// @brief Remove name from the table.
     /// @param[in] name name to be removed.
     void remove_name( StaticName name );

private:
     std::unordered_map< StaticName, std::string > table_;  ///< name table.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_NAME_TABLE_H
