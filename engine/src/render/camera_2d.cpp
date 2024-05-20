#include <16nar/render/camera_2d.h>

namespace _16nar
{

Camera2D::Camera2D( const FloatRect& view_rect ): view_rect_{ view_rect } {}


FloatRect Camera2D::get_global_bounds() const
{
     return matr_ * view_rect_;
}

void View::set_center( const Vector2f pos )
{
     view_.setCenter( pos );
}


void View::set_center( float x, float y )
{
     view_.setCenter( x, y );
}


void View::set_size( float width, float height )
{
     view_.setSize( width, height );
}


void View::set_size( const Vector2f& size )
{
     view_.setSize( size );
}


void View::set_rotation( float angle )
{
     view_.setRotation( angle );
}


void View::set_viewport( const FloatRect& viewport )
{
     view_.setViewport( viewport );
}


void View::reset( const FloatRect& rectangle )
{
     view_.reset( rectangle );
}


const Vector2f& View::get_center() const
{
     return view_.getCenter();
}


const Vector2f& View::get_size() const
{
     return view_.getSize();
}


float View::get_rotation() const
{
     return view_.getRotation();
}


const FloatRect& View::get_viewport() const
{
     return view_.getViewport();
}


void View::move( float offset_x, float offset_y )
{
     view_.move( offset_x, offset_y );
}


void View::move( const Vector2f& offset )
{
     view_.move( offset );
}


void View::rotate( float angle )
{
     view_.rotate( angle );
}


void View::zoom( float factor )
{
     view_.zoom( factor );
}


const TransformMatrix& View::get_transform_matr() const
{
     return view_.getTransform();
}


const TransformMatrix& View::get_inverse_transform_matr() const
{
     return view_.getInverseTransform();
}

} // namespace _16nar
