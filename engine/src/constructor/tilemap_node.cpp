#include <constructor/tilemap_node.h>

namespace _16nar
{

Tile& TilemapNode::make_tile(Quadrant *quad, size_t vertex_count)
{
     return tiles_.emplace_back( quad, *this, vertex_count );
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
               tile.fix_quadrant();
          }
     }
     for ( auto child : get_children() )
     {
          child->loop_call( update, delta );
     }
}

} // namespace _16nar