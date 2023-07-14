/// @file Header file with Rectangle template class declaration.
#ifndef _16NAR_RECTANGLE_H
#define _16NAR_RECTANGLE_H

#include <16nar/common/16nardefs.h>
#include <16nar/common/math/vec.h>

namespace _16nar
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
     Rectangle( const Vec< 2, T >& pos, T width, T height ):
          pos_{ pos }, width_{ width }, height_{ height } {}

     /// @brief Copy constructor.
     /// @param[in] other rectangle to be copied.
     Rectangle( const Rectangle& other ):
          pos_{ other.pos_ }, width_{ other.width_ }, height_{ other.height_ } {}

     /// @brief Get position of the rectangle.
     inline const Vec< 2, T >& get_pos() const { return pos_; }

     /// @brief Get width of the rectangle.
     inline T get_width() const { return width_; }

     /// @brief Get height of the rectangle.
     inline T get_height() const { return height_; }

     /// @brief Check if rectangle contains point.
     /// @param[in] point point in space.
     bool contains( const Vec< 2, T >& point ) const;

     /// @brief Check if rectangle intersects with another one.
     /// @param[in] rect rectangle to check intersection.
     bool intersects( const Rectangle& rect ) const;

private:
     Vec< 2, T > pos_;             ///< position of the rectangle.
     T width_;                     ///< witdh of the rectangle.
     T height_;                    ///< height of the rectangle.
};


/// @brief Comprasion operator for two rectangles.
/// @tparam T type of rectangles' coordinates.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
template < typename T >
bool operator==( const Rectangle< T >& lhs, const Rectangle< T >& rhs );


/// @brief Inverse comprasion operator for two rectangles.
/// @tparam T type of rectangles' coordinates.
/// @param[in] lhs left operand.
/// @param[in] rhs right operand.
template < typename T >
bool operator!=( const Rectangle< T >& lhs, const Rectangle< T >& rhs );


using FloatRect = Rectangle< float >;
using IntRect = Rectangle< int >;

} // namespace _16nar

#include "rectangle.inl"

#endif // #ifndef _16NAR_RECTANGLE_H
