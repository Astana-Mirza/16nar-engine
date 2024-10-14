/// @file
/// @brief File with Quadrant class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_QUADRANT_H
#define _16NAR_CONSTRUCTOR_2D_QUADRANT_H

#include <map>
#include <array>
#include <unordered_set>

#include <16nar/render/render_defs.h>
#include <16nar/math/rectangle.h>

namespace _16nar::constructor2d
{

class Drawable2D;

/// @brief Class for space partitioning with quadrant tree.
/// @details Quadrant represents rectangular area and stores all drawable
/// objects located in this area. Quadrant can be queried to get all its
/// drawable objects which intersect with given rectangle (typically, camera
/// view rectangle).
class ENGINE_API Quadrant
{
public:
     /// @brief Constant which represents this quadrant's children count.
     constexpr static std::size_t quad_count = 4;

     using DrawableSet = std::unordered_set< Drawable2D * >;
     using LayerMap = std::map< int, DrawableSet >;
     using QuadArray = std::array< std::unique_ptr< Quadrant >, quad_count >;

     /// @brief Constructor sets area of this quadrant.
     /// @param[in] area area of the quadrant.
     explicit Quadrant( const FloatRect& area );

     /// @brief Get this quadrant's area.
     /// @return This quadrant's area.
     const FloatRect& get_area() const;

     /// @brief Get this quadrant's children quadrants.
     /// @return This quadrant's children quadrants.
     const QuadArray& get_children() const;

     /// @brief Get all drawable objects in this quadrant.
     /// @return All drawable objects in this quadrant.
     const DrawableSet& get_draw_children() const;

     /// @brief Get this quadrant's parent.
     /// @return This quadrant's parent quadrant.
     Quadrant *get_parent() const;

     /// @brief Add a child quadrant at given index.
     /// @param[in] child pointer to child quadrant to be added.
     /// @param[in] index at which child will be added.
     void add_child( std::unique_ptr< Quadrant >&& child, int index );

     /// @brief Add a drawable object to this quadrant.
     /// @param[in] child pointer to drawable object to be added.
     void add_draw_child( Drawable2D *child );

     /// @brief Delete a drawable object from this quadrant, memory will not be freed.
     /// @param[in] child pointer to drawable object to be deleted.
     void delete_draw_child( Drawable2D *child );

     /// @brief Recursively find objects in this quadrant, which intersect with given area.
     /// @param[in] area area for which we look for intersections.
     /// @param[out] layers layers and sets of all found objects on the layers.
     void find_objects( const FloatRect& area, LayerMap& layers ) const;

private:
     DrawableSet drawables_;       ///< all drawable objects stored with their layers.
     FloatRect area_;              ///< area of this quadrant.
     QuadArray children_;          ///< children of this quadrant.
     Quadrant *parent_;            ///< parent of this quadrant.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_QUADRANT_H
