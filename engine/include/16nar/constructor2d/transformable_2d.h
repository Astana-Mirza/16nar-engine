/// @file
/// @brief Header file with Transformable2D class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_TRANSFORMABLE_2D_H
#define _16NAR_CONSTRUCTOR_2D_TRANSFORMABLE_2D_H

#include <16nar/16nardefs.h>
#include <16nar/math/vec.h>
#include <16nar/math/transform_matrix.h>

namespace _16nar::constructor2d
{

/// @brief Base class providing functionality for 2D transformations.
class ENGINE_API Transformable2D
{
public:
     /// @brief Default constructor.
     Transformable2D() noexcept;

     /// @brief Get current object's position.
     /// @return current object's position.
     const Vec2f& get_position() const noexcept;

     /// @brief Get current object's rotation.
     /// @return current object's rotation.
     float get_rotation() const noexcept;

     /// @brief Get current object's scale.
     /// @return current object's scale.
     const Vec2f& get_scale() const noexcept;

     /// @brief Get current object's transform origin point.
     /// @return current object's transform origin point.
     const Vec2f& get_origin() const noexcept;

     /// @brief Get current object's transformation matrix.
     /// @return current object's transformation matrix.
     const TransformMatrix& get_transform_matr() const noexcept;

     /// @brief Get current object's inverse transformation matrix.
     /// @return current object's inverse transformation matrix.
     TransformMatrix get_inv_transform_matr() const noexcept;

     /// @brief Set current object position.
     /// @param[in] x object's x coordinate.
     /// @param[in] y object's y coordinate.
     void set_position( float x, float y ) noexcept;

     /// @brief Set current object position.
     /// @param[in] position object's coordinate vector.
     void set_position( const Vec2f& position ) noexcept;

     /// @brief Set current object rotation.
     /// @param[in] angle object's rotation angle, in degrees.
     void set_rotation( float angle ) noexcept;

     /// @brief Set current object scale.
     /// @param[in] factor_x object's x scale factor.
     /// @param[in] factor_y object's y scale factor.
     void set_scale( float factor_x, float factor_y ) noexcept;

     /// @brief Set current object scale.
     /// @param[in] factors object's scale factor vector.
     void set_scale( const Vec2f& factors ) noexcept;

     /// @brief Set current object transform origin point.
     /// @param[in] x object's origin x coordinate.
     /// @param[in] y object's origin y coordinate.
     void set_origin( float x, float y ) noexcept;

     /// @brief Set current object transform origin point.
     /// @param[in] origin object's origin coordinate vector.
     void set_origin( const Vec2f& origin ) noexcept;

     /// @brief Move object by given offsets.
     /// @param[in] offset_x object's x moving offset.
     /// @param[in] offset_y object's y moving offset.
     void move( float offset_x, float offset_y ) noexcept;

     /// @brief Move object by given offset vector.
     /// @param[in] offset object's moving offset.
     void move( const Vec2f& offset ) noexcept;

     /// @brief Rotate object by given angle.
     /// @param[in] angle object's rotation angle, in degrees.
     void rotate( float angle ) noexcept;

     /// @brief Scale object by given factors.
     /// @param[in] factor_x object's x scale factor.
     /// @param[in] factor_y object's y scale factor.
     void scale( float factor_x, float factor_y ) noexcept;

     /// @brief Scale current object by given factors.
     /// @param[in] factors object's scale factor vector.
     void scale( const Vec2f& factor ) noexcept;

protected:
     /// @brief Calculate transformation matrix, if needed.
     /// @details Transformation matrix is calculated if any parameters
     /// have changed since last calculation.
     /// @return true if transformation matrix changed, false otherwise.
     bool calculate_matr() noexcept;

private:
     TransformMatrix matr_;        ///< matrix with transformation of this object.
     Vec2f position_;              ///< position of this object.
     Vec2f origin_;                ///< transformation origin point of this object.
     Vec2f scale_;                 ///< scale of this object.
     float rotation_;              ///< clockwise rotation of this object, in degrees.
	bool transformed_;            ///< indicator of object being transformed.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_TRANSFORMABLE_2D_H
