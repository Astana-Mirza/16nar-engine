/// @file
/// @brief File with AssetIterator class definition.
#ifndef _16NAR_CORE_ASSETS_ASSET_ITERATOR_H
#define _16NAR_CORE_ASSETS_ASSET_ITERATOR_H

#include <16nar/core/assets/defs.h>
#include <16nar/core/assets/iasset_visitor.h>

#include <functional>
#include <stdexcept>

namespace _16nar::assets
{

/// @brief Class for iterating asset tree.
class NARENGINE_CORE_API AssetIterator
{
public:
     /// @brief Constructor.
     /// @param[in] visitor asset visitor.
     /// @param[in] reader asset reader.
     AssetIterator( IAssetVisitor& visitor, IAssetReader& reader );

     /// @brief Get asset reader.
     /// @return asset reader.
     IAssetReader& get_reader();

     /// @brief Check whether current asset is array or not.
     /// @throws std::runtime_error if the check fails.
     /// @param[in] is_array true if checking for array, false if checking for set.
     /// @return current object.
     AssetIterator& check_array( bool is_array );

     /// @brief Call visitor with given action.
     /// @param[in] action_id identifier of the action with wchich the visitor will be called.
     /// @return current object.
     AssetIterator& call( AssetActionId action_id );

     /// @brief Go to child asset.
     /// @throws std::runtime_error if the given child does not exist.
     /// @param[in] name name of the child asset.
     /// @return current object.
     AssetIterator& go( std::string_view name );

     /// @brief Go to child asset if it exists.
     /// @param[in] name name of the child asset.
     /// @return true if successfully switched to child asset, false otherwise.
     bool go_opt( std::string_view name );

     /// @brief Go to parent assset.
     /// @throws std::runtime_error if the parent does not exist.
     /// @return current object.
     AssetIterator& up();

     /// @brief Call function object for each child, if current asset is an array.
     /// @details The called function must leave the iterator on
     /// the same asset on which it was called.
     /// @throws std::runtime_error if the children array must be non-empty and is empty.
     /// @tparam Call type of called function object.
     /// @param[in] func function to be called for each child.
     /// @param[in] mandatory true if the children array must be non-empty, false otherwise.
     /// @return current object.
     template < typename Call >
     AssetIterator& for_each( Call&& func, bool mandatory = false )
     {
          const auto count = reader_.get_children_count();
          if ( mandatory && !count )
          {
               throw std::runtime_error{ "children array is empty" };
          }
          for ( std::size_t i = 0; i < count; ++i )
          {
               reader_.to_child_index( i );
               std::invoke( std::forward< Call >( func ) );
               reader_.to_parent();
          }
          return *this;
     }

private:
     IAssetVisitor& visitor_; ///< asset visitor.
     IAssetReader& reader_;   ///< asset reader.
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_ASSET_ITERATOR_H
