/// @file
/// @brief Header file with Transformable abstract class definition.
#ifndef _16NAR_TRANSFORMABLE_H
#define _16NAR_TRANSFORMABLE_H

#include <16nar/16nardefs.h>

namespace _16nar
{

/// @brief Abstract base class providing functionality for transformations
class ENGINE_API Transformable
{
public:
     virtual ~Transformable()                           = default;

     /// @brief Gets current object position.
     virtual const Vector2f& get_position() const             = 0;

     /// @brief Gets current object rotation.
     virtual float get_rotation() const                       = 0;

     /// @brief Gets current object scale.
     virtual const Vector2f& get_scale() const                = 0;

     /// @brief Gets current object tarnsform origin point.
     virtual const Vector2f& get_origin() const               = 0;

     /// @brief Gets current object's transformation matrix.
     virtual const TransformMatrix& get_transform_matr() const           = 0;

     /// @brief Gets current object's inverse transformation matrix.
     virtual const TransformMatrix& get_inv_transform_matr() const       = 0;

     /// @brief Sets current object position.
     /// @param x object's x coordinate.
     /// @param y object's y coordinate.
     virtual void set_position( float x, float y )            = 0;

     /// @brief Sets current object position.
     /// @param position object's coordinate vector.
     virtual void set_position( const Vector2f& position )    = 0;

     /// @brief Sets current object rotation.
     /// @param angle object's rotation angle, in degrees.
     virtual void set_rotation( float angle )                 = 0;

     /// @brief Sets current object scale.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     virtual void set_scale( float factor_x, float factor_y ) = 0;

     /// @brief Sets current object scale.
     /// @param factors object's scale factor vector.
     virtual void set_scale( const Vector2f& factors )        = 0;

     /// @brief Sets current object transform origin point.
     /// @param x object's origin x coordinate.
     /// @param y object's origin y coordinate.
     virtual void set_origin( float x, float y )              = 0;

     /// @brief Sets current object transform origin point.
     /// @param origin object's origin coordinate vector.
     virtual void set_origin( const Vector2f& origin )        = 0;

     /// @brief Moves object by given offsets.
     /// @param offset_x object's x moving offset.
     /// @param offset_y object's y moving offset.
     virtual void move( float offset_x, float offset_y )      = 0;

     /// @brief Moves object by given offset vector.
     /// @param offset object's moving offset.
     virtual void move( const Vector2f& offset )              = 0;

     /// @brief Rotates object by given angle.
     /// @param angle object's rotation angle, in degrees.
     virtual void rotate( float angle )                       = 0;

     /// @brief Scales object by given factors.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     virtual void scale( float factor_x, float factor_y )     = 0;

     /// @brief Scales current object by given factors.
     /// @param factors object's scale factor vector.
     virtual void scale( const Vector2f& factor )             = 0;

protected:
	bool transformed_ = false;        ///< indicator of object being transformed.
};

} // namespace _16nar

#endif // #ifndef _16NAR_TRANSFORMABLE_H
