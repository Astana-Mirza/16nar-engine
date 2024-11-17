#include <16nar/tools/flatbuffers_asset_reader.h>
#include <16nar/tools/convertor_utils.inl>

#include <16nar/render/render_defs.h>
#include <16nar/gen/package_generated.h>
#include <16nar/gen/resource_generated.h>

#include <stdexcept>
#include <istream>

namespace
{

constexpr std::size_t initial_buffer_size = 256;


_16NAR_ENUM_CONVERTOR( _16nar::BufferDataFormat, _16nar::data::package::BufferDataFormat,
     { _16nar::data::package::BufferDataFormat::Rgb,             _16nar::BufferDataFormat::Rgb                 },
     { _16nar::data::package::BufferDataFormat::Rgba,            _16nar::BufferDataFormat::Rgba,               },
     { _16nar::data::package::BufferDataFormat::Srgb,            _16nar::BufferDataFormat::Srgb                },
     { _16nar::data::package::BufferDataFormat::Srgba,           _16nar::BufferDataFormat::Srgba               } )
_16NAR_ENUM_CONVERTOR( _16nar::TextureFilter, _16nar::data::package::TextureFilter,
     { _16nar::data::package::TextureFilter::Nearest,                 _16nar::TextureFilter::Nearest               },
     { _16nar::data::package::TextureFilter::Linear,                  _16nar::TextureFilter::Linear                },
     { _16nar::data::package::TextureFilter::NearestMipmapNearest,    _16nar::TextureFilter::NearestMipmapNearest  },
     { _16nar::data::package::TextureFilter::NearestMipmapLinear,     _16nar::TextureFilter::NearestMipmapLinear   },
     { _16nar::data::package::TextureFilter::LinearMipmapNearest,     _16nar::TextureFilter::LinearMipmapNearest   },
     { _16nar::data::package::TextureFilter::LinearMipmapLinear,      _16nar::TextureFilter::LinearMipmapLinear    } )
_16NAR_ENUM_CONVERTOR( _16nar::TextureWrap, _16nar::data::package::TextureWrap,
     { _16nar::data::package::TextureWrap::Repeat,          _16nar::TextureWrap::Repeat                        },
     { _16nar::data::package::TextureWrap::MirroredRepeat,  _16nar::TextureWrap::MirroredRepeat                },
     { _16nar::data::package::TextureWrap::ClampToEdge,     _16nar::TextureWrap::ClampToEdge,                  },
     { _16nar::data::package::TextureWrap::ClampToBorder,   _16nar::TextureWrap::ClampToBorder                 } )
_16NAR_ENUM_CONVERTOR( _16nar::DataType, _16nar::data::package::DataType,
     { _16nar::data::package::DataType::Byte,               _16nar::DataType::Byte                             },
     { _16nar::data::package::DataType::Float,              _16nar::DataType::Float                            } )
_16NAR_ENUM_CONVERTOR( _16nar::ShaderType, _16nar::data::package::ShaderType,
     { _16nar::data::package::ShaderType::Vertex,           _16nar::ShaderType::Vertex                         },
     { _16nar::data::package::ShaderType::Fragment,         _16nar::ShaderType::Fragment                       },
     { _16nar::data::package::ShaderType::Geometry,         _16nar::ShaderType::Geometry                       } )
_16NAR_ENUM_CONVERTOR( _16nar::BufferType, _16nar::data::package::BufferType,
     { _16nar::data::package::BufferType::StreamDraw,       _16nar::BufferType::StreamDraw                     },
     { _16nar::data::package::BufferType::StreamRead,       _16nar::BufferType::StreamRead                     },
     { _16nar::data::package::BufferType::StreamCopy,       _16nar::BufferType::StreamCopy                     },
     { _16nar::data::package::BufferType::StaticDraw,       _16nar::BufferType::StaticDraw                     },
     { _16nar::data::package::BufferType::StaticRead,       _16nar::BufferType::StaticRead                     },
     { _16nar::data::package::BufferType::StaticCopy,       _16nar::BufferType::StaticCopy                     },
     { _16nar::data::package::BufferType::DynamicDraw,      _16nar::BufferType::DynamicDraw                    },
     { _16nar::data::package::BufferType::DynamicRead,      _16nar::BufferType::DynamicRead                    },
     { _16nar::data::package::BufferType::DynamicCopy,      _16nar::BufferType::DynamicCopy                    } )


void read_texture( const _16nar::data::package::Resource *res_buffer,
     const std::vector< _16nar::DataSharedPtr >& data, _16nar::tools::ResourceData& resource )
{
     auto params = res_buffer->params_as_TextureLoadParams();
     _16nar::LoadParams< _16nar::ResourceType::Texture > api_params{};
     resource.type = _16nar::ResourceType::Texture;

     api_params.format = convert_enum( params->format() );
     api_params.min_filter = convert_enum( params->min_filter() );
     api_params.mag_filter = convert_enum( params->mag_filter() );
     api_params.wrap_x = convert_enum( params->wrap_x() );
     api_params.wrap_y = convert_enum( params->wrap_y() );
     api_params.data_type = convert_enum( params->data_type() );

     const auto& border_color = *params->border_color()->data();
     api_params.border_color.x() = border_color[ 0 ];
     api_params.border_color.y() = border_color[ 1 ];
     api_params.border_color.z() = border_color[ 2 ];
     api_params.border_color.w() = border_color[ 3 ];

     const auto& size = *params->size()->data();
     api_params.size.x() = size[ 0 ];
     api_params.size.y() = size[ 1 ];

     api_params.samples = params->samples();
     api_params.data = data.at( 0 );

     resource.params = std::any{ api_params };
}


void read_shader( const _16nar::data::package::Resource *res_buffer,
     const std::vector< _16nar::DataSharedPtr >& data, _16nar::tools::ResourceData& resource )
{
     auto params = res_buffer->params_as_ShaderLoadParams();
     _16nar::LoadParams< _16nar::ResourceType::Shader > api_params{};
     resource.type = _16nar::ResourceType::Shader;

     std::size_t shader_num = 0;
     const auto& shaders = *params->shaders();
     for ( const auto& shader : shaders )
     {
          _16nar::LoadParams< _16nar::ResourceType::Shader >::ShaderParams api_shader{};
          api_shader.entrypoint = shader->entrypoint()->c_str();
          api_shader.size = resource.data_sizes.at( shader_num );
          api_shader.data = data.at( shader_num );
          api_shader.type = convert_enum( shader->type() );
          api_shader.from_source = shader->from_source();

          api_params.shaders.emplace_back( api_shader );
          ++shader_num;
     }

     resource.params = std::any{ api_params };
}


void read_vertex_buffer( const _16nar::data::package::Resource *res_buffer,
     const std::vector< _16nar::DataSharedPtr >& data, _16nar::tools::ResourceData& resource )
{
     auto params = res_buffer->params_as_VertexBufferLoadParams();
     _16nar::LoadParams< _16nar::ResourceType::VertexBuffer > api_params{};
     resource.type = _16nar::ResourceType::VertexBuffer;

     api_params.buffer.type = convert_enum( params->buffer_type() );
     api_params.buffer.data = data.at( 0 );
     api_params.buffer.size = resource.data_sizes.at( 0 );

     api_params.index_buffer.type = convert_enum( params->index_buffer_type() );
     api_params.index_buffer.data = data.at( 1 );
     api_params.index_buffer.size = resource.data_sizes.at( 1 );

     const auto& attrs = *params->attrs();
     for ( const auto& attr : attrs )
     {
          _16nar::LoadParams< _16nar::ResourceType::VertexBuffer >::AttribParams api_attr{};
          api_attr.size = attr->size();
          api_attr.data_type = convert_enum( attr->data_type() );
          api_attr.normalized = attr->normalized();

          api_params.attributes.emplace_back( api_attr );
     }

     resource.params = std::any{ api_params };
}


void read_cubemap( const _16nar::data::package::Resource *res_buffer,
     const std::vector< _16nar::DataSharedPtr >& data, _16nar::tools::ResourceData& resource )
{
     auto params = res_buffer->params_as_CubemapLoadParams();
     _16nar::LoadParams< _16nar::ResourceType::Cubemap > api_params{};
     resource.type = _16nar::ResourceType::Cubemap;

     api_params.format = convert_enum( params->format() );
     api_params.min_filter = convert_enum( params->min_filter() );
     api_params.mag_filter = convert_enum( params->mag_filter() );
     api_params.wrap_x = convert_enum( params->wrap_x() );
     api_params.wrap_y = convert_enum( params->wrap_y() );
     api_params.wrap_z = convert_enum( params->wrap_z() );
     api_params.data_type = convert_enum( params->data_type() );

     const auto& border_color = *params->border_color()->data();
     api_params.border_color.x() = border_color[ 0 ];
     api_params.border_color.y() = border_color[ 1 ];
     api_params.border_color.z() = border_color[ 2 ];
     api_params.border_color.w() = border_color[ 3 ];

     const auto& size = *params->size()->data();
     api_params.size.x() = size[ 0 ];
     api_params.size.y() = size[ 1 ];

     for ( std::size_t i = 0; i < api_params.data.size(); i++ )
     {
          api_params.data[ i ] = data.at( i );
     }

     resource.params = std::any{ api_params };
}


std::vector< uint8_t > read_header( std::istream& input, uint32_t& header_size )
{
     input.read( reinterpret_cast< char * >( &header_size ), sizeof( header_size ) );
     std::vector< uint8_t > header_buffer( header_size );
     input.read( reinterpret_cast< char * >( header_buffer.data() ), header_size );
     return header_buffer;
}


_16nar::tools::ResourceData read_resource( const _16nar::data::package::Resource *res_buffer,
     std::istream& input )
{
     _16nar::tools::ResourceData resource{};
     std::vector< _16nar::DataSharedPtr > data;
     for ( uint32_t data_size : *res_buffer->data_sizes() )
     {
          data.emplace_back( _16nar::DataSharedPtr{
                    new std::byte[ data_size ], std::default_delete< std::byte[] >() } );
          input.read( reinterpret_cast< char * >( data.back().get() ), data_size );
          resource.data_sizes.push_back( data_size );
     }

     auto type = res_buffer->params_type();
     resource.name = res_buffer->name()->c_str();
     switch ( type )
     {
          case _16nar::data::package::AnyLoadParams::TextureLoadParams:
               read_texture( res_buffer, data, resource );
               break;
          case _16nar::data::package::AnyLoadParams::ShaderLoadParams:
               read_shader( res_buffer, data, resource );
               break;
          case _16nar::data::package::AnyLoadParams::VertexBufferLoadParams:
               read_vertex_buffer( res_buffer, data, resource );
               break;
          case _16nar::data::package::AnyLoadParams::CubemapLoadParams:
               read_cubemap( res_buffer, data, resource );
               break;
          default:
               throw std::runtime_error{ "wrong resource type: "
                    + std::to_string( static_cast< std::size_t >( type ) ) };
     }
     return resource;
}

} // anonymous namespace


namespace _16nar::tools
{

std::string FlatBuffersAssetReader::get_file_ext() const noexcept
{
     return _16nar::data::package::ResourceExtension();
}


std::string FlatBuffersAssetReader::get_pkg_ext() const noexcept
{
     return _16nar::data::package::PackageExtension();
}


ResourceData FlatBuffersAssetReader::read_asset( std::istream& input )
{
     uint32_t header_size = 0;
     auto header_buffer = read_header( input, header_size );

     auto res_buffer = _16nar::data::package::GetResource( header_buffer.data() );
     flatbuffers::Verifier verifier{ header_buffer.data(), header_size };
     if ( !_16nar::data::package::VerifyResourceBuffer( verifier ) )
     {
          throw std::runtime_error{ "asset is broken" };
     }
     return read_resource( res_buffer, input );
}


PackageData FlatBuffersAssetReader::read_package( std::istream& input )
{
     uint32_t header_size = 0;
     auto header_buffer = read_header( input, header_size );

     auto pkg_buffer = _16nar::data::package::GetPackage( header_buffer.data() );
     flatbuffers::Verifier verifier{ header_buffer.data(), header_size };
     if ( !_16nar::data::package::VerifyPackageBuffer( verifier ) )
     {
          throw std::runtime_error{ "package is broken" };
     }

     PackageData package{};
     const auto& resources = *pkg_buffer->resources();
     for ( const auto& resource : resources )
     {
          package.resources.emplace_back( read_resource( resource, input ) );
     }
     return package;
}

} // namespace _16nar::tools
