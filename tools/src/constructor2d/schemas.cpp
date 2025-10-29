#include <16nar/tools/constructor2d/schemas.h>

#include <16nar/tools/utils.h>

namespace _16nar::tools::constructor2d
{

DataSchema make_default_schema( NodeType type, IPropsWriterFactory& factory )
{
     DataSchema schema{};
     std::uint16_t index = 0;

     auto add = [ &schema, &index ]( const std::string& name, StoredDataType type, bool mandatory ) -> void
     {
          schema.items.emplace( name, DataItem{ index++, type, mandatory } );
          schema.ordered_items.emplace_back( name );
     };

     // common properties for all nodes
     add( "parent", StoredDataType::Uint32, false );
     add( "pos", StoredDataType::Vec2f, true );
     add( "scale", StoredDataType::Vec2f, true );
     add( "rotation", StoredDataType::Float, true );
     add( "setup", StoredDataType::ResourceIndex, false );
     add( "loop", StoredDataType::ResourceIndex, false );
     add( "link", StoredDataType::ResourceIndex, false );
     add( "name", StoredDataType::String, false );

     switch ( type )
     {
          case NodeType::SpriteNode:
          {
               add( "size", StoredDataType::Vec2f, true );
               add( "shader", StoredDataType::ResourceIndex, true );
               add( "texture", StoredDataType::ResourceIndex, true );
               add( "layer", StoredDataType::Uint8, true );
               add( "visible", StoredDataType::Bool, true );
               add( "texrect", StoredDataType::FloatRect, true );
               break;
          }
          default:
               break;
     }

     // now items' description is complete, write default values
     auto writer = factory.create_writer( schema );

     writer->set_vec2f( "pos", Vec2f{ 0.0f, 0.0f } );
     writer->set_vec2f( "scale", Vec2f{ 1.0f, 1.0f } );
     writer->set_float( "rotation", 0.0f );

     switch ( type )
     {
          case NodeType::SpriteNode:
          {
               writer->set_bool( "visible", true );
               writer->set_float_rect( "texrect", FloatRect{ Vec2f{ 0.0f, 0.0f }, 0.0f, 0.0f } );
               break;
          }
          default:
               break;
     }

     schema.set_default_vals( writer->conver_to_reader() );
     return schema;
}


DataSchema make_render_system_schema( RenderSystemType type, IPropsWriterFactory& factory )
{
     DataSchema schema{};
     std::uint16_t index = 0;

     auto add = [ &schema, &index ]( const std::string& name, StoredDataType type, bool mandatory ) -> void
     {
          schema.items.emplace( name, DataItem{ index++, type, mandatory } );
          schema.ordered_items.emplace_back( name );
     };

     switch ( type )
     {
          case RenderSystemType::QTreeRenderSystem:
          {
               add( "quad_start", StoredDataType::Vec2f, true );
               add( "quad_size", StoredDataType::Vec2f, true );
               add( "size", StoredDataType::Vec2i, true );
               break;
          }
          default:
               break;
     }

     return schema;
}

} // namespace _16nar::tools::constructor2d