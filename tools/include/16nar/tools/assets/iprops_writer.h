/// @file
/// @brief Header file with IPropsWriter interface definition.
#ifndef _16NAR_TOOLS_IPROPS_WRITER_H
#define _16NAR_TOOLS_IPROPS_WRITER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/data_schema.h>

#include <16nar/math/vec.h>
#include <16nar/math/rectangle.h>

#include <string>

namespace _16nar::tools
{

/// @brief Interface for writing properties in form of key-value pairs.
class ENGINE_API IPropsWriter
{
public:
     /// @brief Virtual default destructor.
     virtual ~IPropsWriter() = default;

     /// @brief Create reader with all written data, reset current object state to default.
     /// @return reader with all written data.
     virtual std::shared_ptr< IPropsReader > conver_to_reader() = 0;

     // basic types

     /// @brief Write value with given name.
     /// @param[in] name name of the value.
     /// @param[in] value value to be written.
     virtual void set_uint64( const std::string& name, uint64_t value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_uint32( const std::string& name, uint32_t value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_uint16( const std::string& name, uint16_t value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_uint8( const std::string& name, uint8_t value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_int64( const std::string& name, int64_t value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_int32( const std::string& name, int32_t value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_int16( const std::string& name, int16_t value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_int8( const std::string& name, int8_t value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_bool( const std::string& name, bool value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_float( const std::string& name, float value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_double( const std::string& name, double value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_string( const std::string& name, const std::string& value ) = 0;

     // array types

     /// @brief Write array of values with given name.
     /// @param[in] name name of the array.
     /// @param[in] value array of values with given name.
     virtual void set_uint64_arr( const std::string& name, const std::vector< uint64_t >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_uint32_arr( const std::string& name, const std::vector< uint32_t >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_uint16_arr( const std::string& name, const std::vector< uint16_t >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_uint8_arr( const std::string& name, const std::vector< uint8_t >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_int64_arr( const std::string& name, const std::vector< int64_t >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_int32_arr( const std::string& name, const std::vector< int32_t >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_int16_arr( const std::string& name, const std::vector< int16_t >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_int8_arr( const std::string& name, const std::vector< int8_t >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_bool_arr( const std::string& name, const std::vector< bool >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_float_arr( const std::string& name, const std::vector< float >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_double_arr( const std::string& name, const std::vector< double >& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     virtual void set_string_arr( const std::string& name, const std::vector< std::string >& value ) = 0;

     // complex types

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_vec2f( const std::string& name, const Vec2f& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_vec3f( const std::string& name, const Vec3f& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_vec4f( const std::string& name, const Vec4f& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_vec2i( const std::string& name, const Vec2i& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_vec3i( const std::string& name, const Vec3i& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_vec4i( const std::string& name, const Vec4i& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_float_rect( const std::string& name, const FloatRect& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_int_rect( const std::string& name, const IntRect& value ) = 0;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     virtual void set_resource_index( const std::string& name, ResourceIndex value ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IPROPS_WRITER_H
