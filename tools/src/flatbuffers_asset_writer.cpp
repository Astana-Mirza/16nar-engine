#include <16nar/tools/flatbuffers_asset_writer.h>
#include <16nar/tools/convertor_utils.inl>

#include <16nar/render/render_defs.h>
#include <16nar/gen/package_generated.h>
#include <16nar/gen/resource_generated.h>

#include <stdexcept>
#include <ostream>

namespace
{

constexpr std::size_t initial_buffer_size = 256;


_16NAR_ENUM_CONVERTOR( _16nar::data::package::BufferDataFormat, _16nar::BufferDataFormat,
     { _16nar::BufferDataFormat::Rgb,               _16nar::data::package::BufferDataFormat::Rgb               },
     { _16nar::BufferDataFormat::Rgba,              _16nar::data::package::BufferDataFormat::Rgba              },
     { _16nar::BufferDataFormat::Srgb,              _16nar::data::package::BufferDataFormat::Srgb              },
     { _16nar::BufferDataFormat::Srgba,             _16nar::data::package::BufferDataFormat::Srgba             } )
_16NAR_ENUM_CONVERTOR( _16nar::data::package::TextureFilter, _16nar::TextureFilter,
     { _16nar::TextureFilter::Nearest,              _16nar::data::package::TextureFilter::Nearest              },
     { _16nar::TextureFilter::Linear,               _16nar::data::package::TextureFilter::Linear               },
     { _16nar::TextureFilter::NearestMipmapNearest, _16nar::data::package::TextureFilter::NearestMipmapNearest },
     { _16nar::TextureFilter::NearestMipmapLinear,  _16nar::data::package::TextureFilter::NearestMipmapLinear  },
     { _16nar::TextureFilter::LinearMipmapNearest,  _16nar::data::package::TextureFilter::LinearMipmapNearest  },
     { _16nar::TextureFilter::LinearMipmapLinear,   _16nar::data::package::TextureFilter::LinearMipmapLinear   } )
_16NAR_ENUM_CONVERTOR( _16nar::data::package::TextureWrap, _16nar::TextureWrap,
     { _16nar::TextureWrap::Repeat,                 _16nar::data::package::TextureWrap::Repeat                 },
     { _16nar::TextureWrap::MirroredRepeat,         _16nar::data::package::TextureWrap::MirroredRepeat         },
     { _16nar::TextureWrap::ClampToEdge,            _16nar::data::package::TextureWrap::ClampToEdge            },
     { _16nar::TextureWrap::ClampToBorder,          _16nar::data::package::TextureWrap::ClampToBorder          } )
_16NAR_ENUM_CONVERTOR( _16nar::data::package::DataType, _16nar::DataType,
     { _16nar::DataType::Byte,                      _16nar::data::package::DataType::Byte                      },
     { _16nar::DataType::Float,                     _16nar::data::package::DataType::Float                     } )
_16NAR_ENUM_CONVERTOR( _16nar::data::package::ShaderType, _16nar::ShaderType,
     { _16nar::ShaderType::Vertex,                  _16nar::data::package::ShaderType::Vertex                  },
     { _16nar::ShaderType::Fragment,                _16nar::data::package::ShaderType::Fragment                },
     { _16nar::ShaderType::Geometry,                _16nar::data::package::ShaderType::Geometry                } )
_16NAR_ENUM_CONVERTOR( _16nar::data::package::BufferType, _16nar::BufferType,
     { _16nar::BufferType::StreamDraw,              _16nar::data::package::BufferType::StreamDraw              },
     { _16nar::BufferType::StreamRead,              _16nar::data::package::BufferType::StreamRead              },
     { _16nar::BufferType::StreamCopy,              _16nar::data::package::BufferType::StreamCopy              },
     { _16nar::BufferType::StaticDraw,              _16nar::data::package::BufferType::StaticDraw              },
     { _16nar::BufferType::StaticRead,              _16nar::data::package::BufferType::StaticRead              },
     { _16nar::BufferType::StaticCopy,              _16nar::data::package::BufferType::StaticCopy              },
     { _16nar::BufferType::DynamicDraw,             _16nar::data::package::BufferType::DynamicDraw             },
     { _16nar::BufferType::DynamicRead,             _16nar::data::package::BufferType::DynamicRead             },
     { _16nar::BufferType::DynamicCopy,             _16nar::data::package::BufferType::DynamicCopy             } )


flatbuffers::Offset< _16nar::data::package::Resource > write_texture(
     const _16nar::tools::ResourceData& resource,
     flatbuffers::FlatBufferBuilder& builder,
     std::vector< _16nar::DataSharedPtr >& data_units )
{
     auto params = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Texture > >( resource.params );

     auto data_sizes = builder.CreateVector( resource.data_sizes.data(), resource.data_sizes.size() );
     auto name = builder.CreateString( resource.name );

     auto border_color = _16nar::data::Vec4f{
          flatbuffers::span< const float, _16nar::Vec4f::size >{ params.border_color.data(), _16nar::Vec4f::size } };
     auto size = _16nar::data::Vec2i{
          flatbuffers::span< const int32_t, _16nar::Vec2i::size >{ params.size.data(), _16nar::Vec2i::size } };

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

     data_units.emplace_back( params.data );
     return res;
}


flatbuffers::Offset< _16nar::data::package::Resource > write_shader(
     const _16nar::tools::ResourceData& resource,
     flatbuffers::FlatBufferBuilder& builder,
     std::vector< _16nar::DataSharedPtr >& data_units )
{
     auto params = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Shader > >( resource.params );

     auto data_sizes = builder.CreateVector( resource.data_sizes.data(), resource.data_sizes.size() );
     auto name = builder.CreateString( resource.name );

     std::vector< flatbuffers::Offset< _16nar::data::package::Shader > > shaders;
     shaders.reserve( params.shaders.size() );
     for ( const auto& param : params.shaders )
     {
          auto entrypoint = builder.CreateString( param.entrypoint );
          _16nar::data::package::ShaderBuilder shader_builder{ builder };
          shader_builder.add_entrypoint( entrypoint );
          shader_builder.add_type( convert_enum( param.type ) );
          shader_builder.add_from_source( param.from_source );
          shaders.emplace_back( shader_builder.Finish() );
          data_units.emplace_back( param.data );
     }

     auto shaders_stored = builder.CreateVector( shaders );
     _16nar::data::package::ShaderLoadParamsBuilder shader_params_builder{ builder };
     shader_params_builder.add_shaders( shaders_stored );
     auto shader_params = shader_params_builder.Finish();

     _16nar::data::package::ResourceBuilder res_builder{ builder };
     res_builder.add_name( name );
     res_builder.add_params_type( _16nar::data::package::AnyLoadParams::ShaderLoadParams );
     res_builder.add_params( shader_params.Union() );
     res_builder.add_data_sizes( data_sizes );
     auto res = res_builder.Finish();

     return res;
}


flatbuffers::Offset< _16nar::data::package::Resource > write_vertex_buffer(
     const _16nar::tools::ResourceData& resource,
     flatbuffers::FlatBufferBuilder& builder,
     std::vector< _16nar::DataSharedPtr >& data_units )
{
     auto params = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::VertexBuffer > >( resource.params );

     auto data_sizes = builder.CreateVector( resource.data_sizes.data(), resource.data_sizes.size() );
     auto name = builder.CreateString( resource.name );

     std::vector< _16nar::data::package::AttribParams > attrs;
     attrs.reserve( params.attributes.size() );
     for ( const auto& attr : params.attributes )
     {
          attrs.emplace_back( _16nar::data::package::AttribParams{
               static_cast< uint32_t >( attr.size ), convert_enum( attr.data_type ), attr.normalized } );
     }

     auto attrs_stored = builder.CreateVectorOfStructs( attrs.data(), attrs.size() );
     _16nar::data::package::VertexBufferLoadParamsBuilder vb_params_builder{ builder };
     vb_params_builder.add_attrs( attrs_stored );
     vb_params_builder.add_buffer_type( convert_enum( params.buffer.type ) );
     vb_params_builder.add_index_buffer_type( convert_enum( params.index_buffer.type ) );
     auto vb_params = vb_params_builder.Finish();

     _16nar::data::package::ResourceBuilder res_builder{ builder };
     res_builder.add_name( name );
     res_builder.add_params_type( _16nar::data::package::AnyLoadParams::VertexBufferLoadParams );
     res_builder.add_params( vb_params.Union() );
     res_builder.add_data_sizes( data_sizes );
     auto res = res_builder.Finish();

     data_units.emplace_back( params.buffer.data );
     data_units.emplace_back( params.index_buffer.data );
     return res;
}


flatbuffers::Offset< _16nar::data::package::Resource > write_cubemap(
     const _16nar::tools::ResourceData& resource,
     flatbuffers::FlatBufferBuilder& builder,
     std::vector< _16nar::DataSharedPtr >& data_units )
{
     auto params = std::any_cast< _16nar::LoadParams< _16nar::ResourceType::Cubemap > >( resource.params );

     auto data_sizes = builder.CreateVector( resource.data_sizes.data(), resource.data_sizes.size() );
     auto name = builder.CreateString( resource.name );

     auto border_color = _16nar::data::Vec4f{
          flatbuffers::span< const float, _16nar::Vec4f::size >{ params.border_color.data(), _16nar::Vec4f::size } };
     auto size = _16nar::data::Vec2i{
          flatbuffers::span< const int32_t, _16nar::Vec2i::size >{ params.size.data(), _16nar::Vec2i::size } };

     _16nar::data::package::CubemapLoadParamsBuilder cubemap_builder{ builder };
     cubemap_builder.add_format( convert_enum( params.format ) );
     cubemap_builder.add_min_filter( convert_enum( params.min_filter ) );
     cubemap_builder.add_mag_filter( convert_enum( params.mag_filter ) );
     cubemap_builder.add_wrap_x( convert_enum( params.wrap_x ) );
     cubemap_builder.add_wrap_y( convert_enum( params.wrap_y ) );
     cubemap_builder.add_wrap_z( convert_enum( params.wrap_z ) );
     cubemap_builder.add_data_type( convert_enum( params.data_type ) );
     cubemap_builder.add_border_color( &border_color );
     cubemap_builder.add_size( &size );
     auto cubemap = cubemap_builder.Finish();

     _16nar::data::package::ResourceBuilder res_builder{ builder };
     res_builder.add_name( name );
     res_builder.add_params_type( _16nar::data::package::AnyLoadParams::CubemapLoadParams );
     res_builder.add_params( cubemap.Union() );
     res_builder.add_data_sizes( data_sizes );
     auto res = res_builder.Finish();

     std::copy( params.data.cbegin(), params.data.cend(), std::back_inserter( data_units ) );
     return res;
}


flatbuffers::Offset< _16nar::data::package::Resource > write_resource(
     const _16nar::tools::ResourceData& resource,
     flatbuffers::FlatBufferBuilder& builder,
     std::vector< _16nar::DataSharedPtr >& data_units )
{
     flatbuffers::Offset< _16nar::data::package::Resource > res_buffer{};
     switch ( resource.type )
     {
          case _16nar::ResourceType::Texture:
               res_buffer = write_texture( resource, builder, data_units );
               break;
          case _16nar::ResourceType::Shader:
               res_buffer = write_shader( resource, builder, data_units );
               break;
          case _16nar::ResourceType::VertexBuffer:
               res_buffer = write_vertex_buffer( resource, builder, data_units );
               break;
          case _16nar::ResourceType::Cubemap:
               res_buffer = write_cubemap( resource, builder, data_units );
               break;
          default:
               throw std::runtime_error{ "wrong resource type: "
                    + std::to_string( static_cast< std::size_t >( resource.type ) ) };
     }
     return res_buffer;
}

} // anonymous namespace


namespace _16nar::tools
{

const char *FlatBuffersAssetWriter::get_file_ext() const noexcept
{
     return _16nar::data::package::ResourceExtension();
}


const char *FlatBuffersAssetWriter::get_pkg_ext() const noexcept
{
     return _16nar::data::package::PackageExtension();
}


void FlatBuffersAssetWriter::write_asset( std::ostream& output, const ResourceData& resource )
{
     std::vector< _16nar::DataSharedPtr > data_units;
     flatbuffers::FlatBufferBuilder builder{ initial_buffer_size };

     auto res_buffer = write_resource( resource, builder, data_units );
     builder.Finish( res_buffer, _16nar::data::package::ResourceIdentifier() );

     uint8_t *buf = builder.GetBufferPointer();
     uint32_t size = builder.GetSize();

     output.write( reinterpret_cast< char * >( &size ), sizeof( size ) );
     output.write( reinterpret_cast< char * >( buf ), size );

     for ( std::size_t i = 0; i < data_units.size(); i++ )
     {
          output.write( reinterpret_cast< char * >( data_units[ i ].get() ), resource.data_sizes.at( i ) );
     }
}


void FlatBuffersAssetWriter::write_package( std::ostream& output, const PackageData& package )
{
     std::vector< _16nar::DataSharedPtr > data_units;
     flatbuffers::FlatBufferBuilder builder{ initial_buffer_size };

     std::vector< flatbuffers::Offset< _16nar::data::package::Resource > > resources;
     resources.reserve( package.resources.size() );
     for ( const auto& resource : package.resources )
     {
          resources.emplace_back( write_resource( resource, builder, data_units ) );
     }
     auto resources_stored = builder.CreateVector( resources );
     _16nar::data::package::PackageBuilder pkg_builder{ builder };
     pkg_builder.add_resources( resources_stored );

     auto pkg_stored = pkg_builder.Finish();
     builder.Finish( pkg_stored, _16nar::data::package::PackageIdentifier() );

     uint8_t *buf = builder.GetBufferPointer();
     uint32_t size = builder.GetSize();

     output.write( reinterpret_cast< char * >( &size ), sizeof( size ) );
     output.write( reinterpret_cast< char * >( buf ), size );

     std::vector< uint32_t > data_sizes;
     data_sizes.reserve( data_units.size() );
     for ( const auto& resource : package.resources )
     {
          std::copy( resource.data_sizes.cbegin(), resource.data_sizes.cend(), std::back_inserter( data_sizes ) );
     }
     for ( std::size_t i = 0; i < data_units.size(); i++ )
     {
          output.write( reinterpret_cast< char * >( data_units[ i ].get() ), data_sizes.at( i ) );
     }
}

} // namespace _16nar::tools
