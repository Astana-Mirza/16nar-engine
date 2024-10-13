#include <16nar/constructor/tilemap_node.h>

namespace _16nar
{

Tile& TilemapNode::make_tile( RenderSystem *render_system, size_t vertex_count )
{
     return tiles_.emplace_front( render_system, *this, vertex_count );
}


TilemapNode::TileDrawSettings& TilemapNode::get_settings()
{
     return settings_;
}


void TilemapNode::loop_call( bool update, float delta )
{
     loop( delta );
     for ( auto& [name, anim] : animations_ )
     {
          anim->advance( delta );
     }
     update = update || transformed_;
     transformed_ = false;
     if ( update )
     {
          for ( Tile& tile : tiles_ )
          {
               tile.get_render_system().handle_change( &tile );
          }
     }
     for ( auto child : get_children() )
     {
          child->loop_call( update, delta );
     }
}

} // namespace _16nar
