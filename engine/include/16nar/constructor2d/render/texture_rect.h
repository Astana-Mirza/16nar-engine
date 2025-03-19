/// @file
/// @brief Header file with TextureRect base class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_TEXTURE_RECT_H
#define _16NAR_CONSTRUCTOR_2D_TEXTURE_RECT_H

#include <16nar/16nardefs.h>
#include <16nar/math/rectangle.h>

namespace _16nar::constructor2d
{

/// @brief Texture and rectangle inside it.
struct ENGINE_API TextureRect
{
     /// @brief Constructor.
     /// @param[in] tex texture.
     /// @param[in] rect rectangle inside texture, in 1x1 space, default is full 1x1 rectangle.
     explicit TextureRect( const Texture& tex,
          const FloatRect& rect = FloatRect{ Vec2f{ 0.0f, 0.0f }, 1.0f, 1.0f } ):
          texture{ tex }, rectangle{ rect } {}

     Texture texture;         ///< texture descriptor.
     FloatRect rectangle;     ///< rectangle inside texture.
};

} // namespace _16nar::constructor2d


#endif // #ifndef _16NAR_CONSTRUCTOR_2D_SPRITE_NODE_H
