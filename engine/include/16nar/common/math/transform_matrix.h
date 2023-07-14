/// @file Header file with TransformMatrix class declaration.
#ifndef _16NAR_TRANSFORM_MATRIX_H
#define _16NAR_TRANSFORM_MATRIX_H

#include <16nar/common/16nardefs.h>
#include <16nar/common/math/vec.h>
#include <16nar/common/math/rectangle.h>

#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
#    include <glm/ext/matrix_float4x4.hpp>
#endif

namespace _16nar
{

/// @brief Transformation matrix (4x4) which can be used for 2D and 3D transformations.
class ENGINE_API TransformMatrix
{
public:
     /// @brief Constructor which makes identity matrix.
     TransformMatrix() : mat_( 1.0f ) {}

     /// @brief Comstructor which makes matrix of values.
     /// @param[in] x0 value (0, 0).
     /// @param[in] y0 value (0, 1).
     /// @param[in] z0 value (0, 2).
     /// @param[in] w0 value (0, 3).
     /// @param[in] x1 value (1, 0).
     /// @param[in] y1 value (1, 1).
     /// @param[in] z1 value (1, 2).
     /// @param[in] w1 value (1, 3).
     /// @param[in] x2 value (2, 0).
     /// @param[in] y2 value (2, 1).
     /// @param[in] z2 value (2, 2).
     /// @param[in] w2 value (2, 3).
     /// @param[in] x3 value (3, 0).
     /// @param[in] y3 value (3, 1).
     /// @param[in] z3 value (3, 2).
     /// @param[in] w3 value (3, 3).
     TransformMatrix( float x0, float y0, float z0, float w0,
                      float x1, float y1, float z1, float w1, 
                      float x2, float y2, float z2, float w2, 
                      float x3, float y3, float z3, float w3 );

     /// @brief Copy constructor.
     /// @param[in] other matrix to be copied.
     TransformMatrix( const TransformMatrix& other );

     /// @brief Assignment operator.
     /// @param[in] other matrix to be copied.
     TransformMatrix& operator=( const TransformMatrix& other );

     /// @brief Get element of matrix (const).
     /// @param[in] i index of a column.
     /// @param[in] j index of a row.
     const float& get( std::size_t i, std::size_t j ) const;

     /// @brief Get element of matrix.
     /// @param[in] i index of a column.
     /// @param[in] j index of a row.
     float& get( std::size_t i, std::size_t j );

     /// @brief Multiply-assignment operator.
     /// @param[in] other other matrix to be multiplied.
     TransformMatrix& operator*=( const TransformMatrix& other );

     /// @brief Vector (4-dimensional) multiplication operator.
     /// @param[in] vector right operand.
     Vec4f operator*( const Vec4f& vector ) const;

     /// @brief Vector (3-dimensional) multiplication operator.
     /// @param[in] vector right operand.
     Vec3f operator*( const Vec3f& vector ) const;

     /// @brief Vector (2-dimensional) multiplication operator.
     /// @param[in] vector right operand.
     Vec2f operator*( const Vec2f& vector ) const;

     /// @brief Rectangle multiplication operator.
     /// @detail Keeps rectangle axis aligned, made of bounds of transformed rectangle.
     /// @param[in] rect right operand.
     FloatRect operator*( const FloatRect& rect ) const;

     /// @brief Get inverse matrix.
     inline TransformMatrix inv() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return TransformMatrix{ glm::inverse( mat_) };
#endif
     }

     /// @brief Get determinant of the matrix.
     inline float det() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return glm::determinant( mat_ );
#endif
     }

     /// @brief Get transposed matrix.
     inline TransformMatrix transpose() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return TransformMatrix{ glm::transpose( mat_ ) };
#endif
     }

     /// @brief Get underlying matrix data.
     inline const float *data() const
     {
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
          return &mat_[ 0 ][ 0 ];
#endif
     }

     /// @brief Get faster inverse matrix for affine matrix.
     TransformMatrix affine_inv() const;

     /// @brief Get inverse transpose of the matrix.
     TransformMatrix inv_transpose() const;

     /// @brief Check equality of matrices with given precision.
     /// @param[in] mat matrix to check equality.
     /// @param[in] precision precision to compare with.
     bool equals( const TransformMatrix& mat, float precision ) const;

     // 2D API

     /// @brief Add translation (2D API).
     /// @param[in] offset move offset.
     TransformMatrix& move( const Vec2f& offset );

     /// @brief Add scale (2D API).
     /// @param[in] factors scale factors.
     TransformMatrix& scale( const Vec2f& factors );

     /// @brief Add scale with a pivot (2D API).
     /// @param[in] factors scale factors.
     /// @param[in] pivot transform origin point.
     TransformMatrix& scale( const Vec2f& factors, const Vec2f& pivot );

     /// @brief Add clockwise rotation (2D API).
     /// @param[in] angle rotation angle, in radians.
     TransformMatrix& rotate( float angle );

     /// @brief Add clockwise rotation with a pivot (2D API).
     /// @param[in] angle rotation angle, in radians.
     /// @param[in] pivot transform origin point.
     TransformMatrix& rotate( float angle, const Vec2f& pivot );

     // 3D API

     /// @brief Add translation (3D API).
     /// @param[in] offset move offset.
     TransformMatrix& move( const Vec3f& offset );

     /// @brief Add scale.
     /// @param[in] factors scale factors (3D API).
     TransformMatrix& scale( const Vec3f& factors );

     /// @brief Add scale with a pivot (3D API).
     /// @param[in] factors scale factors.
     /// @param[in] pivot transform origin point.
     TransformMatrix& scale( const Vec3f& factors, const Vec3f& pivot );

     /// @brief Add clockwise rotation (3D API).
     /// @param[in] angle rotation angle, in radians.
     /// @param[in] axis rotation axis.
     TransformMatrix& rotate( float angle, const Vec3f& axis );

     /// @brief Add clockwise rotation with a pivot (3D API).
     /// @param[in] angle rotation angle, in radians.
     /// @param[in] axis rotation axis.
     /// @param[in] pivot transform origin point.
     TransformMatrix& rotate( float angle, const Vec3f& axis, const Vec3f& pivot );

private:
#if defined( NARENGINE_RENDER_OPENGL ) || defined( NARENGINE_RENDER_VULKAN )
     /// @brief Constructor which uses inner matrix representation.
     /// @detail For inner use.
     /// @param[in] mat inner matrix representation.
     explicit TransformMatrix(const glm::mat4x4& mat) : mat_{ mat } {}

     glm::mat4x4 mat_;   ///< inner representation of matrix.
#endif
};


/// @brief Matrix multiplication operator.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
TransformMatrix operator*( const TransformMatrix& lhs, const TransformMatrix& rhs );

/// @brief Matrix comprasion operator.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
bool operator==( const TransformMatrix& lhs, const TransformMatrix& rhs );

/// @brief Matrix inverse comprasion operator.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
bool operator!=( const TransformMatrix& lhs, const TransformMatrix& rhs );

} // namespace _16nar

#endif // #ifndef _16NAR_TRANSFORM_MATRIX_H
