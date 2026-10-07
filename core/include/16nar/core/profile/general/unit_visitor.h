/// @file
/// @brief File with UnitVisitor class definition.
#ifndef _16NAR_CORE_PROFILE_GENERAL_UNIT_VISITOR_H
#define _16NAR_CORE_PROFILE_GENERAL_UNIT_VISITOR_H

#include <16nar/core/assets/iasset_visitor.h>

namespace _16nar::profile
{

/// @brief Class for unit of load traversal processing.
class NARENGINE_CORE_API UnitVisitor : public assets::IAssetVisitor
{
public:
     /// @brief Actions of resource package parsing.
     enum Actions : assets::AssetActionId
     {
          DepEnter       = 1, ///< parse dependencies list of the unit of load.
          ExtBegin       = 2, ///< start parsing extensions data.
          ExtEnter       = 3, ///< parse single extension data.
          ExtEnd         = 4, ///< finish parsing extensions data.
          ObjBegin       = 5, ///< start parsing an entity object.
          ObjEnter       = 6, ///< parse entity object data.
          ComponentEnter = 7, ///< parse component data.
          ObjEnd         = 8, ///< finish parsing an entity object.
     };

     /// @copydoc assets::IAssetVisitor::visit(assets::AssetActionId, const assets::IAssetReader&)
     void visit( assets::AssetActionId action_id, const assets::IAssetReader& reader ) override;

     /// @brief Process unit of load data.
     /// @throws std::runtime_error or format-specific exceptions in case of parsing errors.
     /// @param[in] reader asset reader.
     void process( assets::IAssetReader& reader );

     /// @brief Set control of dependencies parsing.
     /// @param[in] need_dep control dependencies parsing.
     void set_need_dep( bool need_dep ) noexcept;

     /// @brief Set control of extensions parsing.
     /// @param[in] need_ext control extensions parsing.
     void set_need_ext( bool need_ext ) noexcept;

     /// @brief Set control of objects parsing.
     /// @param[in] need_obj control objects parsing.
     void set_need_obj( bool need_obj ) noexcept;

private:
     bool need_dep_{ true };  ///< control of dependencies parsing.
     bool need_ext_{ true };  ///< control of extensions parsing.
     bool need_obj_{ true };  ///< control of objects parsing.
};

} // namespace _16nar::profile

#endif // #ifndef _16NAR_CORE_PROFILE_GENERAL_UNIT_VISITOR_H
