/// @file
/// @brief Header file with DrawableNode2D base class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_DRAWABLE_NODE_2D_H
#define _16NAR_CONSTRUCTOR_2D_DRAWABLE_NODE_2D_H

#include <16nar/constructor2d/render/drawable_2d.h>
#include <16nar/constructor2d/node_2d.h>

namespace _16nar::constructor2d
{

/// @brief Base class of scene tree node which can be drawn on the screen.
class ENGINE_API DrawableNode2D : public Drawable2D, public Node2D
{
public:
     /// @copydoc Drawable2D::Drawable2D(const Shader&) noexcept
     explicit DrawableNode2D( const Shader& shader ) noexcept;

     /// @copydoc Drawable2D::get_global_bounds() const
     FloatRect get_global_bounds() const override;

     /// @copydoc Node2D::loop_call(SceneState&, float, bool)
     void loop_call( SceneState& state, float delta, bool updated ) override;
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_DRAWABLE_NODE_2D_H
