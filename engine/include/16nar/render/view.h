/// @file
/// @brief Header file with View class definition.
#ifndef _16NAR_VIEW_H
#define _16NAR_VIEW_H

#include <16nar/16nardefs.h>

namespace _16nar
{

/// @brief View, defining a 2D camera which looks into the scene.
class ENGINE_API View
{
public:
     /// @brief Constructor.
     /// @param[in] view_rect view rectangle of the view.
     View( const FloatRect& view_rect );

     /// @brief Get global bounds of view rectangle, including scale and rotation.
     FloatRect get_global_bounds() const;

     void set_center( const Vector2f pos );

     void set_center( float x, float y );
     void set_size( float width, float height );
     void set_size( const Vector2f& size );
     void set_rotation( float angle );
     void set_viewport( const FloatRect& viewport );
     void reset( const FloatRect& rectangle );
     const Vector2f& get_center() const;
     const Vector2f& get_size() const;
     float get_rotation() const;
     const FloatRect& get_viewport() const;
     void move( float offset_x, float offset_y );
     void move( const Vector2f& offset );
     void rotate( float angle );
     void zoom( float factor );
     const TransformMatrix& get_transform_matr() const;
     const TransformMatrix& get_inverse_transform_matr() const;

#ifdef USE_SFML
     /// @brief Implicit cast to SFML view.
     operator sf::View() const { return view_; }
#endif

private:
#ifdef USE_SFML
     sf::View view_;     ///< inner SFML view.
#endif
};

} // namespace _16nar

#endif // #ifndef _16NAR_VIEW_H
