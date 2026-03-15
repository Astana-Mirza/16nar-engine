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
     .go( "resources" )
          .call( Actions::ResourcesBegin )
          .for_each( [ &iter ]()
          {
               // optionally, asset metadata may be stored directly in this array,
               // but it can also be a regular resource within the scene
               // also, some formats (JSON) are able to store it only here
               iter.call( Actions::ResourceEnter );
          }, true )
          .call( Actions::ResourcesEnd )
     .up()
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
