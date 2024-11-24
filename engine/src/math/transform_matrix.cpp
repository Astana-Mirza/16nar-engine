#include <16nar/math/transform_matrix.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <array>
#include <algorithm>

namespace _16nar
{

TransformMatrix::TransformMatrix( float x0, float y0, float z0, float w0,
                                  float x1, float y1, float z1, float w1, 
                                  float x2, float y2, float z2, float w2, 
                                  float x3, float y3, float z3, float w3 ) noexcept:
     mat_{ x0, y0, z0, w0,
           x1, y1, z1, w1,
           x2, y2, z2, w2,
           x3, y3, z3, w3 }
{}


const float& TransformMatrix::get( std::size_t i, std::size_t j ) const noexcept
{
     return mat_[ i ][ j ];
}


float& TransformMatrix::get( std::size_t i, std::size_t j ) noexcept
{
     return mat_[ i ][ j ];
}


TransformMatrix& TransformMatrix::operator*=( const TransformMatrix& other ) noexcept
{
     mat_ *= other.mat_;
     return *this;
}


Vec4f TransformMatrix::operator*( const Vec4f& vector ) const noexcept
{
     return Vec4f{ mat_ * vector.vec_ };
}


Vec3f TransformMatrix::operator*( const Vec3f& vector ) const noexcept
{
     auto result = mat_ * glm::vec4{ vector.vec_, 1.0f };
     return Vec3f{ result.x, result.y, result.z };
}


Vec2f TransformMatrix::operator*( const Vec2f& vector ) const noexcept
{
     auto result = mat_ * glm::vec4{ vector.vec_, 0.0f, 1.0f };
     return Vec2f{ result.x, result.y };
}


FloatRect TransformMatrix::operator*( const FloatRect& rect ) const noexcept
{
     Vec2f v1 = ( *this ) * rect.get_pos();
     Vec2f v2 = ( *this ) * Vec2f{ rect.get_pos().x() + rect.get_width(), rect.get_pos().y() };
     Vec2f v3 = ( *this ) * Vec2f{ rect.get_pos().x(), rect.get_pos().y() + rect.get_height() };
     Vec2f v4 = ( *this ) * Vec2f{ rect.get_pos().x() + rect.get_width(), rect.get_pos().y() + rect.get_height() };

     std::array< float, 4 > x{ v1.x(), v2.x(), v3.x(), v4.x() };
     std::array< float, 4 > y{ v1.y(), v2.y(), v3.y(), v4.y() };
     float x_max = *std::max_element( x.cbegin(), x.cend() );
     float x_min = *std::min_element( x.cbegin(), x.cend() );
     float y_max = *std::max_element( y.cbegin(), y.cend() );
     float y_min = *std::min_element( y.cbegin(), y.cend() );

     return FloatRect{ Vec2f{ x_min, y_min }, x_max - x_min, y_max - y_min };
}


TransformMatrix TransformMatrix::affine_inv() const noexcept
{
     return TransformMatrix{ glm::affineInverse( mat_ ) };
}


TransformMatrix TransformMatrix::inv_transpose() const noexcept
{
     return TransformMatrix{ glm::inverseTranspose( mat_ ) };
}


bool TransformMatrix::equals( const TransformMatrix& mat, float precision ) const noexcept
{
     for ( std::size_t i = 0; i < 4; i++ )
     {
          for ( std::size_t j = 0; j < 4; j++ )
          {
               if ( std::fabs( get( i, j ) - mat.get( i, j ) ) > precision )
               {
                    return false;
               }
          }
     }
     return true;
}


// 2D API

TransformMatrix& TransformMatrix::move( const Vec2f& offset ) noexcept
{
     mat_ = glm::translate( mat_, glm::vec3{ offset.vec_, 0.0f } );
     return *this;
}


TransformMatrix& TransformMatrix::scale( const Vec2f& factors ) noexcept
{
     mat_ = glm::scale( mat_, glm::vec3{ factors.vec_, 1.0f } );
     return *this;
}


TransformMatrix& TransformMatrix::scale( const Vec2f& factors, const Vec2f& pivot ) noexcept
{
     mat_ = glm::translate( glm::mat4( 1.0f ), glm::vec3{ pivot.vec_, 0.0f } ) *
            glm::scale( glm::mat4( 1.0f ), glm::vec3{ factors.vec_, 1.0f } ) *
            glm::translate( glm::mat4( 1.0f ), glm::vec3{ -pivot.vec_, 0.0f } ) * mat_;
     return *this;
}


TransformMatrix& TransformMatrix::rotate( float angle ) noexcept
{
     mat_ = glm::rotate( glm::mat4( 1.0f ), angle, glm::vec3{ 0.0f, 0.0f, 1.0f } ) * mat_;
     return *this;
}


TransformMatrix& TransformMatrix::rotate( float angle, const Vec2f& pivot ) noexcept
{
     mat_ = glm::translate( glm::mat4( 1.0f ), glm::vec3{ pivot.vec_, 0.0f } ) *
            glm::rotate( glm::mat4( 1.0f ), angle, glm::vec3{ 0.0f, 0.0f, 1.0f } ) *
            glm::translate( glm::mat4( 1.0f ), glm::vec3{ -pivot.vec_, 0.0f } ) * mat_;
     return *this;
}


// 3D API

TransformMatrix& TransformMatrix::move( const Vec3f& offset ) noexcept
{
     mat_ = glm::translate( mat_, offset.vec_ );
     return *this;
}


TransformMatrix& TransformMatrix::scale( const Vec3f& factors ) noexcept
{
     mat_ = glm::scale( mat_, factors.vec_ );
     return *this;
}


TransformMatrix& TransformMatrix::scale( const Vec3f& factors, const Vec3f& pivot ) noexcept
{
     mat_ = glm::translate( glm::mat4( 1.0f ), pivot.vec_ ) *
            glm::scale( glm::mat4( 1.0f ), factors.vec_ ) *
            glm::translate( glm::mat4( 1.0f ), -pivot.vec_ ) * mat_;
     return *this;
}


TransformMatrix& TransformMatrix::rotate( float angle, const Vec3f& axis ) noexcept
{
     mat_ = glm::rotate( glm::mat4( 1.0f ), angle, axis.vec_ ) * mat_;
     return *this;
}


TransformMatrix& TransformMatrix::rotate( float angle, const Vec3f& axis, const Vec3f& pivot ) noexcept
{
     mat_ = glm::translate( glm::mat4( 1.0f ), pivot.vec_ ) *
            glm::rotate( glm::mat4( 1.0f ), angle, axis.vec_ ) *
            glm::translate( glm::mat4( 1.0f ), -pivot.vec_ ) * mat_;
     return *this;
}


// Operators

TransformMatrix operator*( const TransformMatrix& lhs, const TransformMatrix& rhs ) noexcept
{
     TransformMatrix tmp = lhs;
     return tmp *= rhs;
}


bool operator==( const TransformMatrix& lhs, const TransformMatrix& rhs ) noexcept
{
     for ( std::size_t i = 0; i < 4; i++ )
     {
          for ( std::size_t j = 0; j < 4; j++ )
          {
               if ( lhs.get( i, j ) != rhs.get( i, j ) )
               {
                    return false;
               }
          }
     }
     return true;
}


bool operator!=( const TransformMatrix& lhs, const TransformMatrix& rhs ) noexcept
{
     return !( lhs == rhs );
}

} // namespace _16nar
