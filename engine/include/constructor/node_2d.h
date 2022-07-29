/// @file
/// File with Node2D class definition.
#ifndef _16NAR_NODE_2D_H
#define _16NAR_NODE_2D_H

#include <constructor/abstract/node.h>

namespace _16nar
{

/// A simple node that has its own transformation object and can be transformed.
class ENGINE_API Node2D : public Node
{
public:
     /// Gets current object position.
     const Vector2f& get_position() const;

     /// Gets current object rotation.
     float get_rotation() const;

     /// Gets current object scale.
     const Vector2f& get_scale() const;

     /// Gets current object tarnsform origin point.
     const Vector2f& get_origin() const;

     /// Gets current object's transformation matrix.
     const TransformMatrix& get_transform_matr() const;

     /// Gets current object's inverse transformation matrix.
     const TransformMatrix& get_inv_transform_matr() const;

     /// Sets current object position.
     /// @param x object's x coordinate.
     /// @param y object's y coordinate.
     void set_position( float x, float y );

     /// Sets current object position.
     /// @param position object's coordinate vector.
     void set_position( const Vector2f& position );

     /// Sets current object rotation.
     /// @param angle object's rotation angle, in degrees.
     void set_rotation( float angle );

     /// Sets current object scale.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     void set_scale( float factor_x, float factor_y );

     /// Sets current object scale.
     /// @param factors object's scale factor vector.
     void set_scale( const Vector2f& factors );

     /// Sets current object transform origin point.
     /// @param x object's origin x coordinate.
     /// @param y object's origin y coordinate.
     void set_origin( float x, float y );

     /// Sets current object transform origin point.
     /// @param origin object's origin coordinate vector.
     void set_origin( const Vector2f& origin );

     /// Moves object by given offsets.
     /// @param offset_x object's x moving offset.
     /// @param offset_y object's y moving offset.
     void move( float offset_x, float offset_y );

     /// Moves object by given offset vector.
     /// @param offset object's moving offset.
     void move( const Vector2f& offset );

     /// Rotates object by given angle.
     /// @param angle object's rotation angle, in degrees.
     void rotate( float angle );

     /// Scales object by given factors.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     void scale( float factor_x, float factor_y );

     /// Scales current object by given factors.
     /// @param factors object's scale factor vector.
     void scale( const Vector2f& factor );

private:
	Transform transform_;        ///< object that keeps transformation of this node.
};

} // namespace _16nar

#endif // #ifndef _16NAR_NODE_2D_H
