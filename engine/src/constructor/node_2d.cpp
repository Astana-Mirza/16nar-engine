#include <16nar/constructor/node_2d.h>

namespace _16nar
{

#ifdef USE_SFML

const Vector2f& Node2D::get_position() const
{
     return transform_.getPosition();
}


float Node2D::get_rotation() const
{
     return transform_.getRotation();
}


const Vector2f& Node2D::get_scale() const
{
     return transform_.getScale();
}


const Vector2f& Node2D::get_origin() const
{
     return transform_.getOrigin();
}


const TransformMatrix& Node2D::get_transform_matr() const
{
     return transform_.getTransform();
}


const TransformMatrix& Node2D::get_inv_transform_matr() const
{
     return transform_.getInverseTransform();
}


void Node2D::set_position( float x, float y )
{
     transform_.setPosition( x, y );
     transformed_ = true;
}


void Node2D::set_position( const Vector2f& position )
{
     transform_.setPosition( position );
     transformed_ = true;
}


void Node2D::set_rotation( float angle )
{
     transform_.setRotation( angle );
     transformed_ = true;
}


void Node2D::set_scale( float factor_x, float factor_y )
{
     transform_.setScale( factor_x, factor_y );
     transformed_ = true;
}


void Node2D::set_scale( const Vector2f& factors )
{
     transform_.setScale( factors );
     transformed_ = true;
}


void Node2D::set_origin( float x, float y )
{
     transform_.setOrigin( x, y );
     transformed_ = true;
}


void Node2D::set_origin( const Vector2f& origin )
{
     transform_.setOrigin( origin );
     transformed_ = true;
}


void Node2D::move( float offset_x, float offset_y )
{
     transform_.move( offset_x, offset_y );
     transformed_ = true;
}


void Node2D::move( const Vector2f& offset )
{
     transform_.move( offset );
     transformed_ = true;
}


void Node2D::rotate( float angle )
{
     transform_.rotate( angle );
     transformed_ = true;
}


void Node2D::scale( float factor_x, float factor_y )
{
     transform_.scale( factor_x, factor_y );
     transformed_ = true;
}


void Node2D::scale( const Vector2f& factor )
{
     transform_.scale( factor );
     transformed_ = true;
}

#endif // #ifdef USE_SFML

} // namespace _16nar
