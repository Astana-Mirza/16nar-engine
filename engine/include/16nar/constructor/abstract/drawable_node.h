/// @file
/// @brief File with DrawableNode abstract class definition.
#ifndef _16NAR_DRAWABLE_NODE_H
#define _16NAR_DRAWABLE_NODE_H

#include <16nar/constructor/abstract/node.h>
#include <16nar/abstract/drawable.h>

namespace _16nar
{

/// @brief Abstract base class for nodes that have graphical representation.
class ENGINE_API DrawableNode : public Node, public Drawable
{
public:
     using Drawable::Drawable;

     /// @brief Checks if the object is visible.
     bool is_visible() const override;

     /// @brief Sets visibility of the object.
     /// @param[in] visible visibility.
     void set_visible( bool visible ) override;

     /// @brief Gets the scene layer of this object.
     int get_layer() const override;

     /// @brief Sets scene layer of this object.
     /// @param[in] layer scene layer.
     void set_layer( int layer ) override;

     /// @brief Sets shader of this object.
     /// @param[in] shader pointer to shader.
     void set_shader( Shader *shader ) override;

     /// @brief Gets shader of this object.
     Shader *get_shader() const override;

     /// @brief Sets the blend mode of this object.
     /// @param[in] blend blend mode of this object.
     void set_blend( const BlendMode& blend ) override;

     /// @brief Gets the blend mode of this object.
     const BlendMode& get_blend() const override;

     /// @brief Gets global bounds of an object (in world coordinates).
     FloatRect get_global_bounds() const override;

protected:
     /// @brief Function that launches update loop recursively.
     /// @param[in] upd value, telling if this branch of scene tree has to be updated.
     /// @param[in] delta time since last update, in seconds.
     void loop_call( bool update, float delta ) override;

private:
     BlendMode blend_ = BlendAlpha;     ///< mode, defining how pixels of the object will be blended.
     Shader *shader_ = nullptr;         ///< shader to be drawn with the object.
     int layer_ = 0;                    ///< scene layer.
     bool visible_ = true;              ///< visibility.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_NODE_H
