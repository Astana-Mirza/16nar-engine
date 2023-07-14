/// @file
/// @brief Header file with QTreeRenderSystem class definition.
#ifndef _16NAR_QTREE_RENDER_SYSTEM_H
#define _16NAR_QTREE_RENDER_SYSTEM_H

#include <unordered_map>

#include <16nar/abstract/render_system.h>
#include <16nar/common/render/quadrant.h>

namespace _16nar
{

/// @brief Render system which uses quadrant tree space partition.
/// @detail Space partition is needed to reduce number of checked nodes.
/// The checks are made to find visible nodes which need to be drawn.
/// With space partition the whole scene space is divided into parts (quadrants),
/// and only nodes from visible parts of scene are checked.
/// The check is performed using traverse in quadrant tree, starting from root.
/// The root quadrant covers the whole scene, then it is divided into 4 smaller quadrants,
/// each of them is also divided into 4 quadrants, and so on.
class ENGINE_API QTreeRenderSystem : public RenderSystem
{
public:
     /// @brief Constructor.
     /// @param[in] root root quadrant.
     QTreeRenderSystem( Quadrant&& root );

     /// @brief Start rendering a frame.
     /// @param[in] view view used to define rendering area.
     /// @param[in] device device used for rendering.
     void start_render( const View& view, RenderDevice& device ) override;

     /// @brief Finish rendering a frame.
     /// @param[in] device device used for rendering.
     void finish_render( RenderDevice& device ) override;

     /// @brief Add a drawable object to this quadrant.
     /// @param[in] child pointer to drawable object to be added.
     void add_draw_child( Drawable *child ) override;

     /// @brief Delete a drawable object from this quadrant, memory will not be freed.
     /// @param[in] child pointer to drawable object to be deleted.
     void delete_draw_child( Drawable *child ) override;

     /// @brief Handle change of object and adjust for future redrawing.
     /// @param[in] child changed object.
     void handle_change( Drawable *child ) override;

     /// @brief Get root quadrant of the tree.
     Quadrant& get_root();

protected:
     /// @brief Check if the object fits in specified quadrant.
     /// @param[in] obj drawable object.
     /// @param[in] quad quadrant to be checked.
     static bool check_quadrant( Drawable *obj, const Quadrant *quad );

private:
     std::unordered_map< Drawable*, Quadrant* > quad_map_;  ///< map of drawable objects and their quadrants.
     Quadrant root_;                                        ///< root quadrant, covering the whole scene.
};

} // namespace _16nar

#endif // #ifndef _16NAR_QTREE_RENDER_SYSTEM_H
