/// @file
/// @brief File with NameTable class definition.
#ifndef _16NAR_PLATFORM_STRINGS_NAME_TABLE_H
#define _16NAR_PLATFORM_STRINGS_NAME_TABLE_H

#include <16nar/platform/defs.h>
#include <16nar/platform/strings/static_name.h>
#include <16nar/platform/memory/memory_domain.h>

#include <unordered_map>
#include <string>
#include <string_view>
#include <memory>

namespace _16nar::strings
{

/// @brief Table of strings represented with static names.
/// @details In order to use fast integer operations instead of time-consuming string
/// operations, one might use hashed strings represented with @ref StaticName.
///
/// Name table contains mapping of static names into strings and allows to get full
/// string by its static name. It also can check if the string is saved in the table.
///
/// There is small probability of hash collision of strings. Such situation is repoted
/// via error message in log.
class NARENGINE_PLATFORM_API NameTable
{
public:
     /// @brief Constructor.
     /// @param[in] domain memory domain for name tables allocation.
     NameTable( memory::MemoryDomain& domain );

     /// @brief Get static name of string if it is contained in the table.
     /// @param[in] name string to be checked.
     /// @return static name of string if it exists in the table, empty name otherwise.
     StaticName get_static_name( std::string_view name ) const;

     /// @brief Get full string view by static name if it exists in table.
     /// @details If the name does not exist in the table, one can get a pretty-printed
     /// string representing hash of requested string. It can be used for display
     /// in diagnostic messages. Pretty printed name is allocated in thread-local static buffer.
     /// @param[in] name name object.
     /// @param[in] pretty get pretty printed name in case of unknown hash.
     /// @return string with the name data, empty string if data does not exist.
     std::string_view get_name( StaticName name, bool pretty = false ) const noexcept;

     /// @brief Add name to the table.
     /// @param[in] name name to be added.
     /// @return static name object, empty object if the name already exists or if the hash collision happens.
     StaticName add_name( std::string_view name );

     /// @brief Remove name from the table.
     /// @details In order to avoid string hash collision, it is recommended to pass
     /// only the names returned with @ref get_name called with the same table.
     /// @param[in] name name to be removed.
     void remove_name( StaticName name );

private:
     std::pmr::unordered_map< StaticName, std::pmr::string > table_;  ///< name table.
};

} // namespace _16nar::strings

#endif // #ifndef _16NAR_PLATFORM_STRINGS_NAME_TABLE_H
