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
     const Vector2f& get_position() const override;

     /// @brief Gets current object rotation.
     float get_rotation() const override;

     /// @brief Gets current object scale.
     const Vector2f& get_scale() const override;

     /// @brief Gets current object tarnsform origin point.
     const Vector2f& get_origin() const override;

     /// @brief Gets current object's transformation matrix.
     const TransformMatrix& get_transform_matr() const override;

     /// @brief Gets current object's inverse transformation matrix.
     const TransformMatrix& get_inv_transform_matr() const override;

     /// @brief Sets current object position.
     /// @param[in] x object's x coordinate.
     /// @param[in] y object's y coordinate.
     void set_position( float x, float y ) override;

     /// @brief Sets current object position.
     /// @param[in] position object's coordinate vector.
     void set_position( const Vector2f& position ) override;

     /// @brief Sets current object rotation.
     /// @param[in] angle object's rotation angle, in degrees.
     void set_rotation( float angle ) override;

     /// @brief Sets current object scale.
     /// @param[in] factor_x object's x scale factor.
     /// @param[in] factor_y object's y scale factor.
     void set_scale( float factor_x, float factor_y ) override;

     /// @brief Sets current object scale.
     /// @param factors object's scale factor vector.
     void set_scale( const Vector2f& factors ) override;

     /// @brief Sets current object transform origin point.
     /// @param[in] x object's origin x coordinate.
     /// @param[in] y object's origin y coordinate.
     void set_origin( float x, float y ) override;

     /// @brief Sets current object transform origin point.
     /// @param[in] origin object's origin coordinate vector.
     void set_origin( const Vector2f& origin ) override;

     /// @brief Moves object by given offsets.
     /// @param[in] offset_x object's x moving offset.
     /// @param[in] offset_y object's y moving offset.
     void move( float offset_x, float offset_y ) override;

     /// @brief Moves object by given offset vector.
     /// @param[in] offset object's moving offset.
     void move( const Vector2f& offset ) override;

     /// @brief Rotates object by given angle.
     /// @param[in] angle object's rotation angle, in degrees.
     void rotate( float angle ) override;

     /// @brief Scales object by given factors.
     /// @param[in] factor_x object's x scale factor.
     /// @param[in] factor_y object's y scale factor.
     void scale( float factor_x, float factor_y ) override;

     /// @brief Scales current object by given factors.
     /// @param[in] factors object's scale factor vector.
     void scale( const Vector2f& factor ) override;

private:
	Transform transform_;        ///< object that keeps transformation of this node.
};

} // namespace _16nar

#endif // #ifndef _16NAR_NODE_2D_H
