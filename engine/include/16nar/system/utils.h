/// @file
/// @brief File with utility classes definition.
#ifndef _16NAR_SYSTEM_UTILS_H
#define _16NAR_SYSTEM_UTILS_H

#include <functional>
#include <memory>

namespace _16nar
{

/// @brief Utility class for transparent pointer comparison with std::less.
/// @tparam T type of pointed object.
template < typename T >
struct PtrComparator
{
     using is_transparent = std::true_type;

     /// @brief Type for reducing number of overloads with use of implicit constructors.
     struct Helper
     {
          T *ptr;

          Helper() : ptr{ nullptr } {}
          Helper( const Helper& ) = default;
          Helper( T *p ) : ptr{ p } {}

          template < typename U>
          Helper( const std::shared_ptr< U >& sp ) : ptr{ sp.get() } {}

          template< typename U, typename D >
          Helper( const std::unique_ptr< U, D >& up ) : ptr{ up.get() } {}

          bool operator<( Helper o ) const
          {
               return std::less< T * >{}( ptr, o.ptr );
          }
     };

     bool operator()( Helper&& lhs, Helper&& rhs ) const
     {
          return lhs < rhs;
     }
};

} // namespace _16nar

#endif // #define _16NAR_SYSTEM_UTILS_H
