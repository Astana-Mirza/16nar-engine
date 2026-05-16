/// @file
/// @brief File with IAssetVisitor interface definition.
#ifndef _16NAR_CORE_ASSETS_IASSET_VISITOR_H
#define _16NAR_CORE_ASSETS_IASSET_VISITOR_H

#include <16nar/core/assets/defs.h>

#include <16nar/core/assets/iasset_reader.h>

namespace _16nar::assets
{

/// @brief Interface for performing specific actions during asset tree parsing.
class NARENGINE_CORE_API IAssetVisitor
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetVisitor() = default;

     /// @brief Perform action with specified identifier.
     /// @throws std::runtime_error or format-specific exceptions in case of parsing errors.
     /// @param[in] action_id action identifier.
     /// @param[in] reader reader of current asset.
     virtual void visit( AssetActionId action_id, const IAssetReader& reader ) = 0;
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_IASSET_VISITOR_H
