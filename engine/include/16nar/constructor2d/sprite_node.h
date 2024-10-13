/// @file
/// @brief File with SpriteNode class definition.
#ifndef _16NAR_SPRITE_NODE_H
#define _16NAR_SPRITE_NODE_H

#include <16nar/constructor2d/node_2d.h>

namespace _16nar
{

/// @brief A node that represents one sprite on the scene.
class ENGINE_API SpriteNode : public Node2D, public Drawable2D
{
public:
     using DrawableNode::DrawableNode;

     /// @brief Constructor creating a sprite with entire texture.
     /// @param[in] render_system render system used to draw this object.
     /// @param[in] tex texture (picture) for the sprite.
     SpriteNode( RenderSystem *render_system, const Texture& tex );

     /// @brief Constructor creating a sprite with part of texture.
     /// @param[in] render_system render system used to draw this object.
     /// @param[in] tex texture (picture) for the sprite.
     /// @param[in] rect rectangular area of texture displayed by the sprite.
     SpriteNode( RenderSystem *render_system, const Texture& tex, const IntRect& rect );

     /// @brief Get render data of the object.
     RenderData get_render_data() const override;

     /// @brief Set texture for the sprite.
     /// @param[in] texture texture (picture) for the sprite.
     /// @param[in] reset_rect should existing rect be used or reset to fit full texture.
     void set_texture( const Texture& texture, bool reset_rect = false );

     /// @brief Sets rectangular area displayed by the sprite.
     /// @param[in] rect rectangle area of texture.
     void set_texture_rect( const IntRect& rect );

     /// @brief Sets color of an object.
     /// @param[in] color color of an object
     void set_color( const Color& color ) override;

     /// @brief Gets color of an object.
     const Color& get_color() const override;

     /// @brief Gets local bounds of an object (in its own coordinates).
     FloatRect get_local_bounds() const override;

     /// @brief Gets texture of the sprite.
     const Texture *get_texture() const;

     /// @brief Gets texture rectangular area displayed by the sprite.
     const IntRect& get_texture_rect() const;

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
     /// @param[in] factors object's scale factor vector.
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
	Sprite sprite_;         ///< sprite that is displayed on the screen.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SPRITE_NODE_H
