/// @file
/// File with DrawableNode abstract class definition.
#ifndef _16NAR_DRAWABLE_NODE_H
#define _16NAR_DRAWABLE_NODE_H

#include <constructor/abstract/node.h>
#include <abstract/drawable.h>
#include <render/quadrant.h>

namespace _16nar
{

/// Abstract base class for nodes that have graphical representation.
/// Every drawable object must be in minimal possible quadrant in
/// scene's quadrant tree. The quadrant tree is used for space
/// partitioning to make render faster, because only visible
/// quadrants are drawn.
class DrawableNode : public Node, public Drawable
{
public:
     /// Constructor which links node to quadrant.
     /// @param quad pointer to quadrant of this node.
     DrawableNode( Quadrant *quad );

     /// Destructor which removes node from quadrant.
     ~DrawableNode();

     /// Sets this node to smallest quadrant, in which it fits.
     void fix_quadrant();

protected:
     /// Function that launches update loop recursively.
     /// @param upd value, telling if this branch of scene tree has to be updated.
     /// @param delta time since last update, in seconds.
     void loop_call( bool update, float delta );

     /// Check if this node fits in specified quadrant.
     /// @param quad pointer to quadrant to be checked.
     virtual bool check_quadrant( const Quadrant *quad ) const = 0;

     Quadrant *quad_;         ///< pointer to quadrant, in which this node is located.
};

} // namespace _16nar

#endif // #ifndef _16NAR_DRAWABLE_NODE_H