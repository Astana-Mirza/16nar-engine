/// @file
/// @brief File with IAssetDataConvertor interface definition.
#ifndef _16NAR_TOOLS_IASSET_DATA_CONVERTOR_H
#define _16NAR_TOOLS_IASSET_DATA_CONVERTOR_H

#include <16nar/tools/defs.h>

#include <memory>
#include <string>

namespace _16nar::tools
{

/// @brief Interface for conversion of asset data between formats.
class ENGINE_API IAssetDataConvertor
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetDataConvertor() = default;

     /// @brief Convert asset data from source format to destination format.
     /// @details Returned data remains valid until next conversion or convertor reset/destruction.
     /// @param[in] content asset data in source format.
     /// @return asset data in destination format, empty data in case of error.
     virtual AssetData convert_forward( AssetData content ) = 0;

     /// @brief Convert asset data from destination format to source format.
     /// @details Returned data remains valid until next conversion or convertor reset/destruction.
     /// @param[in] content asset data in destination format.
     /// @return asset data in source format, empty data in case of error.
     virtual AssetData convert_backward( AssetData content ) = 0;

     /// @brief Get description of latest error.
     /// @return Description of latest error, empty string if there was no error.
     virtual std::string get_error_description() const = 0;
};

using IAssetDataConvertorPtr = std::shared_ptr< IAssetDataConvertor >;

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IASSET_DATA_CONVERTOR_H
