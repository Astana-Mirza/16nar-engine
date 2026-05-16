/// @file
/// @brief File with ResourcePkgVisitor class definition.
#ifndef _16NAR_CORE_RESOURCES_RESOURCE_PKG_VISITOR_H
#define _16NAR_CORE_RESOURCES_RESOURCE_PKG_VISITOR_H

#include <16nar/core/resources/defs.h>
#include <16nar/core/assets/iasset_visitor.h>

namespace _16nar::resources
{

/// @brief Class for resource package traversal processing.
class NARENGINE_CORE_API ResourcePkgVisitor : public assets::IAssetVisitor
{
public:
     /// @brief Actions of resource package parsing.
     enum Actions : assets::AssetActionId
     {
          PackageBegin      = 1,   ///< start parsing the resource packge.
          ResourceEnter     = 2,   ///< parse new resource of the packge (optional).
          PackageEnd        = 3,   ///< finish parsing the package.
          ResourceBegin     = 4,   ///< start parsing the resource.
          ResourceEnd       = 5,   ///< finish parsing the resource.
     };

     /// @copydoc IAssetVisitor::visit(assets::AssetActionId, const assets::IAssetReader&)
     void visit( assets::AssetActionId action_id, const assets::IAssetReader& reader ) override;

     /// @brief Process resource package tree using the reader and make visits.
     /// @throws std::runtime_error or format-specific exceptions in case of parsing errors.
     /// @param[in] reader asset reader.
     void process_resource_package( assets::IAssetReader& reader );

     /// @brief Process single resource tree using the reader and make visits.
     /// @throws std::runtime_error or format-specific exceptions in case of parsing errors.
     /// @param[in] reader asset reader.
     void process_resource( assets::IAssetReader& reader );
};

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_RESOURCE_PKG_VISITOR_H
