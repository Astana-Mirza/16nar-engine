#include <16nar/constructor2d/transformable_2d.h>

#include <16nar/math/math_functions.h>

#include <cmath>

namespace _16nar::constructor2d
{

Transformable2D::Transformable2D() noexcept:
     matr_{}, position_{}, origin_{}, scale_{ 1.0f, 1.0f },
     rotation_{ 0.0f }, transformed_{ false }
{}


bool Transformable2D::calculate_matr() noexcept
{
     if ( transformed_ )
     {
          matr_ = TransformMatrix{};
          matr_.move( position_ ).rotate( deg2rad( rotation_ ) ).scale( scale_ );
          transformed_ = false;
          return true;
     }
     return false;
}


const Vec2f& Transformable2D::get_position() const noexcept
{
     return position_;
}


float Transformable2D::get_rotation() const noexcept
{
     return rotation_;
}


const Vec2f& Transformable2D::get_scale() const noexcept
{
     return scale_;
}


const Vec2f& Transformable2D::get_origin() const noexcept
{
     return origin_;
}


const TransformMatrix& Transformable2D::get_transform_matr() const noexcept
{
     return matr_;
}


TransformMatrix Transformable2D::get_inv_transform_matr() const noexcept
{
     return matr_.inv();
}


void Transformable2D::set_position( float x, float y ) noexcept
{
     position_ = Vec2f{ x, y };
     transformed_ = true;
}


void Transformable2D::set_position( const Vec2f& position ) noexcept
{
     position_ = position;
     transformed_ = true;
}


void Transformable2D::set_rotation( float angle ) noexcept
{
     rotation_ = angle >= 0.0f ? std::fmod( angle, 360.0f ) : ( 360.0f - std::fmod( -angle, 360.0f ) );
     transformed_ = true;
}


void Transformable2D::set_scale( float factor_x, float factor_y ) noexcept
{
     scale_ = Vec2f{ factor_x, factor_y };
     transformed_ = true;
}


void Transformable2D::set_scale( const Vec2f& factors ) noexcept
{
     scale_ = factors;
     transformed_ = true;
}


void Transformable2D::set_origin( float x, float y ) noexcept
{
     origin_ = Vec2f{ x, y };
     transformed_ = true;
}


void Transformable2D::set_origin( const Vec2f& origin ) noexcept
{
     origin_ = origin;
     transformed_ = true;
}


void Transformable2D::move( float offset_x, float offset_y ) noexcept
{
     position_ += Vec2f{ offset_x, offset_y };
     transformed_ = true;
}


void Transformable2D::move( const Vec2f& offset ) noexcept
{
     position_ += offset;
     transformed_ = true;
}


void Transformable2D::rotate( float angle ) noexcept
{
     set_rotation( rotation_ + angle );
}


void Transformable2D::scale( float factor_x, float factor_y ) noexcept
{
     scale_.x() *= factor_x;
     scale_.y() *= factor_y;
     transformed_ = true;
}


void Transformable2D::scale( const Vec2f& factor ) noexcept
{
     scale_.x() *= factor.x();
     scale_.y() *= factor.y();
     transformed_ = true;
}

} // namespace _16nar::constructor2d
