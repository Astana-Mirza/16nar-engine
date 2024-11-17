#ifndef _16NAR_TOOLS_CONVERTOR_UTILS_INL
#define _16NAR_TOOLS_CONVERTOR_UTILS_INL

#include <unordered_map>

#define _16NAR_ENUM_CONVERTOR( ToType, FromType, ... )                     \
ToType convert_enum( FromType value )                                      \
{                                                                          \
     const static std::unordered_map< FromType, ToType > conv_map          \
     { __VA_ARGS__ } ;                                                     \
     auto iter = conv_map.find( value );                                   \
     if ( iter == conv_map.cend() )                                        \
     {                                                                     \
          throw std::runtime_error{ "wrong value of " #FromType ": "       \
               + std::to_string( static_cast< std::size_t >( value ) ) };  \
     }                                                                     \
     return iter->second;                                                  \
}

#endif // #ifndef _16NAR_TOOLS_CONVERTOR_UTILS_INL
