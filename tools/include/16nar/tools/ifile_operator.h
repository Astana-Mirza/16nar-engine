/// @file
/// @brief File with definition of IFileOperator interface.
#ifndef _16NAR_TOOLS_IFILE_OPERATOR_H
#define _16NAR_TOOLS_IFILE_OPERATOR_H

#include <16nar/16nardefs.h>

#include <string>

namespace _16nar::tools
{

/// @brief Interface for operating with resource files.
class ENGINE_API IFileOperator
{
public:
     /// @brief Virtual default destructor.
     virtual ~IFileOperator() = default;

     /// @brief Get default file extension for current format.
     /// @return Default file extension for current format.
     virtual std::string get_file_ext() const noexcept = 0;

     /// @brief Get default package extension for current format.
     /// @return Default package extension for current format.
     virtual std::string get_pkg_ext() const noexcept = 0;

};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IFILE_OPERATOR_H
