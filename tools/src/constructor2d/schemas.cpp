#include <16nar/tools/constructor2d/schemas.h>

namespace _16nar::tools::constructor2d
{

DataSchema make_default_schema( IPropsWriter& writer )
{
     DataSchema schema;

     // common properties for all nodes
     schema.items[ "parent" ] = DataItem{ StoredDataType::Uint32, false };
     schema.items[ "pos" ] = DataItem{ StoredDataType::Vec2f, true };
     schema.items[ "scale" ] = DataItem{ StoredDataType::Vec2f, true };
     schema.items[ "rotation" ] = DataItem{ StoredDataType::Float, true };
     schema.items[ "setup" ] = DataItem{ StoredDataType::ResourceIndex, false };
     schema.items[ "loop" ] = DataItem{ StoredDataType::ResourceIndex, false };
     schema.items[ "link" ] = DataItem{ StoredDataType::ResourceIndex, false };
     schema.items[ "name" ] = DataItem{ StoredDataType::String, false };
     writer.set_vec2f( "pos", Vec2f{ 0.0f, 0.0f } );
     writer.set_vec2f( "scale", Vec2f{ 1.0f, 1.0f } );
     writer.set_float( "rotation", 0.0f );

     // sprite node
     schema.items[ "size" ] = DataItem{ StoredDataType::Vec2f, true };
     schema.items[ "shader" ] = DataItem{ StoredDataType::ResourceIndex, true };
     schema.items[ "texture" ] = DataItem{ StoredDataType::ResourceIndex, true };
     schema.items[ "layer" ] = DataItem{ StoredDataType::Uint8, true };
     schema.items[ "visible" ] = DataItem{ StoredDataType::Bool, true };
     schema.items[ "texrect" ] = DataItem{ StoredDataType::FloatRect, true };
     writer.set_bool( "visible", true );
     writer.set_float_rect( "texrect", FloatRect{ Vec2f{ 0.0f, 0.0f }, 0.0f, 0.0f } );

     return schema;
}


DataSchema make_render_system_schema( RenderSystemType type, IPropsWriter& writer )
{
     DataSchema schema;

     switch ( type )
     {
          case RenderSystemType::QTreeRenderSystem:
               schema.items[ "quad_start" ] = DataItem{ StoredDataType::Vec2f, true };
               schema.items[ "quad_size" ] = DataItem{ StoredDataType::Vec2f, true };
               schema.items[ "size" ] = DataItem{ StoredDataType::Vec2i, true };
               break;
          default:
               break;
     }

     return schema;
}

} // namespace _16nar::tools::constructor2d