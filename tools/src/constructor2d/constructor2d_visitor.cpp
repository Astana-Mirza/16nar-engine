#include <16nar/tools/constructor2d/constructor2d_visitor.h>

#include <16nar/tools/asset_iterator.h>

namespace _16nar::tools
{

void Constructor2DVisitor::visit( AssetActionId, const IAssetReader& )
{
     // do nothing by default, this function should be overriden in derived classes
}


void Constructor2DVisitor::process_scene( IAssetReader& reader )
{
     AssetIterator iter{ *this, reader };
     iter
     .check_array( false )
     .call( Actions::SceneBegin )
     .go( "states" )
          .call( Actions::StatesBegin )
          .for_each( [ &iter ]()
          {
               iter
               .check_array( false )
               .call( Actions::StateEnter )
               .go( "render" )
                    .call( Actions::RenderSystemEnter )
               .up();
          }, true )
          .call( Actions::StatesEnd )
     .up()
     .call( Actions::SceneEnd );
}

} // namespace _16nar::tools
