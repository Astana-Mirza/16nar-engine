/// @file
/// @brief File with definition of JsonFileOperator class.
#ifndef _16NAR_TOOLS_JSON_OPERATOR_H
#define _16NAR_TOOLS_JSON_OPERATOR_H

#include <16nar/16nardefs.h>

#include <string>

namespace _16nar::tools
{

/// @brief Class which defines JSON format file extensions.
/// @tparam T class to inherit from (CRTP).
template < typename T >
class JsonFileOperator : public T
{
public:
     /// @copydoc IFileOperator::get_file_ext() const noexcept
     const char *get_file_ext() const noexcept
     {
          return "json";
     }

     /// @copydoc IFileOperator::get_pkg_ext() const noexcept
     const char *get_pkg_ext() const noexcept
     {
          return "json";
     }

};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_JSON_OPERATOR_H
