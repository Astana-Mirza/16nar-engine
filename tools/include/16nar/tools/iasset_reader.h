/// @file
/// @brief File with definition of IAssetReader interface.
#ifndef _16NAR_TOOLS_IASSET_READER_H
#define _16NAR_TOOLS_IASSET_READER_H

#include <16nar/tools/resource_package.h>
#include <16nar/tools/ifile_operator.h>

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
     /// @throw std::exception and its derivatives.
     virtual ResourceData read_asset( std::istream& input ) = 0;

     /// @brief Read package data with multiple assets.
     /// @param[in] input input data stream.
     /// @return package read from data stream.
     /// @throw std::exception and its derivatives.
     virtual PackageData read_package( std::istream& input ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IASSET_READER_H
