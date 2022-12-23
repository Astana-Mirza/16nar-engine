/// @file
/// @brief File with Node2D class definition.
#ifndef _16NAR_NODE_2D_H
#define _16NAR_NODE_2D_H

#include <16nar/constructor/abstract/node.h>

namespace _16nar
{

/// @brief A simple node that has its own transformation object and can be transformed.
class ENGINE_API Node2D : public Node
{
public:
     /// @brief Gets current object position.
     const Vector2f& get_position() const;

     /// @brief Gets current object rotation.
     float get_rotation() const;

     /// @brief Gets current object scale.
     const Vector2f& get_scale() const;

     /// @brief Gets current object tarnsform origin point.
     const Vector2f& get_origin() const;

     /// @brief Gets current object's transformation matrix.
     const TransformMatrix& get_transform_matr() const;

     /// @brief Gets current object's inverse transformation matrix.
     const TransformMatrix& get_inv_transform_matr() const;

     /// @brief Sets current object position.
     /// @param x object's x coordinate.
     /// @param y object's y coordinate.
     void set_position( float x, float y );

     /// @brief Sets current object position.
     /// @param position object's coordinate vector.
     void set_position( const Vector2f& position );

     /// @brief Sets current object rotation.
     /// @param angle object's rotation angle, in degrees.
     void set_rotation( float angle );

     /// @brief Sets current object scale.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     void set_scale( float factor_x, float factor_y );

     /// @brief Sets current object scale.
     /// @param factors object's scale factor vector.
     void set_scale( const Vector2f& factors );

     /// @brief Sets current object transform origin point.
     /// @param x object's origin x coordinate.
     /// @param y object's origin y coordinate.
     void set_origin( float x, float y );

     /// @brief Sets current object transform origin point.
     /// @param origin object's origin coordinate vector.
     void set_origin( const Vector2f& origin );

     /// @brief Moves object by given offsets.
     /// @param offset_x object's x moving offset.
     /// @param offset_y object's y moving offset.
     void move( float offset_x, float offset_y );

     /// @brief Moves object by given offset vector.
     /// @param offset object's moving offset.
     void move( const Vector2f& offset );

     /// @brief Rotates object by given angle.
     /// @param angle object's rotation angle, in degrees.
     void rotate( float angle );

     /// @brief Scales object by given factors.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     void scale( float factor_x, float factor_y );

     /// @brief Scales current object by given factors.
     /// @param factors object's scale factor vector.
     void scale( const Vector2f& factor );

private:
	Transform transform_;        ///< object that keeps transformation of this node.
};

} // namespace _16nar

#endif // #ifndef _16NAR_NODE_2D_H
