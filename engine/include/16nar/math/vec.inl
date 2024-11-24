#ifndef _16NAR_VEC_INL
#define _16NAR_VEC_INL

#include <cmath>

namespace _16nar
{

template < std::size_t N, typename T >
inline bool operator==( const Vec< N, T >& lhs, const Vec< N, T >& rhs ) noexcept
{
     for ( std::size_t i = 0; i < N; i++ )
     {
          if ( lhs[ i ] != rhs[ i ] )
          {
               return false;
          }
     }
     return true;
}


template < std::size_t N, typename T >
inline bool operator!=( const Vec< N, T >& lhs, const Vec< N, T >& rhs ) noexcept
{
     return !( lhs == rhs );
}


template < std::size_t N, typename T >
bool Vec< N, T >::equals( const Vec< N, T >& vec, float precision ) const noexcept
{
     for ( std::size_t i = 0; i < N; i++ )
     {
          if ( std::fabs( ( *this )[ i ] - vec[ i ] ) > precision )
          {
               return false;
          }
     }
     return true;
}


template < std::size_t N, typename T >
inline Vec< N, T > operator+( const Vec< N, T >& lhs, const Vec< N, T >& rhs ) noexcept
{
     auto result = lhs;
     result += rhs;
     return result;
}


template < std::size_t N, typename T >
inline Vec< N, T > operator-( const Vec< N, T >& lhs, const Vec< N, T >& rhs ) noexcept
{
     auto result = lhs;
     result -= rhs;
     return result;
}


template < std::size_t N, typename T >
inline Vec< N, T > operator*( const Vec< N, T >& lhs, T rhs ) noexcept
{
    auto result = lhs;
    result *= rhs;
    return result;
}


template < std::size_t N, typename T >
inline Vec< N, T > operator*( T lhs, const Vec< N, T >& rhs ) noexcept
{
     auto result = rhs;
     result *= lhs;
     return result;
}


} // namespace _16nar

#endif // #ifndef _16NAR_VEC_INL
