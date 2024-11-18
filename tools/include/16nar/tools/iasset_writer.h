/// @file
/// @brief File with definition of IAssetWriter interface.
#ifndef _16NAR_TOOLS_IASSET_WRITER_H
#define _16NAR_TOOLS_IASSET_WRITER_H

#include <16nar/tools/resource_package.h>
#include <16nar/tools/ifile_operator.h>

#include <string>
#include <iosfwd>

namespace _16nar::tools
{

/// @brief Interface for writing asset data.
class ENGINE_API IAssetWriter : public IFileOperator
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetWriter() = default;

     /// @brief Write single asset data.
     /// @param[in] output output data stream.
     /// @param[in] resource resource to be written to data stream.
     /// @throw std::exception and its derivatives.
     virtual void write_asset( std::ostream& output, const ResourceData& resource ) = 0;

     /// @brief Write package data with multiple assets.
     /// @param[in] output output data stream.
     /// @param[in] package package to be written to data stream.
     /// @throw std::exception and its derivatives.
     virtual void write_package( std::ostream& output, const PackageData& package ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IASSET_WRITER_H
