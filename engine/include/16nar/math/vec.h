/// @file Header file with Vec template class declaration.
#ifndef _16NAR_VEC_H
#define _16NAR_VEC_H

#include <16nar/16nardefs.h>

#define GLM_FORCE_INLINE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_SIZE_T_LENGTH

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>

#include <type_traits>
#include <utility>

#define _16NAR_ENABLE_IF( expr ) template < bool _B = true, typename = typename std::enable_if_t< ( expr ) && _B > >

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
     Vec() noexcept: vec_{ T{} } {}

     /// @brief Constructor for 2-dimensional vector.
     /// @param[in] v0 first element.
     /// @param[in] v1 second element.
     _16NAR_ENABLE_IF( N == 2 )
     Vec( T v0, T v1 ) noexcept:
          vec_{ v0, v1 }
     {}

     /// @brief Constructor for 3-dimensional vector.
     /// @param[in] v0 first element.
     /// @param[in] v1 second element.
     /// @param[in] v2 third element.
     _16NAR_ENABLE_IF( N == 3 )
     Vec( T v0, T v1, T v2 ) noexcept:
          vec_{ v0, v1, v2 }
     {}

     /// @brief Constructor for 4-dimensional vector.
     /// @param[in] v0 first element.
     /// @param[in] v1 second element.
     /// @param[in] v2 third element.
     /// @param[in] v3 fourth element.
     _16NAR_ENABLE_IF( N == 4 )
     Vec( T v0, T v1, T v2, T v3 ) noexcept:
          vec_{ v0, v1, v2, v3 }
     {}

     /// @brief Assignment add operator.
     /// @param[in] other vector to be added.
     /// @return reference to current object.
     inline Vec& operator+=( const Vec& other ) noexcept
     {
          vec_ += other.vec_;
          return *this;
     }

     /// @brief Assignment subtract operator.
     /// @param[in] other vector to be subtracted.
     /// @return reference to current object.
     inline Vec& operator-=( const Vec& other ) noexcept
     {
          vec_ -= other.vec_;
          return *this;
     }

     /// @brief Assignment multiplication operator with scalar.
     /// @param[in] num scalar value.
     /// @return reference to current object.
     inline Vec& operator*=( T num ) noexcept
     {
          vec_ *= num;
          return *this;
     }

     /// @brief Negate operator.
     /// @return negated vector.
     inline Vec operator-() const noexcept
     {
          return Vec{ -vec_ };
     }

     /// @brief Subscription operator.
     /// @param[in] i index of element to get.
     /// @return i-th element.
     inline T& operator[]( std::size_t i ) noexcept
     {
          return vec_[ static_cast< typename glm::vec< N, T, glm::qualifier::defaultp >::length_type >( i ) ];
     }

     /// @brief Subscription operator (const).
     /// @param[in] i index of element to get.
     /// @return i-th element.
     inline const T& operator[]( std::size_t i ) const noexcept
     {
          return vec_[ static_cast< typename glm::vec< N, T, glm::qualifier::defaultp >::length_type >( i ) ];
     }

     /// @brief Get element x.
     /// @return element x.
     inline T& x() noexcept
     {
          return vec_.x;
     }

     /// @brief Get element x (const).
     /// @return element x.
     inline const T& x() const noexcept
     {
          return vec_.x;
     }

     /// @brief Get element y.
     /// @return element y.
     inline T& y() noexcept
     {
          return vec_.y;
     }

     /// @brief Get element y (const).
     /// @return element y.
     inline const T& y() const noexcept
     {
          return vec_.y;
     }

     /// @brief Get element z.
     /// @return element z.
     _16NAR_ENABLE_IF( N > 2 ) inline T& z() noexcept
     {
          return vec_.z;
     }

     /// @brief Get element z (const).
     /// @return element z.
     _16NAR_ENABLE_IF( N > 2 ) inline const T& z() const noexcept
     {
          return vec_.z;
     }

     /// @brief Get element w.
     /// @return element w.
     _16NAR_ENABLE_IF( N > 3 ) inline T& w() noexcept
     {
          return vec_.w;
     }

     /// @brief Get element w (const).
     /// @return element w.
     _16NAR_ENABLE_IF( N > 3 ) inline const T& w() const noexcept
     {
          return vec_.w;
     }

     /// @brief Get underlying matrix data.
     /// @return underlying matrix data.
     inline const T *data() const noexcept
     {
          return &vec_[ 0 ];
     }

     /// @brief Get length of the vector.
     /// @return length of the vector.
     _16NAR_ENABLE_IF( std::is_floating_point< T >::value )
     inline T length() const noexcept
     {
          return glm::length( vec_ );
     }

     /// @brief Normalize a vector.
     /// @return normalized vector.
     inline Vec normalize() const noexcept
     {
          return Vec{ glm::normalize( vec_ ) };
     }

     /// @brief Check equality of vectors with given precision.
     /// @param[in] vec vector to check equality.
     /// @param[in] precision precision to compare with.
     /// @return true if vectors are equal with precision, false otherwise.
     bool equals( const Vec& vec, float precision = 0.0f ) const noexcept;

     /// @brief Find distance to given vector.
     /// @param[in] vec vector to calculate distance.
     /// @return distance to given vector.
     inline T distance( const Vec& vec ) const noexcept
     {
          return glm::distance( vec_, vec.vec_ );
     }

     /// @brief Find dot product of vectors.
     /// @param[in] vec vector.
     /// @return dot product of vectors.
     inline T dot( const Vec& vec ) const noexcept
     {
          return glm::dot( vec_, vec.vec_ );
     }

     /// @brief Find cross product of vectors.
     /// @param[in] vec first vector.
     /// @return cross product of vectors.
     _16NAR_ENABLE_IF( N == 3 ) inline Vec cross( const Vec& vec ) const noexcept
     {
          return Vec{ glm::cross( vec_, vec.vec_ ) };
     }

     /// @brief Orient a vector to point away from the plane defined by two vectors.
     /// @param[in] incident vector of sight directed to the plane.
     /// @param[in] normal normal vector of the plane.
     /// @return self if dot( normal, incident ) < 0.0, -self otherwise.
     inline Vec faceforward( const Vec& incident, const Vec& normal ) const noexcept
     {
          return Vec{ glm::faceforward( vec_, incident.vec_, normal.vec_ ) };
     }

     /// @brief Reflect vector from a plane defined by its normal.
     /// @param[in] normal normal of the plane (must be normalized).
     /// @return self - 2 * dot( normal, self ) * normal.
     inline Vec reflect( const Vec& normal ) const noexcept
     {
          return Vec{ glm::reflect( vec_, normal.vec_ ) };
     }

     /// @brief Refract vector.
     /// @param[in] normal normal to the plane of refraction (must be normalized).
     /// @param[in] eta ratio of indices of refraction (n1 / n2).
     /// @return refracted vector.
     _16NAR_ENABLE_IF( std::is_floating_point< T >::value )
     inline Vec refract( const Vec& normal, T eta ) const noexcept
     {
          return Vec{ glm::refract( vec_, normal.vec_, eta ) };
     }

private:
     /// @brief Constructor with inner vector representation.
     /// @details For inner use.
     /// @param[in] vec inner vector representation.
     explicit Vec( const glm::vec< N, T, glm::qualifier::defaultp >& vec ) noexcept : vec_{ vec } {}

     glm::vec< N, T, glm::qualifier::defaultp > vec_;  ///< inner vector representation.
}; // class Vec


/// @brief Comprasion operator for two vectors.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vectors' elements.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return true if vectors are equal, false otherwise.
template < std::size_t N, typename T >
inline bool operator==( const Vec< N, T >& lhs, const Vec< N, T >& rhs ) noexcept;


/// @brief Inverse comprasion operator for two vectors.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vectors' elements.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return true if vectors are not equal, false otherwise.
template < std::size_t N, typename T >
inline bool operator!=( const Vec< N, T >& lhs, const Vec< N, T >& rhs ) noexcept;


/// @brief Add operator.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vectors' elements.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return sum of vectors.
template < std::size_t N, typename T >
inline Vec< N, T > operator+( const Vec< N, T >& lhs, const Vec< N, T >& rhs ) noexcept;


/// @brief Subtract operator.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vectors' elements.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return difference of vectors.
template < std::size_t N, typename T >
inline Vec< N, T > operator-( const Vec< N, T >& lhs, const Vec< N, T >& rhs ) noexcept;


/// @brief Multiplication by a scalar operator.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vector's elements and scalar.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return product of vector and scalar.
template < std::size_t N, typename T >
inline Vec< N, T > operator*( const Vec< N, T >& lhs, T rhs ) noexcept;


/// @brief Multiplication by a scalar operator.
/// @tparam N vector size (number of dimensions).
/// @tparam T type of vector's elements and scalar.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return product of vector and scalar.
template < std::size_t N, typename T >
inline Vec< N, T > operator*( T lhs, const Vec< N, T >& rhs ) noexcept;


using Vec2f = Vec< 2, float >;
using Vec3f = Vec< 3, float >;
using Vec4f = Vec< 4, float >;

using Vec2i = Vec< 2, int >;
using Vec3i = Vec< 3, int >;
using Vec4i = Vec< 4, int >;

} // namespace _16nar

#include "vec.inl"

#endif // #ifndef _16NAR_VEC_H
