#include <16nar/constructor/tiles/tile.h>

#include <16nar/constructor/tilemap_node.h>

namespace _16nar
{

Tile::Tile( Quadrant* quad, TilemapNode& tilemap, size_t vertex_count ):
     Drawable::Drawable( quad ), tilemap_{ tilemap },
     vertices_{ PrimitiveType::TriangleStrip, vertex_count } {}


Tile::Tile( const Tile& other ):
     Drawable::Drawable( other.quad_ ),
     tilemap_{ other.tilemap_ },
     vertices_{ other.vertices_ } {}


size_t Tile::get_vertex_count() const
{
     return vertices_.getVertexCount();
}


Vertex& Tile::get_vertex( size_t index )
{
     return vertices_[ index ];
}


void Tile::set_primitive_type( PrimitiveType type )
{
     vertices_.setPrimitiveType( type );
}


PrimitiveType Tile::get_primitive_type() const
{
     return vertices_.getPrimitiveType();
}


bool Tile::is_visible() const
{
     return tilemap_.get_settings().visible;
}


void Tile::set_visible( bool visible )
{
     tilemap_.get_settings().visible = visible;
}


int Tile::get_layer() const
{
     return tilemap_.get_settings().layer;
}


void Tile::set_layer( int layer )
{
     tilemap_.get_settings().layer = layer;
}


void Tile::set_shader( Shader* shader )
{
     tilemap_.get_settings().shader = shader;
}


Shader *Tile::get_shader() const
{
     return tilemap_.get_settings().shader;
}


void Tile::set_blend( const BlendMode& blend )
{
     tilemap_.get_settings().blend = blend;
}


const BlendMode& Tile::get_blend() const
{
     return tilemap_.get_settings().blend;
}


FloatRect Tile::get_local_bounds() const
{
     return vertices_.getBounds();
}


void Tile::draw( RenderTarget& target ) const
{
     RenderStates states{ get_blend(), tilemap_.get_global_transform_matr(),
                          tilemap_.get_settings().texture, get_shader() };
     target.draw( vertices_, states );
}


void Tile::set_color( const Color& color )
{
     tilemap_.get_settings().color = color;
}


const Color& Tile::get_color() const
{
     return tilemap_.get_settings().color;
}


bool Tile::check_quadrant( const Quadrant* quad ) const
{
     auto transform = tilemap_.get_global_transform_matr();
     auto rect = transform.transformRect( get_local_bounds() );
     if ( quad->get_area().contains( rect.left, rect.top ) &&
          quad->get_area().contains( rect.left + rect.width - 1, rect.top + rect.height - 1 ) )
     {
          return true;
     }
     return false;
}

} // namespace _16nar
