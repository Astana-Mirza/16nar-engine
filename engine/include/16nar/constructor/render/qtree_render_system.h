/// @file
/// @brief Header file with QTreeRenderSystem class definition.
#ifndef _16NAR_CONSTRUCTOR_QTREE_RENDER_SYSTEM_H
#define _16NAR_CONSTRUCTOR_QTREE_RENDER_SYSTEM_H

#include <unordered_map>

#include <16nar/constructor/render/irender_system_2d.h>
#include <16nar/constructor/render/quadrant.h>

#include <16nar/render/irender_api.h>

namespace _16nar
{

class Window;

} // namespace _16nar


namespace _16nar::constructor
{

class Drawable2D;

/// @brief Render system which uses quadrant tree space partition.
/// @details Space partition is needed to reduce number of checked nodes.
/// The checks are made to find visible nodes which need to be drawn.
/// With space partition the whole scene space is divided into parts (quadrants),
/// and only nodes from visible parts of scene are checked.
/// The check is performed using traverse in quadrant tree, starting from root.
/// The root quadrant covers the whole scene, then it is divided into 4 smaller quadrants,
/// each of them is also divided into 4 quadrants, and so on.
class ENGINE_API QTreeRenderSystem : public IRenderSystem2D
{
public:
     /// @brief Constructor.
     /// @param[in] api API for rendering selected objects.
     /// @param[in] window game window.
     QTreeRenderSystem( IRenderApi& api, Window& window );

     /// @copydoc IRenderSystem::reset()
     virtual void reset() override;

     /// @copydoc IRenderSystem::clear_screen()
     virtual void clear_screen() override;

     /// @copydoc IRenderSystem::select_objects()
     virtual void select_objects() override;

     /// @copydoc IRenderSystem::draw_objects()
     virtual void draw_objects() override;

     /// @copydoc IRenderSystem2D::add_draw_child(Drawable2D*)
     virtual void add_draw_child( Drawable2D *child ) override;

     /// @copydoc IRenderSystem2D::delete_draw_child(Drawable2D*)
     virtual void delete_draw_child( Drawable2D *child ) override;

     /// @copydoc IRenderSystem2D::handle_change(Drawable2D*)
     void handle_change( Drawable2D *child ) override;

     /// @copydoc IRenderSystem2D::set_camera(Camera2D*)
     virtual void set_camera( Camera2D *camera ) override;

     /// @copydoc IRenderSystem2D::get_camera()
     virtual const Camera2D* get_camera() const override;

     /// @copydoc IRenderSystem::get_render_api()
     virtual IRenderApi& get_render_api() override;

     /// @copydoc IRenderSystem::get_window()
     virtual Window& get_window() override;

     /// @brief Set render quadrant for next draw call.
     /// @param[in] root root quadrant for drawing.
     void set_root_quadrant( Quadrant *root );

protected:
     /// @brief Check if the object fits in specified quadrant.
     /// @param[in] obj drawable object.
     /// @param[in] quad quadrant to be checked.
     static bool check_quadrant( Drawable2D *obj, const Quadrant *quad );

private:
     /// @brief Call render API to draw the object.
     /// @param[in] obj object to be drawn.
     void draw_object( Drawable2D *obj );

     /// @brief Set shader parameters of render system.
     void set_shader_params() const;

private:
     std::unordered_map< Drawable2D*, Quadrant* > quad_map_;     ///< map of drawable objects and their quadrants.
     Quadrant::LayerMap layers_;                                 ///< map of layers and selected drawables on them.
     Quadrant *root_;                                            ///< root quadrant, covering the whole scene.
     IRenderApi& api_;                                           ///< API for rendering selected objects.
     Shader current_shader_;                                     ///< currently bound shader.
     Camera2D *camera_;                                          ///< camera of the render system.
     Window& window_;                                            ///< main application window.
};

} // namespace _16nar::constructor

#endif // #ifndef _16NAR_CONSTRUCTOR_QTREE_RENDER_SYSTEM_H
