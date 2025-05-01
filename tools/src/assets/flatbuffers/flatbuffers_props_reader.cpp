#include <16nar/tools/assets/flatbuffers/flatbuffers_props_reader.h>

#include <flatbuffers/flexbuffers.h>

#define _16NAR_GET_FB_VAL( CHECK, GET ) \
     auto ref = ( *root_ )[ name ];     \
     if ( !ref.CHECK() )                \
     {                                  \
          return {};                    \
     }                                  \
     return ref.GET();


#define _16NAR_GET_FB_TYPED_VECTOR( CPP_TYPE, TYPE, GET )   \
     auto ref = ( *root_ )[ name ];                         \
     if ( !ref.IsTypedVector() )                            \
     {                                                      \
          return {};                                        \
     }                                                      \
     auto vec = ref.AsTypedVector();                        \
     if ( vec.ElementType() != flexbuffers::Type::TYPE )    \
     {                                                      \
          return {};                                        \
     }                                                      \
     std::vector< CPP_TYPE > result;                        \
     result.reserve( vec.size() );                          \
     for ( std::size_t i = 0; i < vec.size(); ++i )         \
     {                                                      \
          result.push_back( vec[ i ].GET() );               \
     }                                                      \
     return result;


#define _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( TYPE, SIZE ) \
     auto ref = ( *root_ )[ name ];                         \
     if ( !ref.IsFixedTypedVector() )                       \
     {                                                      \
          return {};                                        \
     }                                                      \
     auto vec = ref.AsFixedTypedVector();                   \
     if ( vec.ElementType() != flexbuffers::Type::TYPE      \
          || vec.size() != SIZE )                           \
     {                                                      \
          return {};                                        \
     }


namespace _16nar::tools
{

FlatBuffersPropsReader::FlatBuffersPropsReader( const std::byte *buffer, std::size_t size, bool own ):
     buffer_{}, root_{ new flexbuffers::Map( flexbuffers::Map::EmptyMap() ) }
{
     if ( own )
     {
          buffer_.assign( buffer, buffer + size );
          *root_ = flexbuffers::GetRoot( reinterpret_cast< const uint8_t * >( buffer_.data() ), buffer_.size() ).AsMap();
     }
     else
     {
          *root_ = flexbuffers::GetRoot( reinterpret_cast< const uint8_t * >( buffer ), size ).AsMap();
     }
}


FlatBuffersPropsReader::~FlatBuffersPropsReader()
{
     delete root_;
}


bool FlatBuffersPropsReader::is_owner() const noexcept
{
     return !buffer_.empty();
}


std::optional< uint64_t > FlatBuffersPropsReader::get_uint64( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsUInt, AsUInt64 );
}


std::optional< uint32_t > FlatBuffersPropsReader::get_uint32( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsUInt, AsUInt32 );
}


std::optional< uint16_t > FlatBuffersPropsReader::get_uint16( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsUInt, AsUInt16 );
}


std::optional< uint8_t > FlatBuffersPropsReader::get_uint8( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsUInt, AsUInt8 );
}


std::optional< int64_t > FlatBuffersPropsReader::get_int64( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsInt, AsInt64 );
}


std::optional< int32_t > FlatBuffersPropsReader::get_int32( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsInt, AsInt32 );
}


std::optional< int16_t > FlatBuffersPropsReader::get_int16( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsInt, AsInt16 );
}


std::optional< int8_t > FlatBuffersPropsReader::get_int8( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsInt, AsInt8 );
}


std::optional< bool > FlatBuffersPropsReader::get_bool( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsBool, AsBool );
}


std::optional< float > FlatBuffersPropsReader::get_float( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsFloat, AsFloat );
}


std::optional< double > FlatBuffersPropsReader::get_double( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsFloat, AsDouble );
}


std::optional< std::string > FlatBuffersPropsReader::get_string( const std::string& name )
{
     _16NAR_GET_FB_VAL( IsString, AsKey );
}


std::optional< std::vector< uint64_t > > FlatBuffersPropsReader::get_uint64_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( uint64_t, FBT_UINT, AsUInt64 );
}


std::optional< std::vector< uint32_t > > FlatBuffersPropsReader::get_uint32_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( uint32_t, FBT_UINT, AsUInt32 );
}


std::optional< std::vector< uint16_t > > FlatBuffersPropsReader::get_uint16_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( uint16_t, FBT_UINT, AsUInt16 );
}


std::optional< std::vector< uint8_t > > FlatBuffersPropsReader::get_uint8_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( uint8_t, FBT_UINT, AsUInt8 );
}


std::optional< std::vector< int64_t > > FlatBuffersPropsReader::get_int64_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( int64_t, FBT_INT, AsInt64 );
}


std::optional< std::vector< int32_t > > FlatBuffersPropsReader::get_int32_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( int32_t, FBT_INT, AsInt32 );
}


std::optional< std::vector< int16_t > > FlatBuffersPropsReader::get_int16_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( int16_t, FBT_INT, AsInt16 );
}


std::optional< std::vector< int8_t > > FlatBuffersPropsReader::get_int8_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( int8_t, FBT_INT, AsInt8 );
}


std::optional< std::vector< bool > > FlatBuffersPropsReader::get_bool_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( bool, FBT_BOOL, AsBool );
}


std::optional< std::vector< float > > FlatBuffersPropsReader::get_float_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( float, FBT_FLOAT, AsFloat );
}


std::optional< std::vector< double > > FlatBuffersPropsReader::get_double_arr( const std::string& name )
{
     _16NAR_GET_FB_TYPED_VECTOR( double, FBT_FLOAT, AsDouble );
}


std::optional< std::vector< std::string > > FlatBuffersPropsReader::get_string_arr( const std::string& name )
{
     // String vector is deprecated in flatbuffers, so use keys, which is equivalent.
     _16NAR_GET_FB_TYPED_VECTOR( std::string, FBT_KEY, AsKey );
}


std::optional< Vec2f > FlatBuffersPropsReader::get_vec2f( const std::string& name )
{
     _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( FBT_FLOAT, Vec2f::size );
     return Vec2f{ vec[ 0 ].AsFloat(), vec[ 1 ].AsFloat() };
}


std::optional< Vec3f > FlatBuffersPropsReader::get_vec3f( const std::string& name )
{
     _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( FBT_FLOAT, Vec3f::size );
     return Vec3f{ vec[ 0 ].AsFloat(), vec[ 1 ].AsFloat(), vec[ 2 ].AsFloat() };
}


std::optional< Vec4f > FlatBuffersPropsReader::get_vec4f( const std::string& name )
{
     _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( FBT_FLOAT, Vec4f::size );
     return Vec4f{ vec[ 0 ].AsFloat(), vec[ 1 ].AsFloat(), vec[ 2 ].AsFloat(), vec[ 3 ].AsFloat() };
}


std::optional< Vec2i > FlatBuffersPropsReader::get_vec2i( const std::string& name )
{
     _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( FBT_INT, Vec2i::size );
     return Vec2i{ vec[ 0 ].AsInt32(), vec[ 1 ].AsInt32() };
}


std::optional< Vec3i > FlatBuffersPropsReader::get_vec3i( const std::string& name )
{
     _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( FBT_INT, Vec3i::size );
     return Vec3i{ vec[ 0 ].AsInt32(), vec[ 1 ].AsInt32(), vec[ 2 ].AsInt32() };
}


std::optional< Vec4i > FlatBuffersPropsReader::get_vec4i( const std::string& name )
{
     _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( FBT_INT, Vec4i::size );
     return Vec4i{ vec[ 0 ].AsInt32(), vec[ 1 ].AsInt32(), vec[ 2 ].AsInt32(), vec[ 3 ].AsInt32() };
}


std::optional< FloatRect > FlatBuffersPropsReader::get_float_rect( const std::string& name )
{
     _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( FBT_FLOAT, 2 * Vec2f::size );
     return FloatRect{ Vec2f{ vec[ 0 ].AsFloat(), vec[ 1 ].AsFloat() },
          Vec2f{ vec[ 2 ].AsFloat(), vec[ 3 ].AsFloat() } };
}


std::optional< IntRect > FlatBuffersPropsReader::get_int_rect( const std::string& name )
{
     _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( FBT_INT, 2 * Vec2i::size );
     return IntRect{ Vec2i{ vec[ 0 ].AsInt32(), vec[ 1 ].AsInt32() },
          Vec2i{ vec[ 2 ].AsInt32(), vec[ 3 ].AsInt32() } };
}


std::optional< ResourceIndex > FlatBuffersPropsReader::get_resource_index( const std::string& name )
{
     _16NAR_GET_FB_DECL_FIXED_TYPED_VECTOR( FBT_UINT, Vec2i::size );
     return ResourceIndex{ vec[ 0 ].AsUInt16(), vec[ 1 ].AsUInt16() };
}

} // namespace _16nar::tools
