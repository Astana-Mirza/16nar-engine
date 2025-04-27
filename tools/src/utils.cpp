#include <16nar/tools/utils.h>

#include <fstream>
#include <filesystem>
#include <stdexcept>


#define _16NAR_COPY_PROP( TYPENAME )                        \
     {                                                      \
          auto prop = reader.get_##TYPENAME( name );        \
          if ( prop.has_value() )                           \
          {                                                 \
               writer.set_##TYPENAME( name, prop.value() ); \
          }                                                 \
     }


namespace _16nar::tools
{

DataSharedPtr read_binary( const std::string& path, std::size_t& data_size, bool zero_terminate )
{
     std::ifstream bin_file( path, std::ios_base::binary );
     if ( !bin_file )
     {
          throw std::runtime_error{ "cannot open file " + path };
     }
     bin_file.seekg( 0, std::ios_base::end );
     data_size = bin_file.tellg();
     bin_file.seekg( 0, std::ios_base::beg );
     std::size_t actual_size = data_size + ( zero_terminate ? 1 : 0 );     // +1 byte for zero termination
     _16nar::DataSharedPtr data{ new std::byte[ actual_size ], std::default_delete< std::byte[] >() };
     bin_file.read( reinterpret_cast< char * >( data.get() ), data_size );
     if ( zero_terminate )
     {
          *( data.get() + ( actual_size - 1 ) ) = std::byte{ 0 };
          data_size = actual_size;
     }
     return data;
}


void write_binary( const std::string& path, DataSharedPtr data, std::size_t data_size, bool zero_terminated )
{
     std::filesystem::path base_path = std::filesystem::path{ path }.remove_filename();
     if ( !std::filesystem::exists( base_path ) && !std::filesystem::create_directories( base_path ) )
     {
          throw std::runtime_error{ "cannot create directory " + base_path.string() };
     }
     std::ofstream bin_file( path, std::ios_base::binary );
     if ( !bin_file )
     {
          throw std::runtime_error{ "cannot open file " + path };
     }
     if ( zero_terminated )   // remove terminating zero byte
     {
          --data_size;
     }
     bin_file.write( reinterpret_cast< char * >( data.get() ), data_size );
}


std::string correct_path( const std::string& base_dir, const std::string& path )
{
     std::filesystem::path result{ path };
     if ( !base_dir.empty() && result.is_relative() )
     {
          result = std::filesystem::path{ base_dir } / result;
     }
     return result.string();
}


int get_channel_count( BufferDataFormat format )
{
     switch ( format )
     {
          case BufferDataFormat::Rgb:
          case BufferDataFormat::Srgb:
               return 3;
          case BufferDataFormat::Rgba:
          case BufferDataFormat::Srgba:
               return 4;
          default:
               throw std::runtime_error{ "wrong buffer data format: "
                    + std::to_string( static_cast< std::size_t >( format ) ) };
     }
     return 3;
}


void copy_property( IPropsReader& reader, const std::string& name,
     const DataItem& item, bool copy_optional, IPropsWriter& writer )
{
     if ( !copy_optional && !item.mandatory )
     {
          return;
     }

     switch ( item.type )
     {
          case StoredDataType::Uint64       :     _16NAR_COPY_PROP( uint64         );     break;
          case StoredDataType::Uint32       :     _16NAR_COPY_PROP( uint32         );     break;
          case StoredDataType::Uint16       :     _16NAR_COPY_PROP( uint16         );     break;
          case StoredDataType::Uint8        :     _16NAR_COPY_PROP( uint8          );     break;
          case StoredDataType::Int64        :     _16NAR_COPY_PROP( int64          );     break;
          case StoredDataType::Int32        :     _16NAR_COPY_PROP( int32          );     break;
          case StoredDataType::Int16        :     _16NAR_COPY_PROP( int16          );     break;
          case StoredDataType::Int8         :     _16NAR_COPY_PROP( int8           );     break;
          case StoredDataType::Bool         :     _16NAR_COPY_PROP( bool           );     break;
          case StoredDataType::Float        :     _16NAR_COPY_PROP( float          );     break;
          case StoredDataType::Double       :     _16NAR_COPY_PROP( double         );     break;
          case StoredDataType::String       :     _16NAR_COPY_PROP( string         );     break;
          case StoredDataType::Uint64Arr    :     _16NAR_COPY_PROP( uint64_arr     );     break;
          case StoredDataType::Uint32Arr    :     _16NAR_COPY_PROP( uint32_arr     );     break;
          case StoredDataType::Uint16Arr    :     _16NAR_COPY_PROP( uint16_arr     );     break;
          case StoredDataType::Uint8Arr     :     _16NAR_COPY_PROP( uint8_arr      );     break;
          case StoredDataType::Int64Arr     :     _16NAR_COPY_PROP( int64_arr      );     break;
          case StoredDataType::Int32Arr     :     _16NAR_COPY_PROP( int32_arr      );     break;
          case StoredDataType::Int16Arr     :     _16NAR_COPY_PROP( int16_arr      );     break;
          case StoredDataType::Int8Arr      :     _16NAR_COPY_PROP( int8_arr       );     break;
          case StoredDataType::BoolArr      :     _16NAR_COPY_PROP( bool_arr       );     break;
          case StoredDataType::FloatArr     :     _16NAR_COPY_PROP( float_arr      );     break;
          case StoredDataType::DoubleArr    :     _16NAR_COPY_PROP( double_arr     );     break;
          case StoredDataType::StringArr    :     _16NAR_COPY_PROP( string_arr     );     break;
          case StoredDataType::Vec2f        :     _16NAR_COPY_PROP( vec2f          );     break;
          case StoredDataType::Vec3f        :     _16NAR_COPY_PROP( vec3f          );     break;
          case StoredDataType::Vec4f        :     _16NAR_COPY_PROP( vec4f          );     break;
          case StoredDataType::Vec2i        :     _16NAR_COPY_PROP( vec2i          );     break;
          case StoredDataType::Vec3i        :     _16NAR_COPY_PROP( vec3i          );     break;
          case StoredDataType::Vec4i        :     _16NAR_COPY_PROP( vec4i          );     break;
          case StoredDataType::FloatRect    :     _16NAR_COPY_PROP( float_rect     );     break;
          case StoredDataType::IntRect      :     _16NAR_COPY_PROP( int_rect       );     break;
          case StoredDataType::ResourceIndex:     _16NAR_COPY_PROP( resource_index );     break;
          default:
               throw std::runtime_error{ "wrong stored property data format: "
                    + std::to_string( static_cast< std::size_t >( item.type ) ) };
     }
}

} // namespace _16nar::tools
