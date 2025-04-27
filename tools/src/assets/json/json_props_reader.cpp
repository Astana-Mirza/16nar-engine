#include <16nar/tools/assets/json/json_props_reader.h>

#include <stdexcept>
#include <array>

#define _16NAR_JSON_GET( TYPE )              \
     const auto iter = json_->find( name );  \
     if ( iter == json_->cend() )            \
     {                                       \
          return {};                         \
     }                                       \
     try                                     \
     {                                       \
          return iter->get< TYPE >();        \
     }                                       \
     catch ( const std::exception& ) {}      \
     return {};


#define _16NAR_JSON_PROCESS( TYPE, FUNC )    \
     const auto iter = json_->find( name );  \
     if ( iter == json_->cend() )            \
     {                                       \
          return {};                         \
     }                                       \
     try                                     \
     {                                       \
          auto val = iter->get< TYPE >();    \
          FUNC;                              \
     }                                       \
     catch ( const std::exception& ) {}      \
     return {};


namespace _16nar::tools
{

JsonPropsReader::JsonPropsReader( const nlohmann::json& json, bool own ):
     own_json_{}, json_{ own ? &own_json_ : &json }
{
     if ( own )
     {
          own_json_ = json;
     }
}


bool JsonPropsReader::is_owner() const noexcept
{
     return json_ == &own_json_;
}


std::optional< uint64_t > JsonPropsReader::get_uint64( const std::string& name )
{
     _16NAR_JSON_GET( uint64_t );
}


std::optional< uint32_t > JsonPropsReader::get_uint32( const std::string& name )
{
     _16NAR_JSON_GET( uint32_t );
}


std::optional< uint16_t > JsonPropsReader::get_uint16( const std::string& name )
{
     _16NAR_JSON_GET( uint16_t );
}


std::optional< uint8_t > JsonPropsReader::get_uint8( const std::string& name )
{
     _16NAR_JSON_GET( uint8_t );
}


std::optional< int64_t > JsonPropsReader::get_int64( const std::string& name )
{
     _16NAR_JSON_GET( int64_t );
}


std::optional< int32_t > JsonPropsReader::get_int32( const std::string& name )
{
     _16NAR_JSON_GET( int32_t );
}


std::optional< int16_t > JsonPropsReader::get_int16( const std::string& name )
{
     _16NAR_JSON_GET( int16_t );
}


std::optional< int8_t > JsonPropsReader::get_int8( const std::string& name )
{
     _16NAR_JSON_GET( int8_t );
}


std::optional< bool > JsonPropsReader::get_bool( const std::string& name )
{
     _16NAR_JSON_GET( bool );
}


std::optional< float > JsonPropsReader::get_float( const std::string& name )
{
     _16NAR_JSON_GET( float );
}


std::optional< double > JsonPropsReader::get_double( const std::string& name )
{
     _16NAR_JSON_GET( double );
}


std::optional< std::string > JsonPropsReader::get_string( const std::string& name )
{
     _16NAR_JSON_GET( std::string );
}


std::optional< std::vector< uint64_t > > JsonPropsReader::get_uint64_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< uint64_t > );
}


std::optional< std::vector< uint32_t > > JsonPropsReader::get_uint32_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< uint32_t > );
}


std::optional< std::vector< uint16_t > > JsonPropsReader::get_uint16_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< uint16_t > );
}


std::optional< std::vector< uint8_t > > JsonPropsReader::get_uint8_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< uint8_t > );
}


std::optional< std::vector< int64_t > > JsonPropsReader::get_int64_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< int64_t > );
}


std::optional< std::vector< int32_t > > JsonPropsReader::get_int32_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< int32_t > );
}


std::optional< std::vector< int16_t > > JsonPropsReader::get_int16_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< int16_t > );
}


std::optional< std::vector< int8_t > > JsonPropsReader::get_int8_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< int8_t > );
}


std::optional< std::vector< bool > > JsonPropsReader::get_bool_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< bool > );
}


std::optional< std::vector< float > > JsonPropsReader::get_float_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< float > );
}


std::optional< std::vector< double > > JsonPropsReader::get_double_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< double > );
}


std::optional< std::vector< std::string > > JsonPropsReader::get_string_arr( const std::string& name )
{
     _16NAR_JSON_GET( std::vector< std::string > );
}


std::optional< Vec2f > JsonPropsReader::get_vec2f( const std::string& name )
{
     using JsonType = std::array< Vec2f::type, Vec2f::size >;
     _16NAR_JSON_PROCESS( JsonType,
     {
          return Vec2f( val[ 0 ], val[ 1 ] );
     } );
}


std::optional< Vec3f > JsonPropsReader::get_vec3f( const std::string& name )
{
     using JsonType = std::array< Vec3f::type, Vec3f::size >;
     _16NAR_JSON_PROCESS( JsonType,
     {
          return Vec3f( val[ 0 ], val[ 1 ], val[ 2 ] );
     } );
}


std::optional< Vec4f > JsonPropsReader::get_vec4f( const std::string& name )
{
     using JsonType = std::array< Vec4f::type, Vec4f::size >;
     _16NAR_JSON_PROCESS( JsonType,
     {
          return Vec4f( val[ 0 ], val[ 1 ], val[ 2 ], val[ 3 ] );
     } );
}


std::optional< Vec2i > JsonPropsReader::get_vec2i( const std::string& name )
{
     using JsonType = std::array< Vec2i::type, Vec2i::size >;
     _16NAR_JSON_PROCESS( JsonType,
     {
          return Vec2i( val[ 0 ], val[ 1 ] );
     } );
}


std::optional< Vec3i > JsonPropsReader::get_vec3i( const std::string& name )
{
     using JsonType = std::array< Vec3i::type, Vec3i::size >;
     _16NAR_JSON_PROCESS( JsonType,
     {
          return Vec3i( val[ 0 ], val[ 1 ], val[ 2 ] );
     } );
}


std::optional< Vec4i > JsonPropsReader::get_vec4i( const std::string& name )
{
     using JsonType = std::array< Vec4i::type, Vec4i::size >;
     _16NAR_JSON_PROCESS( JsonType,
     {
          return Vec4i( val[ 0 ], val[ 1 ], val[ 2 ], val[ 3 ] );
     } );
}


std::optional< FloatRect > JsonPropsReader::get_float_rect( const std::string& name )
{
     using JsonType = std::array< Vec2f::type, 2 * Vec2f::size >;
     _16NAR_JSON_PROCESS( JsonType,
     {
          return FloatRect( Vec2f( val[ 0 ], val[ 1 ] ), Vec2f( val[ 2 ], val[ 3 ] ) );
     } );
}


std::optional< IntRect > JsonPropsReader::get_int_rect( const std::string& name )
{
     using JsonType = std::array< Vec2i::type, 2 * Vec2i::size >;
     _16NAR_JSON_PROCESS( JsonType,
     {
          return IntRect( Vec2i( val[ 0 ], val[ 1 ] ), Vec2i( val[ 2 ], val[ 3 ] ) );
     } );
}


std::optional< ResourceIndex > JsonPropsReader::get_resource_index( const std::string& name )
{
     using JsonType = std::array< uint16_t, 2 >;
     _16NAR_JSON_PROCESS( JsonType,
     {
          ResourceIndex index{};
          index.package = val[ 0 ];
          index.resource = val[ 1 ];
          return index;
     } );
}

} // namespace _16nar::tools
