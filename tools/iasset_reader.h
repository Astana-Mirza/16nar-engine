/// @file
/// @brief File with definition of IAssetReader interface.
#ifndef _16NAR_TOOLS_IASSET_READER_H
#define _16NAR_TOOLS_IASSET_READER_H

#include <resource_package.h>

#include <iosfwd>

namespace _16nar::tools
{

/// @brief Interface for reading asset data.
class ENGINE_API IAssetReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetReader() = default;

     /// @brief Read single asset data.
     /// @param[in] input input data stream.
     virtual void read_asset( std::istream& input, ResourceData& resource ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IASSET_READER_H
