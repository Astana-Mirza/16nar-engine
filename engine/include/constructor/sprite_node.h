/// @file
/// @brief File with SpriteNode class definition.
#ifndef _16NAR_SPRITE_NODE_H
#define _16NAR_SPRITE_NODE_H

#include <constructor/abstract/drawable_node.h>

namespace _16nar
{

/// @brief A node that represents one sprite on the scene.
class ENGINE_API SpriteNode : public DrawableNode
{
public:
     using DrawableNode::DrawableNode;

     /// @brief Constructor creating a sprite with entire texture.
     /// @param quad base quadrant of this node.
     /// @param tex texture (picture) for the sprite.
     SpriteNode( Quadrant *quad, const Texture& tex );

     /// @brief Constructor creating a sprite with part of texture.
     /// @param quad base quadrant of this node.
     /// @param tex texture (picture) for the sprite.
     /// @param rect rectangular area of texture displayed by the sprite.
     SpriteNode( Quadrant *quad, const Texture& tex, const IntRect& rect );

     /// @brief Draw this object on the target.
     /// @param target target where object should be rendered.
     void draw( RenderTarget& target ) const;

     /// @brief Set texture for the sprite.
     /// @param texture texture (picture) for the sprite.
     /// @param reset_rect should existing rect be used or reset to fit full texture.
     void set_texture( const Texture& texture, bool reset_rect = false );

     /// @brief Sets rectangular area displayed by the sprite.
     /// @param rect rectangle area of texture.
     void set_texture_rect( const IntRect& rect );

     /// @brief Sets color of an object.
     /// @param color color of an object
     void set_color( const Color& color );

     /// @brief Gets color of an object.
     const Color& get_color() const;

     /// @brief Gets local bounds of an object (in its own coordinates).
     FloatRect get_local_bounds() const;

     /// @brief Gets texture of the sprite.
     const Texture *get_texture() const;

     /// @brief Gets texture rectangular area displayed by the sprite.
     const IntRect& get_texture_rect() const;

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
	Sprite sprite_;         ///< sprite that is displayed on the screen.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SPRITE_NODE_H
