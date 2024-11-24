/// @file
/// @brief Header file with Camera2D class definition.
#ifndef _16NAR_CAMERA_2D_H
#define _16NAR_CAMERA_2D_H

#include <16nar/math/rectangle.h>
#include <16nar/math/transform_matrix.h>

namespace _16nar
{

/// @brief 2D camera, defining a view which looks into the scene.
class ENGINE_API Camera2D
{
public:
     /// @brief Constructor.
     /// @param[in] center position of camera center.
     /// @param[in] width width of camera rectangle.
     /// @param[in] height height of camera rectangle.
     Camera2D( const Vec2f& center, float width, float height ) noexcept;

     /// @brief Get global bounds of view rectangle, including scale and rotation.
     /// @return global bounds of view rectangle.
     const FloatRect& get_global_bounds() const noexcept;

     /// @brief Set center of the view.
     /// @param[in] pos position of new center.
     void set_center( const Vec2f& pos ) noexcept;

     /// @brief Set size of camera rectangle.
     /// @param[in] width width of camera rectangle.
     /// @param[in] height height of camera rectangle.
     void set_size( float width, float height ) noexcept;

     /// @brief Set camera clockwise rotation angle, in degrees.
     /// @param[in] angle rotation angle.
     void set_rotation( float angle ) noexcept;

     /// @brief Set camera zoom, non-pozitive values will be ignored.
     /// @param[in] camera zoom.
     void set_zoom( float zoom ) noexcept;

     /// @brief Reset camera to given parameters.
     /// @param[in] center new camera center.
     /// @param[in] width new camera width.
     /// @param[in] height new camera height.
     void reset( const Vec2f& center, float width, float height ) noexcept;

     /// @brief Get camera center global position.
     /// @return camera center global position.
     const Vec2f& get_center() const noexcept;

     /// @brief Get camera rectangle size.
     /// @return size of camera rectangle.
     Vec2f get_size() const noexcept;

     /// @brief Get current camera clockwise rotation angle, in degrees.
     /// @details Angle is always in [0; 360).
     /// @return camera rotation angle.
     float get_rotation() const noexcept;

     /// @brief Get camera zoom.
     /// @return camera zoom.
     float get_zoom() const noexcept;

     /// @brief Move camera with given offset.
     /// @param[in] offset offset to move camera.
     void move( const Vec2f& offset ) noexcept;

     /// @brief Rotate camera clockwise with given angle, in degrees.
     /// @param[in] angle rotation angle, in degrees.
     void rotate( float angle ) noexcept;

     /// @brief Zoom camera with given factor.
     /// @param[in] factor camera zoom factor.
     void zoom( float factor ) noexcept;

     /// @brief Get camera transformation matrix.
     /// @return camera transformation matrix.
     const TransformMatrix& get_transform_matr() const noexcept;

     /// @brief Get camera inverse transformation matrix.
     /// @return camera inverse transformation matrix.
     TransformMatrix get_inverse_transform_matr() const noexcept;

private:
     /// @brief Do calculations of transofrmation matrix and global bounds.
     void params_calculate() const noexcept;

private:
     mutable TransformMatrix matr_;     ///< transformation matrix of the view.
     mutable FloatRect global_bounds_;  ///< bounding rectangle of the camera in the world.
     Vec2f center_;                     ///< view rectangle of the camera.
     float half_width_;                 ///< half of width of camera rectangle.
     float half_height_;                ///< half of height of camera rectangle.
     float scale_;                      ///< current camera scale (inverse to zoom).
     float rotation_;                   ///< current camera clockwise rotation, in degrees.
     mutable bool transformed_;         ///< flag indicating that matrix must be recalculated.
};

} // namespace _16nar

#endif // #ifndef _16NAR_CAMERA_2D_H
