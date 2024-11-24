#ifndef _16NAR_RECTANGLE_INL
#define _16NAR_RECTANGLE_INL

namespace _16nar
{

template < typename T >
bool Rectangle< T >::contains( const Vec< 2, T >& point ) const noexcept
{
     return ( point.x() >= pos_.x() )
         && ( point.y() >= pos_.y() )
         && ( point.x() <= pos_.x() + width_ )
         && ( point.y() <= pos_.y() + height_ );
}


template < typename T >
bool Rectangle< T >::intersects( const Rectangle< T >& rect ) const noexcept
{
     return ( pos_.x() <= rect.pos_.x() + rect.width_ )
         && ( pos_.x() + width_ >= rect.pos_.x() )
         && ( pos_.y() <= rect.pos_.y() + rect.height_ )
         && ( pos_.y() + height_ >= rect.pos_.y() );
}


template < typename T >
bool operator==( const Rectangle< T >& lhs, const Rectangle< T >& rhs ) noexcept
{
     return ( lhs.get_pos() == rhs.get_pos() )
         && ( lhs.get_width() == rhs.get_width() )
         && ( lhs.get_height() == rhs.get_height() );
}


template < typename T >
bool operator!=( const Rectangle< T >& lhs, const Rectangle< T >& rhs ) noexcept
{
     return !( lhs == rhs );
}

} // namespace _16nar

#endif // #ifndef _16NAR_RECTANGLE_INL
