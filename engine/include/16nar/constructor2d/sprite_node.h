/// @file
/// @brief Header file with SpriteNode base class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_SPRITE_NODE_H
#define _16NAR_CONSTRUCTOR_2D_SPRITE_NODE_H

#include <16nar/constructor2d/drawable_node_2d.h>
#include <16nar/constructor2d/render/texture_rect.h>

namespace _16nar::constructor2d
{

/// @brief Node which represents a sprite on the scene.
class ENGINE_API SpriteNode : public DrawableNode2D
{
public:
     /// @brief Constructor.
     /// @param[in] shader shader used to draw this node.
     /// @param[in] texture_rect texture rectangle drawn on the sprite.
     /// @param[in] local_bounds local bounds of sprite.
     SpriteNode( const Shader& shader,
          const TextureRect& texture_rect,
          const FloatRect& local_bounds );

     /// @copydoc Drawable::get_draw_info() const
     DrawInfo get_draw_info() const override;

     /// @copydoc Drawable2D::get_local_bounds() const
     FloatRect get_local_bounds() const;

private:
     TextureRect texture_rect_;    ///< texture rectangle used to draw this node.
     FloatRect local_bounds_;      ///< local bounds of sprite.
     VertexBuffer vertex_buffer_;  ///< vertex buffer used to draw this node.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_SPRITE_NODE_H
