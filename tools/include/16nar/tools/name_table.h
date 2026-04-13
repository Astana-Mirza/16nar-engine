/// @file
/// @brief File with NameTable class definition.
#ifndef _16NAR_TOOLS_NAME_TABLE_H
#define _16NAR_TOOLS_NAME_TABLE_H

#include <16nar/tools/defs.h>
#include <16nar/tools/static_name.h>

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
     /// @param[in] name name object.
     /// @return string with the name data, empty string if data does not exist.
     std::string_view get_name( StaticName name ) const noexcept;

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
