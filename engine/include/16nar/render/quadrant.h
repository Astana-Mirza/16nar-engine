/// @file
/// @brief File with Quadrant class definition.
#ifndef _16NAR_QUADRANT_H
#define _16NAR_QUADRANT_H

#include <map>
#include <array>
#include <unordered_set>

#include <16nar/16nardefs.h>
#include <16nar/abstract/drawable.h>

namespace _16nar
{

/// @brief Class for space partitioning with quadrant tree.
class ENGINE_API Quadrant
{
public:
     /// @brief Constant which represents this quadrant's children count.
     constexpr static size_t quad_count = 4;

     using DrawableSet = std::unordered_set< Drawable * >;
     using LayerMap = std::map< int, DrawableSet >;
     using QuadArray = std::array< Quadrant *, quad_count >;

     /// @brief Constructor sets area of this quadrant.
     /// @param[in] area area of the quadrant.
     explicit Quadrant( const FloatRect& area = {} );

     /// @brief Destructor deletes all children quadrants recursively.
     ~Quadrant();

     /// @brief Get this quadrant's area.
     const FloatRect& get_area() const;

     /// @brief Get this quadrant's children.
     const QuadArray& get_children() const;

     /// @brief Get this quadrant's parent.
     Quadrant *get_parent() const;

     /// @brief Add a child quadrant at given index.
     /// @param[in] child pointer to child quadrant to be added.
     /// @param[in] index at which child will be added.
     void add_child( Quadrant *child, int index );

     /// @brief Add a drawable object to this quadrant.
     /// @param[in] child pointer to drawable object to be added.
     void add_draw_child( Drawable *child );

     /// @brief Delete a drawable object from this quadrant, memory will not be freed.
     /// @param[in] child pointer to drawable object to be deleted.
     void delete_draw_child( Drawable *child );

     /// @brief Recursively find objects in this quadrant, which intersect with given area.
     /// @param[in] target target where objects should be rendered.
     /// @param[in] area area for which we look for intersections.
     /// @param[out] layers layers and sets of all found objects on the layers.
     void find_objects( const RenderTarget& target, IntRect area, LayerMap& layers ) const;

private:
     DrawableSet drawables_;       ///< all drawable objects stored with their layers.
     FloatRect area_;              ///< area of this quadrant.
     QuadArray children_;          ///< children of this quadrant.
     Quadrant *parent_;            ///< parent of this quadrant.
};

} // namespace _16nar

#endif // #ifndef _16NAR_QUADRANT_H
