#include <flatbuffers_asset_writer.h>

#include <16nar/render/render_defs.h>
#include <16nar/gen/package_generated.h>
#include <16nar/gen/resource_generated.h>

#include <fstream>
#include <stdexcept>
#include <unordered_map>

namespace
{

constexpr std::size_t initial_buffer_size = 256;


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


_16NAR_ENUM_CONVERTOR( _16nar::data::package::BufferDataFormat, _16nar::BufferDataFormat,
     { _16nar::BufferDataFormat::Rgb,             _16nar::data::package::BufferDataFormat::Rgb          },
     { _16nar::BufferDataFormat::Rgba,            _16nar::data::package::BufferDataFormat::Rgba         },
     { _16nar::BufferDataFormat::Srgb,            _16nar::data::package::BufferDataFormat::Srgb         },
     { _16nar::BufferDataFormat::Srgba,           _16nar::data::package::BufferDataFormat::Srgba        } )
_16NAR_ENUM_CONVERTOR( _16nar::data::package::TextureFilter, _16nar::TextureFilter,
     { _16nar::TextureFilter::Nearest,              _16nar::data::package::TextureFilter::Nearest               },
     { _16nar::TextureFilter::Linear,               _16nar::data::package::TextureFilter::Linear                },
     { _16nar::TextureFilter::NearestMipmapNearest, _16nar::data::package::TextureFilter::NearestMipmapNearest  },
     { _16nar::TextureFilter::NearestMipmapLinear,  _16nar::data::package::TextureFilter::NearestMipmapLinear   },
     { _16nar::TextureFilter::LinearMipmapNearest,  _16nar::data::package::TextureFilter::LinearMipmapNearest   },
     { _16nar::TextureFilter::LinearMipmapLinear,   _16nar::data::package::TextureFilter::LinearMipmapLinear    } )
_16NAR_ENUM_CONVERTOR( _16nar::data::package::TextureWrap, _16nar::TextureWrap,
     { _16nar::TextureWrap::Repeat,               _16nar::data::package::TextureWrap::Repeat         },
     { _16nar::TextureWrap::MirroredRepeat,       _16nar::data::package::TextureWrap::MirroredRepeat },
     { _16nar::TextureWrap::ClampToEdge,          _16nar::data::package::TextureWrap::ClampToEdge    },
     { _16nar::TextureWrap::ClampToBorder,        _16nar::data::package::TextureWrap::ClampToBorder  } )
_16NAR_ENUM_CONVERTOR( _16nar::data::package::DataType, _16nar::DataType,
     { _16nar::DataType::Byte,                    _16nar::data::package::DataType::Byte     },
     { _16nar::DataType::Float,                   _16nar::data::package::DataType::Float    } )


std::vector< _16nar::DataSharedPtr > write_texture(
     const _16nar::tools::ResourceData& resource,
     flatbuffers::FlatBufferBuilder& builder )
{
     std::vector< _16nar::DataSharedPtr > data_units;
     auto params = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Texture > >( resource.params );

     auto border_color = _16nar::data::Vec4f{
          flatbuffers::span< const float, _16nar::Vec4f::size >{ params.border_color.data(), _16nar::Vec4f::size } };
     auto size = _16nar::data::Vec2i{
          flatbuffers::span< const int32_t, _16nar::Vec2i::size >{ params.size.data(), _16nar::Vec2i::size } };
     auto data_sizes = builder.CreateVector( resource.data_sizes.data(), resource.data_sizes.size() );
     auto name = builder.CreateString( resource.name );

     _16nar::data::package::TextureLoadParamsBuilder tex_builder{ builder };
     tex_builder.add_format( convert_enum( params.format ) );
     tex_builder.add_min_filter( convert_enum( params.min_filter ) );
     tex_builder.add_mag_filter( convert_enum( params.mag_filter ) );
     tex_builder.add_wrap_x( convert_enum( params.wrap_x ) );
     tex_builder.add_wrap_y( convert_enum( params.wrap_y ) );
     tex_builder.add_data_type( convert_enum( params.data_type ) );
     tex_builder.add_border_color( &border_color );
     tex_builder.add_size( &size );
     tex_builder.add_samples( params.samples );
     auto texture = tex_builder.Finish();

     _16nar::data::package::ResourceBuilder res_builder{ builder };
     res_builder.add_name( name );
     res_builder.add_params_type( _16nar::data::package::AnyLoadParams::TextureLoadParams );
     res_builder.add_params( texture.Union() );
     res_builder.add_data_sizes( data_sizes );

     auto res = res_builder.Finish();
     builder.Finish( res );

     data_units.push_back( params.data );
     return data_units;
}

} // anonymous namespace


namespace _16nar::tools
{

void FlatBuffersAssetWriter::write_asset( const ResourceData& resource, const std::string& dir )
{
     std::vector< _16nar::DataSharedPtr > data_units;
     flatbuffers::FlatBufferBuilder builder{ initial_buffer_size };

     switch ( resource.type )
     {
          case _16nar::ResourceType::Texture:
               data_units = write_texture( resource, builder );
               break;
          default:
               throw std::runtime_error{ "wrong resource type: "
                    + std::to_string( static_cast< std::size_t >( resource.type ) ) };
     }

     uint8_t *buf = builder.GetBufferPointer();
     uint32_t size = builder.GetSize();

     std::ofstream ofs{ dir + "/" + resource.name + "." + _16nar::data::package::ResourceExtension(),
          std::ios::out | std::ios::binary };
     ofs.write( reinterpret_cast< char * >( &size ), sizeof( size ) );
     ofs.write( reinterpret_cast< char * >( buf ), size );

     for ( std::size_t i = 0; i < data_units.size(); i++ )
     {
          ofs.write( reinterpret_cast< char * >( data_units[ i ].get() ), resource.data_sizes.at( i ) );
     }
}

} // namespace _16nar::tools
