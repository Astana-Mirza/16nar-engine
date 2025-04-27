/// @file
/// @brief Header file with FlatBuffersPropsWriter class definition.
#ifndef _16NAR_TOOLS_FLATBUFFERS_PROPS_WRITER_H
#define _16NAR_TOOLS_FLATBUFFERS_PROPS_WRITER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/assets/iprops_writer.h>

#include <flatbuffers/flexbuffers.h>

namespace _16nar::tools
{

/// @brief Class for writing key-value properties in flatbuffers format.
class ENGINE_API FlatBuffersPropsWriter : public IPropsWriter
{
public:
     /// @brief Constructor.
     FlatBuffersPropsWriter();

     /// @brief Finish writing and return result buffer.
     /// @details Must be called only once for an object. Further calls will raise flatbuffers error.
     /// @return written buffer.
     const std::vector< uint8_t >& finish_and_get_result();

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_uint64( const std::string& name, uint64_t value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_uint32( const std::string& name, uint32_t value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_uint16( const std::string& name, uint16_t value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_uint8( const std::string& name, uint8_t value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_int64( const std::string& name, int64_t value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_int32( const std::string& name, int32_t value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_int16( const std::string& name, int16_t value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_int8( const std::string& name, int8_t value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_bool( const std::string& name, bool value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_float( const std::string& name, float value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_double( const std::string& name, double value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_string( const std::string& name, const std::string& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_uint64_arr( const std::string& name, const std::vector< uint64_t >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_uint32_arr( const std::string& name, const std::vector< uint32_t >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_uint16_arr( const std::string& name, const std::vector< uint16_t >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_uint8_arr( const std::string& name, const std::vector< uint8_t >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_int64_arr( const std::string& name, const std::vector< int64_t >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_int32_arr( const std::string& name, const std::vector< int32_t >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_int16_arr( const std::string& name, const std::vector< int16_t >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_int8_arr( const std::string& name, const std::vector< int8_t >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_bool_arr( const std::string& name, const std::vector< bool >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_float_arr( const std::string& name, const std::vector< float >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_double_arr( const std::string& name, const std::vector< double >& value ) override;

     /// @copydoc IPropsWriter::set_uint64_arr(const std::string&, const std::vector< uint64_t >&)
     void set_string_arr( const std::string& name, const std::vector< std::string >& value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_vec2f( const std::string& name, const Vec2f& value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_vec3f( const std::string& name, const Vec3f& value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_vec4f( const std::string& name, const Vec4f& value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_vec2i( const std::string& name, const Vec2i& value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_vec3i( const std::string& name, const Vec3i& value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_vec4i( const std::string& name, const Vec4i& value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_float_rect( const std::string& name, const FloatRect& value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_int_rect( const std::string& name, const IntRect& value ) override;

     /// @copydoc IPropsWriter::set_uint64(const std::string&, uint64_t)
     void set_resource_index( const std::string& name, ResourceIndex value ) override;

private:
     flexbuffers::Builder builder_;     ///< builder of the buffer.
     std::size_t map_key_;              ///< key of builded map.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_FLATBUFFERS_PROPS_WRITER_H

