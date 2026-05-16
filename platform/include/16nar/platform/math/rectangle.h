/// @file Header file with Rectangle template class declaration.
#ifndef _16NAR_PLATFORM_MATH_RECTANGLE_H
#define _16NAR_PLATFORM_MATH_RECTANGLE_H

#include <16nar/platform/math/vec.h>

namespace _16nar::math
{

/// @brief Simple rectangle.
/// @tparam T type of vertex coordinates.
template < typename T >
class Rectangle
{
public:
     /// @brief Constructor.
     /// @param[in] pos position of the rectangle.
     /// @param[in] width witdh of the rectangle.
     /// @param[in] height height of the rectangle.
     Rectangle( const Vec< 2, T >& pos, T width, T height ) noexcept:
          pos_{ pos }, end_{ pos.x() + width, pos.y() + height } {}

     /// @brief Constructor.
     /// @param[in] pos position of the rectangle.
     /// @param[in] end position of the end of the rectangle.
     Rectangle( const Vec< 2, T >& pos, const Vec< 2, T >& end ) noexcept:
          pos_{ pos }, end_{ end } {}

     /// @brief Get position of the start of the rectangle.
     /// @return position of the rectangle.
     inline const Vec< 2, T >& get_pos() const noexcept { return pos_; }

     /// @brief Get position of the end of the rectangle.
     /// @return position of the end of the rectangle.
     inline const Vec< 2, T >& get_end() const noexcept { return end_; }

     /// @brief Get width of the rectangle.
     /// @return width of the rectangle.
     inline T get_width() const noexcept { return end_.x() - pos_.x(); }

     /// @brief Get height of the rectangle.
     /// @return height of the rectangle.
     inline T get_height() const noexcept { return end_.y() - pos_.y(); }

     /// @brief Check if rectangle contains point.
     /// @param[in] point point in space.
     /// @return true if rectangle contains point, false otherwise.
     bool contains( const Vec< 2, T >& point ) const noexcept;

     /// @brief Check if rectangle intersects with another one.
     /// @param[in] rect rectangle to check intersection.
     /// @return true if rectangles intersect, false otherwise.
     bool intersects( const Rectangle& rect ) const noexcept;

private:
     Vec< 2, T > pos_;             ///< position of the starting corner of the rectangle.
     Vec< 2, T > end_;             ///< position of the ending corner of the rectangle.
};


/// @brief Comprasion operator for two rectangles.
/// @tparam T type of rectangles' coordinates.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return true if rectangles are equal, false otherwise.
template < typename T >
bool operator==( const Rectangle< T >& lhs, const Rectangle< T >& rhs ) noexcept;


/// @brief Inverse comprasion operator for two rectangles.
/// @tparam T type of rectangles' coordinates.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
/// @return true if rectangles are not equal, false otherwise.
template < typename T >
bool operator!=( const Rectangle< T >& lhs, const Rectangle< T >& rhs ) noexcept;


using FloatRect = Rectangle< float >;
using IntRect = Rectangle< int >;

} // namespace _16nar::math

#include <16nar/platform/math/rectangle.inl>

#endif // #ifndef _16NAR_PLATFORM_MATH_RECTANGLE_H
