#include <constructor/sprite_node.h>

namespace _16nar
{

#ifdef USE_SFML

SpriteNode::SpriteNode( Quadrant *quad, const Texture& tex ):
     DrawableNode::DrawableNode( quad ), sprite_( tex ) {}


SpriteNode::SpriteNode( Quadrant *quad, const Texture& tex, const IntRect& rect ):
     DrawableNode::DrawableNode( quad ), sprite_( tex, rect ) {}


void SpriteNode::draw( RenderTarget& target ) const
{
     target.draw( sprite_, get_global_transform_matr( false ) );
}


void SpriteNode::set_texture( const Texture& texture, bool reset_rect )
{
     sprite_.setTexture( texture, reset_rect );
}


void SpriteNode::set_texture_rect( const IntRect& rect )
{
     sprite_.setTextureRect( rect );
}


void SpriteNode::set_color( const Color& color )
{
     sprite_.setColor( color );
}


const Color& SpriteNode::get_color() const
{
     return sprite_.getColor();
}


FloatRect SpriteNode::get_local_bounds() const
{
     return sprite_.getLocalBounds();
}


const Texture *SpriteNode::get_texture() const
{
     return sprite_.getTexture();
}


const IntRect& SpriteNode::get_texture_rect() const
{
     return sprite_.getTextureRect();
}


const Vector2f& SpriteNode::get_position() const
{
     return sprite_.getPosition();
}


float SpriteNode::get_rotation() const
{
     return sprite_.getRotation();
}


const Vector2f& SpriteNode::get_scale() const
{
     return sprite_.getScale();
}


const Vector2f& SpriteNode::get_origin() const
{
     return sprite_.getOrigin();
}


const TransformMatrix& SpriteNode::get_transform_matr() const
{
     return sprite_.getTransform();
}


const TransformMatrix& SpriteNode::get_inv_transform_matr() const
{
     return sprite_.getInverseTransform();
}


void SpriteNode::set_position( float x, float y )
{
     sprite_.setPosition( x, y );
     transformed_ = true;
}


void SpriteNode::set_position( const Vector2f& position )
{
     sprite_.setPosition( position );
     transformed_ = true;
}


void SpriteNode::set_rotation( float angle )
{
     sprite_.setRotation( angle );
     transformed_ = true;
}


void SpriteNode::set_scale( float factor_x, float factor_y )
{
     sprite_.setScale( factor_x, factor_y );
     transformed_ = true;
}


void SpriteNode::set_scale( const Vector2f& factors )
{
     sprite_.setScale( factors );
     transformed_ = true;
}


void SpriteNode::set_origin( float x, float y )
{
     sprite_.setOrigin( x, y );
     transformed_ = true;
}


void SpriteNode::set_origin( const Vector2f& origin )
{
     sprite_.setOrigin( origin );
     transformed_ = true;
}


void SpriteNode::move( float offset_x, float offset_y )
{
     sprite_.move( offset_x, offset_y );
     transformed_ = true;
}


void SpriteNode::move( const Vector2f& offset )
{
     sprite_.move( offset );
     transformed_ = true;
}


void SpriteNode::rotate( float angle )
{
     sprite_.rotate( angle );
     transformed_ = true;
}


void SpriteNode::scale( float factor_x, float factor_y )
{
     sprite_.scale( factor_x, factor_y );
     transformed_ = true;
}


void SpriteNode::scale( const Vector2f& factor )
{
     sprite_.scale( factor );
     transformed_ = true;
}

#endif // #ifdef USE_SFML

} // namespace _16nar
