/// @file
/// @brief File with definition of IAssetWriter interface.
#ifndef _16NAR_TOOLS_IASSET_WRITER_H
#define _16NAR_TOOLS_IASSET_WRITER_H

#include <resource_package.h>

#include <string>

namespace _16nar::tools
{

/// @brief Interface for writing asset data.
class ENGINE_API IAssetWriter
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetWriter() = default;

     /// @brief Write single asset data.
     /// @param[in] resource resource parameters to be written.
     /// @param[in] dir output directory of file to be written.
     virtual void write_asset( const ResourceData& resource, const std::string& dir ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IASSET_WRITER_H
