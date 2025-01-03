#include <16nar/render/camera_2d.h>

#include <16nar/math/math_functions.h>
#include <cmath>

namespace _16nar
{

Camera2D::Camera2D( const Vec2f& center, float width, float height ) noexcept:
     matr_{}, global_bounds_{ Vec2f{ center.x() - width / 2, center.y() - height / 2 }, width, height},
     center_{ center }, half_width_{ width / 2 }, half_height_{ height / 2 }, scale_{ 1.0f },
     rotation_{ 0.0f }
{}


const FloatRect& Camera2D::get_global_bounds() const noexcept
{
     return global_bounds_;
}


void Camera2D::set_center( const Vec2f& pos ) noexcept
{
     center_ = pos;
     params_calculate();
}


void Camera2D::set_size( float width, float height ) noexcept
{
     half_width_ = width / 2;
     half_height_ = height / 2;
     params_calculate();
}


void Camera2D::set_rotation( float angle ) noexcept
{
     rotation_ = angle >= 0.0f ? std::fmod( angle, 360.0f ) : ( 360.0f - std::fmod( -angle, 360.0f ) );
     params_calculate();
}


void Camera2D::set_zoom( float zoom ) noexcept
{
     if ( zoom > 0.0f )
     {
          scale_ = 1.0f / zoom;
          params_calculate();
     }
}


void Camera2D::reset( const Vec2f& center, float width, float height ) noexcept
{
     matr_ = {};
     half_width_ = width / 2;
     half_height_ = height / 2;
     center_ = center;
     rotation_ = 0.0f;
     scale_ = 1.0f;
}


const Vec2f& Camera2D::get_center() const noexcept
{
     return center_;
}


Vec2f Camera2D::get_size() const noexcept
{
     return Vec2f{ half_width_ * 2.0f, half_height_ * 2.0f };
}


float Camera2D::get_rotation() const noexcept
{
     return rotation_;
}


float Camera2D::get_zoom() const noexcept
{
     return 1.0f / scale_;
}


void Camera2D::move( const Vec2f& offset ) noexcept
{
     set_center( center_ + offset );
}


void Camera2D::rotate( float angle ) noexcept
{
     set_rotation( rotation_ + angle );
}


void Camera2D::zoom( float factor ) noexcept
{
     if ( factor > 0.0f )
     {
          scale_ /= factor;
     }
     params_calculate();
}


const TransformMatrix& Camera2D::get_transform_matr() const noexcept
{
     return matr_;
}


TransformMatrix Camera2D::get_inverse_transform_matr() const noexcept
{
     return get_transform_matr().affine_inv();
}


void Camera2D::params_calculate() noexcept
{
     matr_ = {};
     matr_.move( -center_ )
          .rotate( deg2rad( rotation_ ) )
          .scale( Vec2f{ scale_, scale_ } );

     TransformMatrix tmp;
     tmp.rotate( deg2rad( rotation_ ) )
          .scale( Vec2f{ scale_, scale_ } );
     auto tmp2 = TransformMatrix{};
     tmp2.move( center_ );
     tmp2 *= tmp;

     global_bounds_ = tmp2 * FloatRect{
          Vec2f{ -half_width_, -half_height_ },
          half_width_ * 2,
          half_height_ * 2
     };
}

} // namespace _16nar
