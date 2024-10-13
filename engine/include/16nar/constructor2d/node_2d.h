/// @file
/// @brief File with Node2D class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_NODE_2D_H
#define _16NAR_CONSTRUCTOR_2D_NODE_2D_H

#include <16nar/math/transform_matrix.h>

namespace _16nar::constructor2d
{

/// @brief A simple node that keep its transformation.
class ENGINE_API Node2D
{
public:
     /// @brief Get node's current position.
     /// @return node's current position.
     const Vec2f& get_position() const;

     /// @brief Get node's current rotation.
     /// @return node's current rotation.
     float get_rotation() const;

     /// @brief Get node's current scale.
     /// @return node's current scale.
     const Vec2f& get_scale() const;

     /// @brief Get node's current tarnsform origin point.
     /// @return node's current tarnsform origin point.
     const Vec2f& get_origin() const;

     /// @brief Get node's current transformation matrix.
     /// @return node's current transformation matrix.
     const TransformMatrix& get_transform_matr() const;

     /// @brief Get node's current inverse transformation matrix.
     /// @return node's current inverse transformation matrix.
     const TransformMatrix& get_inv_transform_matr() const;

     /// @brief Set node's current position.
     /// @param[in] x node's x coordinate.
     /// @param[in] y node's y coordinate.
     virtual void set_position( float x, float y );

     /// @brief Set node's current position.
     /// @param[in] position coordinate vector.
     virtual void set_position( const Vec2f& position );

     /// @brief Set node's current rotation.
     /// @param[in] angle node's rotation angle, in degrees.
     virtual void set_rotation( float angle );

     /// @brief Set node's current scale.
     /// @param[in] factor_x node's x scale factor.
     /// @param[in] factor_y node's y scale factor.
     virtual void set_scale( float factor_x, float factor_y );

     /// @brief Set node's current scale.
     /// @param factors node's scale factors vector.
     virtual void set_scale( const Vec2f& factors );

     /// @brief Set node's current transform origin point.
     /// @param[in] x node's origin x coordinate.
     /// @param[in] y node's origin y coordinate.
     virtual void set_origin( float x, float y );

     /// @brief Set node's current transform origin point.
     /// @param[in] origin node's origin coordinate vector.
     virtual void set_origin( const Vec2f& origin );

     /// @brief Move node by given offsets.
     /// @param[in] offset_x node's x moving offset.
     /// @param[in] offset_y node's y moving offset.
     virtual void move( float offset_x, float offset_y );

     /// @brief Move node by given offset vector.
     /// @param[in] offset node's moving offset.
     virtual void move( const Vec2f& offset );

     /// @brief Rotate node by given angle.
     /// @param[in] angle node's rotation angle, in degrees.
     virtual void rotate( float angle );

     /// @brief Scale node by given factors.
     /// @param[in] factor_x node's x scale factor.
     /// @param[in] factor_y node's y scale factor.
     virtual void scale( float factor_x, float factor_y );

     /// @brief Scale current node by given factors.
     /// @param[in] factors node's scale factor vector.
     virtual void scale( const Vec2f& factor );

private:
	TransformMatrix transform_;   ///< matrix with transformation of this node.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_NODE_2D_H
