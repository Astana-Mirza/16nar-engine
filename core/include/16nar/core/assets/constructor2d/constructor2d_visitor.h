/// @file
/// @brief File with Constructor2DVisitor class definition.
#ifndef _16NAR_CORE_ASSETS_CONSTRUCTOR2D_VISITOR_H
#define _16NAR_CORE_ASSETS_CONSTRUCTOR2D_VISITOR_H

#include <16nar/core/assets/defs.h>

#include <16nar/core/assets/iasset_visitor.h>

namespace _16nar::assets
{

/// @brief Class for scene traversal processing.
class NARENGINE_CORE_API Constructor2DVisitor : public IAssetVisitor
{
public:
     /// @brief Actions of scene parsing.
     enum Actions : AssetActionId
     {
          SceneBegin        = 1,   ///< start parsing of the scene.
          StatesBegin       = 2,   ///< start parsing scene states.
          StateEnter        = 3,   ///< parse new scene state.
          RenderSystemEnter = 4,   ///< parse render system of scene state.
          StatesEnd         = 5,   ///< finish parsing scene states.
          SceneEnd          = 6,   ///< finish parsing of the scene.
     };

     /// @copydoc IAssetVisitor::visit(AssetActionId, const IAssetReader&)
     void visit( AssetActionId action_id, const IAssetReader& reader ) override;

     /// @brief Process scene tree using the reader and make visits.
     /// @throws std::runtime_error or format-specific exceptions in case of parsing errors.
     /// @param[in] reader asset reader.
     void process_scene( IAssetReader& reader );
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_CONSTRUCTOR2D_VISITOR_H
