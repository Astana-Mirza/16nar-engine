#include <16nar/tools/assets/flatbuffers/flatbuffers_props_writer.h>

#include <flatbuffers/flexbuffers.h>

namespace _16nar::tools
{

FlatBuffersPropsWriter::FlatBuffersPropsWriter():
     builder_{ new flexbuffers::Builder() }, map_key_{}
{
     map_key_ = builder_->StartMap();
}


FlatBuffersPropsWriter::~FlatBuffersPropsWriter()
{
     delete builder_;
}


const std::vector< uint8_t >& FlatBuffersPropsWriter::finish_and_get_result()
{
     builder_->EndMap( map_key_ );
     builder_->Finish();
     return builder_->GetBuffer();
}


void FlatBuffersPropsWriter::set_uint64( const std::string& name, uint64_t value )
{
     builder_->UInt( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_uint32( const std::string& name, uint32_t value )
{
     builder_->UInt( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_uint16( const std::string& name, uint16_t value )
{
     builder_->UInt( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_uint8( const std::string& name, uint8_t value )
{
     builder_->UInt( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_int64( const std::string& name, int64_t value )
{
     builder_->Int( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_int32( const std::string& name, int32_t value )
{
     builder_->Int( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_int16( const std::string& name, int16_t value )
{
     builder_->Int( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_int8( const std::string& name, int8_t value )
{
     builder_->Int( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_bool( const std::string& name, bool value )
{
     builder_->Bool( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_float( const std::string& name, float value )
{
     builder_->Float( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_double( const std::string& name, double value )
{
     builder_->Double( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_string( const std::string& name, const std::string& value )
{
     builder_->String( name.c_str(), value );
}


void FlatBuffersPropsWriter::set_uint64_arr( const std::string& name, const std::vector< uint64_t >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->UInt( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_uint32_arr( const std::string& name, const std::vector< uint32_t >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->UInt( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_uint16_arr( const std::string& name, const std::vector< uint16_t >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->UInt( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_uint8_arr( const std::string& name, const std::vector< uint8_t >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->UInt( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_int64_arr( const std::string& name, const std::vector< int64_t >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->Int( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_int32_arr( const std::string& name, const std::vector< int32_t >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->Int( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_int16_arr( const std::string& name, const std::vector< int16_t >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->Int( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_int8_arr( const std::string& name, const std::vector< int8_t >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->Int( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_bool_arr( const std::string& name, const std::vector< bool >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->Bool( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_float_arr( const std::string& name, const std::vector< float >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->Float( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_double_arr( const std::string& name, const std::vector< double >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto val : value )
     {
          builder_->Double( val );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_string_arr( const std::string& name, const std::vector< std::string >& value )
{
     auto start = builder_->StartVector( name.c_str() );
     for ( const auto& val : value )
     {
          builder_->Key( val.c_str() );
     }
     builder_->EndVector( start, true, false );
}


void FlatBuffersPropsWriter::set_vec2f( const std::string& name, const Vec2f& value )
{
     builder_->FixedTypedVector( name.c_str(), value.data(), Vec2f::size );
}


void FlatBuffersPropsWriter::set_vec3f( const std::string& name, const Vec3f& value )
{
     builder_->FixedTypedVector( name.c_str(), value.data(), Vec3f::size );
}


void FlatBuffersPropsWriter::set_vec4f( const std::string& name, const Vec4f& value )
{
     builder_->FixedTypedVector( name.c_str(), value.data(), Vec4f::size );
}


void FlatBuffersPropsWriter::set_vec2i( const std::string& name, const Vec2i& value )
{
     builder_->FixedTypedVector( name.c_str(), value.data(), Vec2i::size );
}


void FlatBuffersPropsWriter::set_vec3i( const std::string& name, const Vec3i& value )
{
     builder_->FixedTypedVector( name.c_str(), value.data(), Vec3i::size );
}


void FlatBuffersPropsWriter::set_vec4i( const std::string& name, const Vec4i& value )
{
     builder_->FixedTypedVector( name.c_str(), value.data(), Vec4i::size );
}


void FlatBuffersPropsWriter::set_float_rect( const std::string& name, const FloatRect& value )
{
     typename Vec2f::type valarr[ 2 * Vec2f::size ] = { value.get_pos().x(), value.get_pos().y(),
          value.get_end().x(), value.get_end().y() };
     builder_->FixedTypedVector( name.c_str(), valarr, 2 * Vec2f::size );
}


void FlatBuffersPropsWriter::set_int_rect( const std::string& name, const IntRect& value )
{
     typename Vec2i::type valarr[ 2 * Vec2i::size ] = { value.get_pos().x(), value.get_pos().y(),
          value.get_end().x(), value.get_end().y() };
     builder_->FixedTypedVector( name.c_str(), valarr, 2 * Vec2i::size );
}


void FlatBuffersPropsWriter::set_resource_index( const std::string& name, ResourceIndex value )
{
     uint16_t valarr[ 2 ] = { value.package, value.resource };
     builder_->FixedTypedVector( name.c_str(), valarr, 2 );
}

} // namespace _16nar::tools
