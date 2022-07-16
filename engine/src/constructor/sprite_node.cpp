#include <constructor/sprite_node.h>

namespace _16nar
{

#ifdef RENDER_SFML

SpriteNode::SpriteNode( Quadrant *quad, const Texture& tex ):
     DrawableNode::DrawableNode( quad ), sprite_( tex ) {}


SpriteNode::SpriteNode( Quadrant *quad, const Texture& tex, const IntRect& rect ):
     DrawableNode::DrawableNode( quad ), sprite_( tex, rect ) {}


void SpriteNode::draw( RenderTarget& target, RenderStates states ) const
{
     target.draw( sprite_, states );
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


bool SpriteNode::check_quadrant( const Quadrant *quad ) const
{
     auto transform = get_global_transform_matr();
     auto rect = transform.transformRect( sprite_.getLocalBounds() );
     if ( quad->get_area().contains( rect.left, rect.top ) &&
          quad->get_area().contains( rect.left + rect.width - 1, rect.top + rect.height - 1 ) )
     {
          return true;
     }
     return false;
}

#endif // #ifdef RENDER_SFML

} // namespace _16nar
