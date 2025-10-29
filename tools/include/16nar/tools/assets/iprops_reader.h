/// @file
/// @brief Header file with IPropsReader interface definition.
#ifndef _16NAR_TOOLS_IPROPS_READER_H
#define _16NAR_TOOLS_IPROPS_READER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/scene_defs.h>

#include <16nar/math/vec.h>
#include <16nar/math/rectangle.h>

#include <optional>
#include <vector>
#include <string>

namespace _16nar::tools
{

class DataSchema;

/// @brief Interface for reading properties in form of key-value pairs.
class ENGINE_API IPropsReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~IPropsReader() = default;

     /// @brief Check if the reader owns the underlying buffer.
     /// @return true if the reader owns the underlying buffer, false otherwise.
     virtual bool is_owner() const noexcept = 0;

     /// @brief Set data schema for reading the data.
     /// @details Setting of data schema may be required by certain data formats.
     /// @param schema data schema of read data.
     virtual void set_data_schema( const DataSchema& schema ) noexcept = 0;

     // basic types

     /// @brief Get value with given name.
     /// @param[in] name name of the value.
     /// @return value with given name.
     virtual std::optional< uint64_t > get_uint64( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< uint32_t > get_uint32( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< uint16_t > get_uint16( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< uint8_t > get_uint8( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< int64_t > get_int64( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< int32_t > get_int32( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< int16_t > get_int16( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< int8_t > get_int8( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< bool > get_bool( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< float > get_float( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< double > get_double( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< std::string > get_string( const std::string& name ) = 0;

     // array types

     /// @brief Get array of values with given name.
     /// @param[in] name name of the array.
     /// @return array of values with given name.
     virtual std::optional< std::vector< uint64_t > > get_uint64_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< uint32_t > > get_uint32_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< uint16_t > > get_uint16_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< uint8_t > > get_uint8_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< int64_t > > get_int64_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< int32_t > > get_int32_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< int16_t > > get_int16_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< int8_t > > get_int8_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< bool > > get_bool_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< float > > get_float_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< double > > get_double_arr( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     virtual std::optional< std::vector< std::string > > get_string_arr( const std::string& name ) = 0;

     // complex types

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< Vec2f > get_vec2f( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< Vec3f > get_vec3f( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< Vec4f > get_vec4f( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< Vec2i > get_vec2i( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< Vec3i > get_vec3i( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< Vec4i > get_vec4i( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< FloatRect > get_float_rect( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< IntRect > get_int_rect( const std::string& name ) = 0;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     virtual std::optional< ResourceIndex > get_resource_index( const std::string& name ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IPROPS_READER_H
