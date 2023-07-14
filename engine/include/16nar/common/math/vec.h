/// @file Header file with Vec template class declaration.
#ifndef _16NAR_VEC_H
#define _16NAR_VEC_H

#include <16nar/common/16nardefs.h>

#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
#    include <glm/vec2.hpp>
#    include <glm/vec3.hpp>
#    include <glm/vec4.hpp>
#    include <glm/geometric.hpp>
#endif

#include <type_traits>
#include <utility>

#define _16NAR_ENABLE_IF( expr ) template < typename = typename std::enable_if_t< ( expr ) > >

namespace _16nar
{

/// @brief Geometric vector.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vector's elements.
template < std::size_t N, typename T >
class Vec
{
public:
     static_assert( N > 1 && N <= 4, "Vector size must be 2, 3 or 4" );
     friend class TransformMatrix;

     /// @brief Vector size (number of dimensions).
     static constexpr std::size_t size = N;

     /// @brief Type of vector's elements.
     using type = T;

     /// @brief Default constructor.
     Vec():
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          vec_{}
#endif
     {}

     /// @brief Copy constructor.
     /// @param[in] other vector to be copied
     Vec( const Vec& other ):
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          vec_{ other.vec_ }
#endif
     {}

     /// @brief Constructor for 2-dimensional vector.
     /// @param[in] v0 first element.
     /// @param[in] v1 second element.
     _16NAR_ENABLE_IF( N == 2 )
     explicit Vec( T v0, T v1 ):
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          vec_{ v0, v1 }
#endif
     {}

     /// @brief Constructor for 3-dimensional vector.
     /// @param[in] v0 first element.
     /// @param[in] v1 second element.
     /// @param[in] v2 third element.
     _16NAR_ENABLE_IF( N == 3 )
     explicit Vec( T v0, T v1, T v2 ):
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          vec_{ v0, v1, v2 }
#endif
     {}

     /// @brief Constructor for 4-dimensional vector.
     /// @param[in] v0 first element.
     /// @param[in] v1 second element.
     /// @param[in] v2 third element.
     /// @param[in] v3 fourth element.
     _16NAR_ENABLE_IF( N == 4 )
     explicit Vec( T v0, T v1, T v2, T v3 ):
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          vec_{ v0, v1, v2, v3 }
#endif
     {}

     /// @brief Copy assignment operator.
     /// @param[in] other vector to be copied.
     inline Vec& operator=( const Vec& other )
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          vec_ = other.vec_;
#endif
          return *this;
     }

     /// @brief Assignment add operator.
     /// @param[in] other vector to be added.
     inline Vec& operator+=( const Vec& other )
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          vec_ += other.vec_;
#endif
          return *this;
     }

     /// @brief Assignment subtract operator.
     /// @param[in] other vector to be subtracted.
     inline Vec& operator-=( const Vec& other )
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          vec_ -= other.vec_;
#endif
          return *this;
     }

     /// @brief Assignment multiplication operator with scalar.
     /// @param[in] num scalar value.
     inline Vec& operator*=( T num )
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          vec_ *= num;
#endif
          return *this;
     }

     /// @brief Negate operator.
     inline Vec operator-() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return Vec{ -vec_ };
#endif
     }

     /// @brief Subscription operator.
     /// @param[in] i index of element to get.
     inline T& operator[]( std::size_t i )
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_[ i ];
#endif
     }

     /// @brief Subscription operator (const).
     /// @param[in] i index of element to get.
     inline const T& operator[]( std::size_t i ) const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_[ i ];
#endif
     }

     /// @brief Get element x.
     inline T& x()
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_.x;
#endif
     }
     /// @brief Get element x (const).
     inline const T& x() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_.x;
#endif
     }
     /// @brief Get element y.
     inline T& y()
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_.y;
#endif
     }
     /// @brief Get element y (const).
     inline const T& y() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_.y;
#endif
     }
     /// @brief Get element z.
     _16NAR_ENABLE_IF( N > 2 ) inline T& z()
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_.z;
#endif
     }
     /// @brief Get element z (const).
     _16NAR_ENABLE_IF( N > 2 ) inline const T& z() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_.z;
#endif
     }
     /// @brief Get element w.
     _16NAR_ENABLE_IF( N > 3 ) inline T& w()
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_.w;
#endif
     }
     /// @brief Get element w (const).
     _16NAR_ENABLE_IF( N > 3 ) inline const T& w() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return vec_.w;
#endif
     }

     /// @brief Get underlying matrix data.
     inline const T *data() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return &vec_[ 0 ];
#endif
     }

     /// @brief Get length of the vector.
     _16NAR_ENABLE_IF( std::is_floating_point< T >::value )
     inline T length() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return glm::length( vec_ );
#endif
     }

     /// @brief Normalize a vector.
     inline Vec normalize() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return Vec{ glm::normalize( vec_ ) };
#endif
     }

     /// @brief Check equality of vectors with given precision.
     /// @param[in] vec vector to check equality.
     /// @param[in] precision precision to compare with.
     bool equals( const Vec& vec, float precision ) const;

     /// @brief Find distance to given vector.
     /// @param[in] vec vector to calculate distance.
     inline T distance( const Vec& vec ) const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return glm::distance( vec_, vec.vec_ );
#endif
     }

     /// @brief Find dot product of vectors.
     /// @param[in] vec vector.
     inline T dot( const Vec& vec ) const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return glm::dot( vec_, vec.vec_ );
#endif
     }

     /// @brief Find cross product of vectors.
     /// @param[in] vec first vector.
     _16NAR_ENABLE_IF( N == 3 ) inline Vec cross( const Vec& vec ) const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return Vec{ glm::cross( vec_, vec.vec_ ) };
#endif
     }

     /// @brief Orient a vector to point away from the plane defined by two vectors.
     /// @param[in] incident vector of sight directed to the plane.
     /// @param[in] normal normal vector of the plane.
     /// @return self if dot( normal, incident ) < 0.0, -self otherwise.
     inline Vec faceforward( const Vec& incident, const Vec& normal ) const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return Vec{ glm::faceforward( vec_, incident.vec_, normal.vec_ ) };
#endif
     }

     /// @brief Reflect vector from a plane defined by its normal.
     /// @param[in] normal normal of the plane (must be normalized).
     /// @return self - 2 * dot( normal, self ) * normal.
     inline Vec reflect( const Vec& normal ) const
     { 
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return Vec{ glm::reflect( vec_, normal.vec_ ) };
#endif
     }

     /// @brief Refract vector.
     /// @param[in] normal normal to the plane of refraction (must be normalized).
     /// @param[in] eta ratio of indices of refraction (n1 / n2).
     _16NAR_ENABLE_IF( std::is_floating_point< T >::value )
     inline Vec refract( const Vec& normal, T eta ) const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return Vec{ glm::refract( vec_, normal.vec_, eta ) };
#endif
     }

private:
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
     /// @brief Constructor with inner vector representation.
     /// @detail For inner use.
     /// @param[in] vec inner vector representation.
     explicit Vec( const glm::vec< N, T, glm::qualifier::defaultp >& vec ) : vec_{ vec } {}

     glm::vec< N, T, glm::qualifier::defaultp > vec_;  ///< inner vector representation.
#endif
}; // class Vec


/// @brief Comprasion operator for two vectors.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vectors' elements.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
template < std::size_t N, typename T >
inline bool operator==( const Vec< N, T >& lhs, const Vec< N, T >& rhs );


/// @brief Inverse comprasion operator for two vectors.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vectors' elements.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
template < std::size_t N, typename T >
inline bool operator!=( const Vec< N, T >& lhs, const Vec< N, T >& rhs );


/// @brief Add operator.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vectors' elements.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
template < std::size_t N, typename T >
inline Vec< N, T > operator+( const Vec< N, T >& lhs, const Vec< N, T >& rhs );


/// @brief Subtract operator.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vectors' elements.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
template < std::size_t N, typename T >
inline Vec< N, T > operator-( const Vec< N, T >& lhs, const Vec< N, T >& rhs );


/// @brief Multiplication by a scalar operator.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vector's elements and scalar.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
template < std::size_t N, typename T >
inline Vec< N, T > operator*( const Vec< N, T >& lhs, T rhs );


/// @brief Multiplication by a scalar operator.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vector's elements and scalar.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
template < std::size_t N, typename T >
inline Vec< N, T > operator*( T lhs, const Vec< N, T >& rhs );


using Vec2f = Vec< 2, float >;
using Vec3f = Vec< 3, float >;
using Vec4f = Vec< 4, float >;

using Vec2i = Vec< 2, int >;
using Vec3i = Vec< 3, int >;
using Vec4i = Vec< 4, int >;

} // namespace _16nar

#include "vec.inl"

#endif // #ifndef _16NAR_VEC_H
