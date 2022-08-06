/// @file
/// File with Quadrant class definition.
#ifndef _16NAR_QUADRANT_H
#define _16NAR_QUADRANT_H

#include <map>
#include <array>
#include <unordered_set>
#include <16nardefs.h>
#include <abstract/drawable.h>

namespace _16nar
{

class Drawable;

/// Class for space partitioning with quadrant tree.
class ENGINE_API Quadrant
{
public:
     /// Constant which represents this quadrant's children count.
     constexpr static size_t quad_count = 4;

     using LayerMap = std::map< int, std::unordered_set< Drawable * > >;
     using QuadArray = std::array< Quadrant *, quad_count >;

     /// Constructor sets area of this quadrant.
     /// @param area area of the quadrant.
     explicit Quadrant( const FloatRect& area = {} );

     /// Destructor deletes all children quadrants recursively.
     ~Quadrant();

     /// Get this quadrant's area.
     const FloatRect& get_area() const;

     /// Get this quadrant's children.
     const QuadArray& get_children() const;

     /// Get this quadrant's parent.
     Quadrant *get_parent() const;

     /// Add a child quadrant at given index.
     /// @param child pointer to child quadrant to be added.
     /// @index index at which child will be added.
     void add_child( Quadrant *child, int index );

     /// Add a drawable object to this quadrant.
     /// @param child pointer to drawable object to be added.
     void add_draw_child( Drawable *child );

     /// Delete a drawable object from this quadrant, memory will not be freed.
     /// @param child pointer to drawable object to be deleted.
     void delete_draw_child( Drawable *child );

     /// Draw all visible objects that intersect with the area of the target.
     /// @param target target where objects should be rendered.
     /// @param states options for rendering.
     void draw( RenderTarget& target, RenderStates& states ) const;

private:
     /// Recursively find objects in this quadrant, which intersect with given area.
     /// @param target target where objects should be rendered.
     /// @param area area for which we look for intersections.
     /// @param layers layers where all found objects will be stored.
     void find_objects( RenderTarget& target, IntRect area, LayerMap& layers ) const;

     LayerMap draw_layers_;        ///< all drawable objects stored with their layers.
     FloatRect area_;              ///< area of this quadrant.
     QuadArray children_;          ///< children of this quadrant.
     Quadrant *parent_;            ///< parent of this quadrant.
};

} // namespace _16nar

#endif // #ifndef _16NAR_QUADRANT_H
