#include <16nar/tools/assets/json/json_props_writer.h>

#include <array>

namespace _16nar::tools
{

const nlohmann::json& JsonPropsWriter::get_result() const
{
     return json_;
}


void JsonPropsWriter::set_uint64( const std::string& name, uint64_t value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_uint32( const std::string& name, uint32_t value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_uint16( const std::string& name, uint16_t value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_uint8( const std::string& name, uint8_t value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_int64( const std::string& name, int64_t value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_int32( const std::string& name, int32_t value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_int16( const std::string& name, int16_t value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_int8( const std::string& name, int8_t value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_bool( const std::string& name, bool value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_float( const std::string& name, float value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_double( const std::string& name, double value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_string( const std::string& name, const std::string& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_uint64_arr( const std::string& name, const std::vector< uint64_t >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_uint32_arr( const std::string& name, const std::vector< uint32_t >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_uint16_arr( const std::string& name, const std::vector< uint16_t >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_uint8_arr( const std::string& name, const std::vector< uint8_t >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_int64_arr( const std::string& name, const std::vector< int64_t >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_int32_arr( const std::string& name, const std::vector< int32_t >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_int16_arr( const std::string& name, const std::vector< int16_t >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_int8_arr( const std::string& name, const std::vector< int8_t >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_bool_arr( const std::string& name, const std::vector< bool >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_float_arr( const std::string& name, const std::vector< float >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_double_arr( const std::string& name, const std::vector< double >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_string_arr( const std::string& name, const std::vector< std::string >& value )
{
     json_[ name ] = value;
}


void JsonPropsWriter::set_vec2f( const std::string& name, const Vec2f& value )
{
     std::array< Vec2f::type, Vec2f::size > arr{ value.x(), value.y() };
     json_[ name ] = arr;
}


void JsonPropsWriter::set_vec3f( const std::string& name, const Vec3f& value )
{
     std::array< Vec3f::type, Vec3f::size > arr{ value.x(), value.y(), value.z() };
     json_[ name ] = arr;
}


void JsonPropsWriter::set_vec4f( const std::string& name, const Vec4f& value )
{
     std::array< Vec4f::type, Vec4f::size > arr{ value.x(), value.y(), value.z(), value.w() };
     json_[ name ] = arr;
}


void JsonPropsWriter::set_vec2i( const std::string& name, const Vec2i& value )
{
     std::array< Vec2i::type, Vec2i::size > arr{ value.x(), value.y() };
     json_[ name ] = arr;
}


void JsonPropsWriter::set_vec3i( const std::string& name, const Vec3i& value )
{
     std::array< Vec3i::type, Vec3i::size > arr{ value.x(), value.y(), value.z() };
     json_[ name ] = arr;
}


void JsonPropsWriter::set_vec4i( const std::string& name, const Vec4i& value )
{
     std::array< Vec4i::type, Vec4i::size > arr{ value.x(), value.y(), value.z(), value.w() };
     json_[ name ] = arr;
}


void JsonPropsWriter::set_float_rect( const std::string& name, const FloatRect& value )
{
     std::array< Vec4f::type, 2 * Vec2f::size > arr{
          value.get_pos().x(), value.get_pos().y(), value.get_end().x(), value.get_end().y() };
     json_[ name ] = arr;
}


void JsonPropsWriter::set_int_rect( const std::string& name, const IntRect& value )
{
     std::array< Vec4i::type, 2 * Vec2i::size > arr{
          value.get_pos().x(), value.get_pos().y(), value.get_end().x(), value.get_end().y() };
     json_[ name ] = arr;
}


void JsonPropsWriter::set_resource_index( const std::string& name, ResourceIndex value )
{
     std::array< uint16_t, 2 > arr{ value.package, value.resource };
     json_[ name ] = arr;
}

} // namespace _16nar::tools

