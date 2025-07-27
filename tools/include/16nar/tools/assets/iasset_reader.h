/// @file
/// @brief File with definition of IAssetReader interface.
#ifndef _16NAR_TOOLS_IASSET_READER_H
#define _16NAR_TOOLS_IASSET_READER_H

#include <16nar/tools/assets/resource_package.h>
#include <16nar/tools/assets/ifile_operator.h>

#include <iosfwd>
#include <vector>

namespace _16nar::tools
{

/// @brief Interface for reading asset data.
class ENGINE_API IAssetReader : public IFileOperator
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetReader() = default;

     /// @brief Read single asset data.
     /// @param[in] input input data stream.
     /// @return resource read from data stream.
     /// @throws std::exception and its derivatives.
     virtual ResourceData read_asset( std::istream& input ) = 0;

     /// @brief Read package data with selected assets.
     /// @param[in] input input data stream.
     /// @param[in] names names of selected assets to be loaded from the package (load all if empty).
     /// @return package read from data stream, the package contains only selected assets.
     /// @throws std::exception and its derivatives.
     virtual PackageData read_package( std::istream& input,
          const std::vector< std::string >& names = {} ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IASSET_READER_H
