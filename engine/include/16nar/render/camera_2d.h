/// @file
/// @brief Header file with Camera2D class definition.
#ifndef _16NAR_CAMERA_2D_H
#define _16NAR_CAMERA_2D_H

#include <16nar/math/rectangle.h>

namespace _16nar
{

/// @brief 2D camera, defining a view which looks into the scene.
class ENGINE_API Camera2D : public Camera
{
public:
     /// @brief Constructor.
     /// @param[in] view_rect view rectangle of the view.
     Camera2D( const FloatRect& view_rect );

     /// @brief Get global bounds of view rectangle, including scale and rotation.
     FloatRect get_view_bounds() const;

     /// @brief Set center of the view.
     /// @param[in] pos position of new center.
     void set_center( const Vec2f& pos );

     void set_center( float x, float y );
     void set_size( float width, float height );
     void set_size( const Vec2f& size );
     void set_rotation( float angle );
     void set_viewport( const FloatRect& viewport );
     void reset( const FloatRect& rectangle );
     const Vec2f& get_center() const;
     const Vec2f& get_size() const;
     float get_rotation() const;
     const FloatRect& get_viewport() const;
     void move( float offset_x, float offset_y );
     void move( const Vec2f& offset );
     void rotate( float angle );
     void zoom( float factor );
     const TransformMatrix& get_transform_matr() const override;
     const TransformMatrix& get_inverse_transform_matr() const override;

private:
     TransformMatrix matr_;        ///< transformation matrix of the view.
     FloatRect view_rect_;         ///< view rectangle of the camera.
};

} // namespace _16nar

#endif // #ifndef _16NAR_CAMERA_2D_H
