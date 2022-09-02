/// @file
/// @brief File with DrawableNode abstract class definition.
#ifndef _16NAR_DRAWABLE_NODE_H
#define _16NAR_DRAWABLE_NODE_H

#include <constructor/abstract/node.h>
#include <abstract/drawable.h>
#include <render/quadrant.h>

namespace _16nar
{

/// @brief Abstract base class for nodes that have graphical representation.
class ENGINE_API DrawableNode : public Node, public Drawable
{
public:
     using Drawable::Drawable;

     /// @brief Checks if the object is visible.
     bool is_visible() const;

     /// @brief Sets visibility of the object.
     /// @param visible visibility.
     void set_visible( bool visible );

     /// @brief Gets the scene layer of this object.
     int get_layer() const;

     /// @brief Sets scene layer of this object.
     /// @param layer scene layer.
     void set_layer( int layer );

     /// @brief Sets shader of this object.
     /// @param shader pointer to shader.
     void set_shader( Shader *shader );

     /// @brief Gets shader of this object.
     Shader *get_shader() const;

     /// @brief Sets the blend mode of this object.
     /// @param blend blend mode of this object.
     void set_blend( const BlendMode& blend );

     /// @brief Gets the blend mode of this object.
     const BlendMode& get_blend() const;

protected:
     /// @brief Function that launches update loop recursively.
     /// @param upd value, telling if this branch of scene tree has to be updated.
     /// @param delta time since last update, in seconds.
     void loop_call( bool update, float delta );

private:
     /// @brief Check if this node fits in specified quadrant.
     /// @param quad pointer to quadrant to be checked.
     bool check_quadrant( const Quadrant *quad ) const;

private:
     BlendMode blend_ = BlendAlpha;     ///< mode, defining how pixels of the object will be blended.
     Shader* shader_ = nullptr;         ///< shader to be drawn with the object.
     int layer_ = 0;                    ///< scene layer.
     bool visible_ = true;              ///< visibility.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_NODE_H
