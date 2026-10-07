#include <16nar/core/profile/general/unit_visitor.h>

#include <16nar/core/assets/asset_iterator.h>

namespace _16nar::profile
{

void UnitVisitor::visit( assets::AssetActionId action_id, const assets::IAssetReader& reader )
{
     // do nothing by default, this function should be overriden in derived classes
}


void UnitVisitor::process( assets::IAssetReader& reader )
{
     assets::AssetIterator iter{ *this, reader };
     iter.check_array( false );
     if ( need_dep_ && iter.go_opt( "dep" ) )
     {
          iter
          .call( Actions::DepEnter )
          .up();
     }
     if ( need_ext_ && iter.go_opt( "ext" ) )
     {
          iter
          .check_array( true )
          .call( Actions::ExtBegin )
          .for_each( [ &iter ]()
          {
               iter.call( Actions::ExtEnter );
          }, true )
          .call( Actions::ExtEnd )
          .up();
     }
     if ( need_obj_ && iter.go_opt( "obj" ) )
     {
          iter
          .check_array( true )
          .call( Actions::ObjBegin )
          .for_each( [ &iter ]()
          {
               iter
               .check_array( true )
               .call( Actions::ObjEnter )
               .for_each( [ &iter ]()
               {
                    iter.call( Actions::ComponentEnter );
               }, true )
               .call( Actions::ObjEnd );
          }, true )
          .up();
     }
}


void UnitVisitor::set_need_dep( bool need_dep ) noexcept
{
     need_dep_ = need_dep;
}


void UnitVisitor::set_need_ext( bool need_ext ) noexcept
{
     need_ext_ = need_ext;
}


void UnitVisitor::set_need_obj( bool need_obj ) noexcept
{
     need_obj_ = need_obj;
}

} // namespace _16nar::profile
