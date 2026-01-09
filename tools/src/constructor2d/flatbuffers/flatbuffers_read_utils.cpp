#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_read_utils.h>

#include <16nar/tools/convertor_utils.inl>

#include <16nar/gen/flatbuffers/constructor2d_scene_generated.h>

#include <stdexcept>

namespace
{

_16NAR_ENUM_CONVERTOR( _16nar::tools::constructor2d::NodeType, _16nar::data::constructor2d::NodeType,
     { _16nar::data::constructor2d::NodeType::Node2D,       _16nar::tools::constructor2d::NodeType::Node2D         },
     { _16nar::data::constructor2d::NodeType::SpriteNode,   _16nar::tools::constructor2d::NodeType::SpriteNode,    } )


_16NAR_ENUM_CONVERTOR( _16nar::tools::constructor2d::RenderSystemType,
          _16nar::data::constructor2d::RenderSystemType,
     { _16nar::data::constructor2d::RenderSystemType::QTreeRenderSystem,
     _16nar::tools::constructor2d::RenderSystemType::QTreeRenderSystem } )

} // anonymous namespace


namespace _16nar::tools::constructor2d
{

Dependencies read_scene_deps( const _16nar::data::constructor2d::Dependencies *deps )
{
     if ( !deps )
     {
          throw std::runtime_error{ "scene dependencies are broken" };
     }

     Dependencies result{};

     auto copy_vector = []( const auto *src, auto& dst ) -> void
     {
          if ( !src )
          {
               throw std::runtime_error{ "dependency vector is broken" };
          }
          for ( const auto str : *src )
          {
               if ( str && str->c_str() && str->size() )
               {
                    dst.emplace_back( str->c_str() );
               }
          }
     };

     const auto *resources = deps->resources();
     if ( !resources )
     {
          throw std::runtime_error{ "scene dependency resources are broken" };
     }
     result.resources.reserve( resources->size() );
     for ( const auto *resource : *resources )
     {
          if ( !resource || !resource->name() || !resource->name()->c_str() )
          {
               throw std::runtime_error{ "scene dependency resource is broken" };
          }
          PackageDeps dep{};
          dep.name = resource->name()->c_str();
          copy_vector( resource->elements(), dep.elements );
          result.resources.emplace_back( std::move( dep ) );
     }

     const auto *libs = deps->libs();
     if ( !libs )
     {
          throw std::runtime_error{ "scene dependency libraries are broken" };
     }
     result.libs.reserve( libs->size() );
     for ( const auto *lib : *libs )
     {
          if ( !lib || !lib->name() || !lib->name()->c_str() )
          {
               throw std::runtime_error{ "scene dependency library is broken" };
          }
          PackageDeps dep{};
          dep.name = lib->name()->c_str();
          copy_vector( lib->elements(), dep.elements );
          result.libs.emplace_back( std::move( dep ) );
     }

     const auto *pieces = deps->pieces();
     if ( !pieces )
     {
          throw std::runtime_error{ "scene pieces are broken" };
     }
     result.pieces.reserve( pieces->size() );
     for ( const auto *piece : *pieces )
     {
          if ( !piece || !piece->name() || !piece->name()->c_str() )
          {
               throw std::runtime_error{ "scene dependency piece is broken" };
          }
          PackageDeps dep{};
          dep.name = piece->name()->c_str();
          copy_vector( piece->elements(), dep.elements );
          result.pieces.emplace_back( std::move( dep ) );
     }

     return result;
}


FlatBuffersPropsReader create_props_reader( const _16nar::data::Properties *props )
{
     if ( !props )
     {
          throw std::runtime_error{ "flatbuffers props are broken" };
     }

     const auto *data = props->data();
     if ( !data )
     {
          throw std::runtime_error{ "flatbuffers props data is broken" };
     }

     FlatBuffersPropsReader reader{ reinterpret_cast< const std::byte * >(
          data->Data() ), data->size(), false };
     const auto *unordered_data = props->unordered_data();
     if ( unordered_data )
     {
          reader.set_unordered_buffer( reinterpret_cast< const std::byte * >(
               unordered_data->Data() ), unordered_data->size(), false );
     }
     return reader;
}


FlatBuffersPropsReader make_node_reader(
     const _16nar::data::constructor2d::NodeData *node, NodeType& type )
{
     if ( !node )
     {
          throw std::runtime_error{ "2d node is broken" };
     }
     auto reader = create_props_reader( node->props() );
     type = convert_enum( node->type() );
     return reader;
}


FlatBuffersPropsReader make_render_system_reader(
     const _16nar::data::constructor2d::RenderSystemData *render_system, RenderSystemType& type )
{
     if ( !render_system )
     {
          throw std::runtime_error{ "2d render system is broken" };
     }
     auto reader = create_props_reader( render_system->props() );
     type = convert_enum( render_system->type() );
     return reader;
}

} // namespace _16nar::tools::constructor2d
