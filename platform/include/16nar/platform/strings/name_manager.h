/// @file
/// @brief File with NameManager class definition.
#ifndef _16NAR_PLATFORM_STRINGS_NAME_MANAGER_H
#define _16NAR_PLATFORM_STRINGS_NAME_MANAGER_H

#include <16nar/platform/defs.h>

#include <16nar/platform/strings/name_table.h>
#include <16nar/platform/memory/imemory_domain.h>

namespace _16nar::strings
{

/// @brief Manager of static name tables.
class NARENGINE_PLATFORM_API NameManager
{
public:
     /// @brief Constructor.
     /// @param[in] domain memory domain for name tables allocation.
     NameManager( memory::IMemoryDomain& domain );

     /// @brief Add new name table.
     /// @details Attempt to create duplicate name table (with the same name) is an error.
     /// @param[in] name name of the name table.
     /// @return pointer to the created table in case of success, nullptr otherwise.
     NameTable *add_table( std::string_view name );

     /// @brief Get name table by its name.
     /// @param[in] name name of the name table.
     /// @return constant name table pointer, nullptr if not found.
     const NameTable *get_table( std::string_view name ) const;

     /// @brief Get name table by its name.
     /// @param[in] name name of the name table.
     /// @return name table pointer, nullptr if not found.
     NameTable *get_table( std::string_view name );

     /// @brief Remove name table by its name.
     /// @param[in] name name of the name table.
     void remove_table( std::string_view name );

private:
     NameManager( const NameManager& ) = delete;
     NameManager& operator=( const NameManager& ) = delete;

private:
     NameTable table_names_;                                     ///< table of table names.
     std::pmr::unordered_map< StaticName, NameTable > tables_;   ///< name tables.
     memory::IMemoryDomain& domain_;                             ///< memory domain for allocations.
};

} // namespace _16nar::strings

#endif // #ifndef _16NAR_PLATFORM_STRINGS_NAME_MANAGER_H

