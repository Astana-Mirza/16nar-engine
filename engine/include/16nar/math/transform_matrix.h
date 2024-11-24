/// @file Header file with TransformMatrix class declaration.
#ifndef _16NAR_TRANSFORM_MATRIX_H
#define _16NAR_TRANSFORM_MATRIX_H

#include <16nar/16nardefs.h>
#include <16nar/math/vec.h>
#include <16nar/math/rectangle.h>

#include <glm/ext/matrix_float4x4.hpp>

namespace _16nar
{

/// @brief Transformation matrix (4x4) which can be used for 2D and 3D transformations.
class ENGINE_API TransformMatrix
{
public:
     /// @brief Constructor which makes identity matrix.
     TransformMatrix() noexcept : mat_( 1.0f ) {}

     /// @brief Constructor which makes matrix of values.
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
                      float x3, float y3, float z3, float w3 ) noexcept;

     /// @brief Get element of matrix (const).
     /// @param[in] i index of a column.
     /// @param[in] j index of a row.
     /// @return element of matrix.
     const float& get( std::size_t i, std::size_t j ) const noexcept;

     /// @brief Get element of matrix.
     /// @param[in] i index of a column.
     /// @param[in] j index of a row.
     /// @return element of matrix.
     float& get( std::size_t i, std::size_t j ) noexcept;

     /// @brief Multiply-assignment operator.
     /// @param[in] other other matrix to be multiplied.
     /// @return product of matrices.
     TransformMatrix& operator*=( const TransformMatrix& other ) noexcept;

     /// @brief Vector (4-dimensional) multiplication operator.
     /// @param[in] vector right operand.
     /// @return transformed vector.
     Vec4f operator*( const Vec4f& vector ) const noexcept;

     /// @brief Vector (3-dimensional) multiplication operator.
     /// @param[in] vector right operand.
     /// @return transformed vector.
     Vec3f operator*( const Vec3f& vector ) const noexcept;

     /// @brief Vector (2-dimensional) multiplication operator.
     /// @param[in] vector right operand.
     /// @return transformed vector.
     Vec2f operator*( const Vec2f& vector ) const noexcept;

     /// @brief Rectangle multiplication operator.
     /// @details Keeps rectangle axis aligned, made of bounds of transformed rectangle.
     /// @param[in] rect right operand.
     /// @return transformed rectangle.
     FloatRect operator*( const FloatRect& rect ) const noexcept;

     /// @brief Get inverse matrix.
     /// @return inverse matrix.
     inline TransformMatrix inv() const noexcept
     {
          return TransformMatrix{ glm::inverse( mat_) };
     }

     /// @brief Get determinant of the matrix.
     /// @return determinant of the matrix.
     inline float det() const noexcept
     {
          return glm::determinant( mat_ );
     }

     /// @brief Get transposed matrix.
     /// @return transposed matrix.
     inline TransformMatrix transpose() const noexcept
     {
          return TransformMatrix{ glm::transpose( mat_ ) };
     }

     /// @brief Get underlying matrix data.
     /// @return underlying matrix data.
     inline const float *data() const noexcept
     {
          return &mat_[ 0 ][ 0 ];
     }

     /// @brief Get faster inverse matrix for affine matrix.
     /// @return inverse matrix.
     TransformMatrix affine_inv() const noexcept;

     /// @brief Get inverse transpose of the matrix.
     /// @return inverse transpose of the matrix.
     TransformMatrix inv_transpose() const noexcept;

     /// @brief Check equality of matrices with given precision.
     /// @param[in] mat matrix to check equality.
     /// @param[in] precision precision to compare with.
     /// @return true if matrices are equal with precision, false otherwise.
     bool equals( const TransformMatrix& mat, float precision = 0.0f ) const noexcept;

     // 2D API

     /// @brief Add translation (2D API).
     /// @param[in] offset move offset.
     /// @return current matrix after movement.
     TransformMatrix& move( const Vec2f& offset ) noexcept;

     /// @brief Add scale (2D API).
     /// @param[in] factors scale factors.
     /// @return current matrix after scale.
     TransformMatrix& scale( const Vec2f& factors ) noexcept;

     /// @brief Add scale with a pivot (2D API).
     /// @param[in] factors scale factors.
     /// @param[in] pivot transform origin point.
     /// @return current matrix after scale.
     TransformMatrix& scale( const Vec2f& factors, const Vec2f& pivot ) noexcept;

     /// @brief Add clockwise rotation (2D API).
     /// @param[in] angle rotation angle, in radians.
     /// @return current matrix after rotation.
     TransformMatrix& rotate( float angle ) noexcept;

     /// @brief Add clockwise rotation with a pivot (2D API).
     /// @param[in] angle rotation angle, in radians.
     /// @param[in] pivot transform origin point.
     /// @return current matrix after rotation.
     TransformMatrix& rotate( float angle, const Vec2f& pivot ) noexcept;

     // 3D API

     /// @brief Add translation (3D API).
     /// @param[in] offset move offset.
     /// @return current matrix after movement.
     TransformMatrix& move( const Vec3f& offset ) noexcept;

     /// @brief Add scale.
     /// @param[in] factors scale factors (3D API).
     /// @return current matrix after scale.
     TransformMatrix& scale( const Vec3f& factors ) noexcept;

     /// @brief Add scale with a pivot (3D API).
     /// @param[in] factors scale factors.
     /// @param[in] pivot transform origin point.
     /// @return current matrix after scale.
     TransformMatrix& scale( const Vec3f& factors, const Vec3f& pivot ) noexcept;

     /// @brief Add clockwise rotation (3D API).
     /// @param[in] angle rotation angle, in radians.
     /// @param[in] axis rotation axis.
     /// @return current matrix after rotation.
     TransformMatrix& rotate( float angle, const Vec3f& axis ) noexcept;

     /// @brief Add clockwise rotation with a pivot (3D API).
     /// @param[in] angle rotation angle, in radians.
     /// @param[in] axis rotation axis.
     /// @param[in] pivot transform origin point.
     /// @return current matrix after rotation.
     TransformMatrix& rotate( float angle, const Vec3f& axis, const Vec3f& pivot ) noexcept;

private:
     /// @brief Constructor which uses inner matrix representation.
     /// @details For inner use.
     /// @param[in] mat inner matrix representation.
     explicit TransformMatrix( const glm::mat4x4& mat ) noexcept : mat_{ mat } {}

     glm::mat4x4 mat_;   ///< inner representation of matrix.
};


/// @brief Matrix multiplication operator.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return matrix product.
TransformMatrix operator*( const TransformMatrix& lhs, const TransformMatrix& rhs ) noexcept;

/// @brief Matrix comprasion operator.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return true if matrices are equal, false otherwise.
bool operator==( const TransformMatrix& lhs, const TransformMatrix& rhs ) noexcept;

/// @brief Matrix inverse comprasion operator.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return true if matrices are not equal, false otherwise.
bool operator!=( const TransformMatrix& lhs, const TransformMatrix& rhs ) noexcept;

} // namespace _16nar

#endif // #ifndef _16NAR_TRANSFORM_MATRIX_H
