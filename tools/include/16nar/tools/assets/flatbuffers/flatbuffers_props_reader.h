/// @file
/// @brief Header file with FlatBuffersPropsReader class definition.
#ifndef _16NAR_TOOLS_FLATBUFFERS_PROPS_READER_H
#define _16NAR_TOOLS_FLATBUFFERS_PROPS_READER_H

#include <16nar/16nardefs.h>
#include <16nar/tools/assets/iprops_reader.h>

namespace flexbuffers
{

class Map;

} // namespace flexbuffers


namespace _16nar::tools
{

/// @brief Class for reading key-value properties in flatbuffers format.
class ENGINE_API FlatBuffersPropsReader : public IPropsReader
{
public:
     /// @brief Constructor.
     /// @param[in] buffer underlying buffer.
     /// @param[in] size size of the buffer.
     /// @param[in] own true if this object owns the buffer (the buffer will be copied then), false otherwise.
     FlatBuffersPropsReader( const std::byte *buffer, std::size_t size, bool own );

     FlatBuffersPropsReader( const FlatBuffersPropsReader& ) = delete;
     FlatBuffersPropsReader& operator=( const FlatBuffersPropsReader& ) = delete;

     /// @brief Destructor.
     ~FlatBuffersPropsReader();

     /// @copydoc IPropsReader::is_owner() const noexcept
     bool is_owner() const noexcept override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< uint64_t > get_uint64( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< uint32_t > get_uint32( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< uint16_t > get_uint16( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< uint8_t > get_uint8( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< int64_t > get_int64( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< int32_t > get_int32( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< int16_t > get_int16( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< int8_t > get_int8( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< bool > get_bool( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< float > get_float( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< double > get_double( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< std::string > get_string( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< uint64_t > > get_uint64_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< uint32_t > > get_uint32_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< uint16_t > > get_uint16_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< uint8_t > > get_uint8_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< int64_t > > get_int64_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< int32_t > > get_int32_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< int16_t > > get_int16_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< int8_t > > get_int8_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< bool > > get_bool_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< float > > get_float_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< double > > get_double_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64_arr(const std::string&)
     std::optional< std::vector< std::string > > get_string_arr( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< Vec2f > get_vec2f( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< Vec3f > get_vec3f( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< Vec4f > get_vec4f( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< Vec2i > get_vec2i( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< Vec3i > get_vec3i( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< Vec4i > get_vec4i( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< FloatRect > get_float_rect( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< IntRect > get_int_rect( const std::string& name ) override;

     /// @copydoc IPropsReader::get_uint64(const std::string&)
     std::optional< ResourceIndex > get_resource_index( const std::string& name ) override;

private:
     std::vector< std::byte > buffer_;  ///< underlying buffer.
     flexbuffers::Map *root_;           ///< reference to root properties map.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_FLATBUFFERS_PROPS_READER_H
