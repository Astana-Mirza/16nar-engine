#ifndef _16NAR_PLATFORM_MATH_RECTANGLE_INL
#define _16NAR_PLATFORM_MATH_RECTANGLE_INL

namespace _16nar::math
{

template < typename T >
bool Rectangle< T >::contains( const Vec< 2, T >& point ) const noexcept
{
     return ( point.x() >= pos_.x() )
         && ( point.y() >= pos_.y() )
         && ( point.x() <= end_.x() )
         && ( point.y() <= end_.y() );
}


template < typename T >
bool Rectangle< T >::intersects( const Rectangle< T >& rect ) const noexcept
{
     return ( pos_.x() <= rect.end_.x() )
         && ( end_.x() >= rect.pos_.x() )
         && ( pos_.y() <= rect.end_.y() )
         && ( end_.y() >= rect.pos_.y() );
}


template < typename T >
bool operator==( const Rectangle< T >& lhs, const Rectangle< T >& rhs ) noexcept
{
     return ( lhs.get_pos() == rhs.get_pos() )
         && ( lhs.get_end() == rhs.get_end() );
}


template < typename T >
bool operator!=( const Rectangle< T >& lhs, const Rectangle< T >& rhs ) noexcept
{
     return !( lhs == rhs );
}

} // namespace _16nar::math

#endif // #ifndef _16NAR_PLATFORM_MATH_RECTANGLE_INL
