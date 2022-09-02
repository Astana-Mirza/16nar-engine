#include <compiler.h>

#include <QJsonDocument>
#include <QJsonArray>

#include <numeric>

Compiler::Compiler( const QString& input_file ) : data_pos_{ 0 }
{
     QFile file( input_file );

     file.open( QIODevice::ReadOnly | QIODevice::Text );
     QString dataString = file.readAll();

     QJsonParseError err;
     QJsonDocument doc{ QJsonDocument::fromJson( dataString.toUtf8(), &err ) };
     if ( err.error != QJsonParseError::NoError )
     {
          throw std::runtime_error{ ( QObject::tr( "wrong JSON file: " ) +
                                      err.errorString() ).toStdString() };
     }
     main_object_ = doc.object();
}


template <>
uint32_t Compiler::save_data< QString >( const QString& data )
{
     if ( data.isEmpty() )
     {
          return 0;
     }
     uint32_t ret = data_pos_;
     data_.push_back( std::make_unique< SceneData< std::string > >( data.toStdString() ) );
     data_pos_ += data_.back()->size();
     return ret;
}


void Compiler::compile_package( const QString& output_file )
{
     out_file_.open( output_file.toStdString(), std::ios_base::binary );

     _16nar::ResourceFileHeader hdr{};
     QJsonArray resources = main_object_[ "resources" ].toArray();
     hdr.res_count = resources.size();

     out_file_.write( reinterpret_cast< char * >( &hdr ), sizeof( hdr ) );

     uint32_t res_offset = sizeof( hdr ) + hdr.res_count * sizeof( _16nar::ResourceSign );
     for ( const auto& res : resources )
     {
          QJsonObject json = res.toObject();
          _16nar::ResourceSign info{};
          QString type = json[ "type" ].toString();
          info.offset = res_offset;
          info.size = 0;
          if ( type == "VFShader" || type == "VGFShader" )
          {
               QStringList files = json[ "file" ].toString().split( ',', Qt::SkipEmptyParts );
               for ( const auto& filename : files )
               {
                    info.size += QFile( filename ).size() + 1; // +1 for \0, because shader is a string
               }
          }
          else
          {
               info.size = QFile( json[ "file" ].toString() ).size();
          }

          if ( type == "Texture" )
          {
               info.type = _16nar::ResourceType::Texture;
          }
          else if ( type == "Sound" )
          {
               info.type = _16nar::ResourceType::Sound;
          }
          else if ( type == "Font" )
          {
               info.type = _16nar::ResourceType::Font;
          }
          else if ( type == "VertexShader" )
          {
               info.type = _16nar::ResourceType::VertexShader;
               info.size += 1;     // +1 for \0, because shader is a string
          }
          else if ( type == "GeometryShader" )
          {
               info.type = _16nar::ResourceType::GeometryShader;
               info.size += 1;     // +1 for \0, because shader is a string
          }
          else if ( type == "FragmentShader" )
          {
               info.type = _16nar::ResourceType::FragmentShader;
               info.size += 1;     // +1 for \0, because shader is a string
          }
          else if ( type == "VFShader" )
          {
               info.type = _16nar::ResourceType::VFShader;
          }
          else if ( type == "VGFShader" )
          {
               info.type = _16nar::ResourceType::VGFShader;
          }
          out_file_.write( reinterpret_cast< char * >( &info ), sizeof( info ) );
          res_offset += info.size;
     }

     for ( const auto& res : resources )
     {
          QJsonObject json = res.toObject();
          QString type = json[ "type" ].toString();
          if ( type == "VFShader" || type == "VGFShader" )
          {
               QStringList files = json[ "file" ].toString().split( ',', Qt::SkipEmptyParts );
               for ( const auto& filename : files )
               {
                    QFile res_file( filename );
                    if ( !res_file.open( QIODevice::ReadOnly ) )
                    {
                         throw std::runtime_error{ ( QObject::tr( "cannot open file " ) +
                                                     json[ "file" ].toString() ).toStdString() };
                    }
                    QByteArray content = res_file.readAll();
                    out_file_.write( content.data(), content.size() );
                    out_file_.put( '\0' );
               }
          }
          else
          {
               QFile res_file( json[ "file" ].toString() );
               if ( !res_file.open( QIODevice::ReadOnly ) )
               {
                    throw std::runtime_error{ ( QObject::tr( "cannot open file " ) +
                                                json[ "file" ].toString() ).toStdString() };
               }
               QByteArray content = res_file.readAll();
               out_file_.write( content.data(), content.size() );
               if ( type.endsWith( "Shader" ) )
               {
                    out_file_.put( '\0' );
               }
          }
     }

     out_file_.close();
}


void Compiler::compile_scene( const QString& output_file )
{
     _16nar::SceneHeader hdr{};
     hdr.main_code_file = main_object_[ "main_code_file" ].toInt();
     hdr.code_file_count = main_object_[ "code_files" ].toArray().size();
     hdr.resource_file_count = main_object_[ "resource_packages" ].toArray().size();
     hdr.state_count = main_object_[ "states" ].toArray().size();

     count_offsets( hdr );
     hdr.setup_off = save_data( main_object_[ "setup_func" ].toString() );
     hdr.loop_off = save_data( main_object_[ "loop_func" ].toString() );

     out_file_.open( output_file.toStdString(), std::ios_base::binary );

     out_file_.write( reinterpret_cast< char * >( &hdr ), sizeof( _16nar::SceneHeader ) );
     write_states();
     write_nodes();
     write_code_files();
     write_resources();
     write_data();

     out_file_.close();
}


void Compiler::count_offsets( _16nar::SceneHeader& hdr )
{
     uint32_t offset = sizeof( _16nar::SceneHeader );       // skip header
     hdr.sections.state_off = offset;

     QJsonArray state_infos = main_object_[ "states" ].toArray();     // skip states section: n StateInfo structures
     offset += static_cast< uint32_t >( state_infos.size() * sizeof( _16nar::StateInfo ) );
     hdr.sections.tree_off = offset;
     for ( const auto& state : state_infos )              // skip tree section: n NodeInfo structures for each state
     {
          offset += static_cast< uint32_t >( state.toObject()[ "nodes" ].toArray().size() * sizeof( _16nar::NodeInfo ) );
     }
     hdr.sections.code_off = offset;

     // skip code section: n offsets to code names, each offset is uint32_t
     offset += static_cast< uint32_t >( main_object_[ "code_files" ].toArray().size() * sizeof( uint32_t ) );
     hdr.sections.rsrc_off = offset;

     // skip rsrc section: n ResourceTableEntry structures, m resource IDs (uint16_t) from each table entry
     QJsonArray package_infos = main_object_[ "resource_packages" ].toArray();
     offset += static_cast< uint32_t >( package_infos.size() * sizeof( _16nar::ResourceTableEntry ) );
     for ( const auto& package : package_infos )
     {
          offset += static_cast< uint32_t >( package.toObject()[ "resources" ].toArray().size() * sizeof( uint16_t ) );
     }

     data_pos_ = offset;
}


void Compiler::write_states()
{
     QJsonArray state_infos = main_object_[ "states" ].toArray();
     for ( const auto& state : state_infos )
     {
          _16nar::StateInfo info{};
          QJsonObject json = state.toObject();
          info.q_start[ 0 ] = json[ "q_start" ].toArray()[ 0 ].toDouble();
          info.q_start[ 1 ] = json[ "q_start" ].toArray()[ 1 ].toDouble();
          info.gravity_vec[ 0 ] = json[ "gravity_vec" ].toArray()[ 0 ].toDouble();
          info.gravity_vec[ 1 ] = json[ "gravity_vec" ].toArray()[ 1 ].toDouble();
          info.q_size[ 0 ] = json[ "q_size" ].toArray()[ 0 ].toDouble();
          info.q_size[ 1 ] = json[ "q_size" ].toArray()[ 1 ].toDouble();
          info.view_rect_pos[ 0 ] = json[ "view_rect_pos" ].toArray()[ 0 ].toDouble();
          info.view_rect_pos[ 1 ] = json[ "view_rect_pos" ].toArray()[ 1 ].toDouble();
          info.view_rect_size[ 0 ] = json[ "view_rect_size" ].toArray()[ 0 ].toDouble();
          info.view_rect_size[ 1 ] = json[ "view_rect_size" ].toArray()[ 1 ].toDouble();
          info.pixels_per_meter = json[ "pixels_per_meter" ].toDouble();
          info.scene_size[ 0 ] = json[ "scene_size" ].toArray()[ 0 ].toInt();
          info.scene_size[ 1 ] = json[ "scene_size" ].toArray()[ 1 ].toInt();
          info.node_count = json[ "nodes" ].toArray().size();
          info.order = json[ "order" ].toInt();
          info.updating = json[ "updating" ].toBool();
          info.rendering = json[ "rendering" ].toBool();

          out_file_.write( reinterpret_cast< char * >( &info ), sizeof( info ) );
     }
}


void Compiler::write_nodes()
{
     QJsonArray state_infos = main_object_[ "states" ].toArray();
     for ( const auto& state : state_infos )
     {
          uint32_t state_subtree_start = out_file_.tellp();
          QJsonArray node_array = state.toObject()[ "nodes" ].toArray();
          for ( const auto& node : node_array )
          {
               _16nar::NodeInfo info{};
               QJsonObject json = node.toObject();
               qint64 parent_index = json[ "parent" ].toInt();
               info.parent = parent_index == -1 ? 0 : static_cast< uint32_t >( state_subtree_start +
                                                      parent_index * sizeof( _16nar::NodeInfo ) );
               info.name_off = save_data( json[ "name" ].toString() );
               info.pos[ 0 ] = json[ "pos" ].toArray()[ 0 ].toDouble();
               info.pos[ 1 ] = json[ "pos" ].toArray()[ 1 ].toDouble();
               info.scale[ 0 ] = json[ "scale" ].toArray()[ 0 ].toDouble();
               info.scale[ 1 ] = json[ "scale" ].toArray()[ 1 ].toDouble();
               info.origin[ 0 ] = json[ "origin" ].toArray()[ 0 ].toDouble();
               info.origin[ 1 ] = json[ "origin" ].toArray()[ 1 ].toDouble();
               info.rotation = json[ "rotation" ].toDouble();
               info.code_file_num = json[ "code_file_num" ].toInt();
               info.creator_name_off = save_data( json[ "creator_name" ].toString() );
               fill_node_by_type( info, json );

               out_file_.write( reinterpret_cast< char * >( &info ), sizeof( info ) );
          }
     }
}


void Compiler::write_code_files()
{
     QJsonArray code_files = main_object_[ "code_files" ].toArray();
     for ( const auto& filename : code_files )
     {
          uint32_t offset = save_data( filename.toString() );
          out_file_.write( reinterpret_cast< char * >( &offset ), sizeof( uint32_t ) );
     }
}


void Compiler::write_resources()
{
     QJsonArray package_infos = main_object_[ "resource_packages" ].toArray();
     uint32_t res_array_offset = static_cast< uint32_t >( out_file_.tellp() ) +
                                 static_cast< uint32_t >( package_infos.size() *
                                                          sizeof( _16nar::ResourceTableEntry ) );
     for ( const auto& package : package_infos )
     {
          _16nar::ResourceTableEntry info{};
          QJsonObject json = package.toObject();
          info.filename_off = save_data( json[ "filename" ].toString() );
          info.res_array_off = res_array_offset;
          info.res_count = json[ "resources" ].toArray().size();
          info.file_id = json[ "file_id" ].toInt();

          res_array_offset += info.res_count * sizeof( uint16_t );
          out_file_.write( reinterpret_cast< char * >( &info ), sizeof( info ) );
     }

     for ( const auto& package : package_infos )
     {
          QJsonArray resources = package.toObject()[ "resources" ].toArray();
          for ( const auto& res_id : resources )
          {
               uint16_t id = res_id.toInt();
               out_file_.write( reinterpret_cast< char * >( &id ), sizeof( id ) );
          }
     }
}


void Compiler::fill_node_by_type( _16nar::NodeInfo& info, QJsonObject& json )
{
     int max_id = static_cast< int >( std::numeric_limits< uint16_t >::max() );
     QString type = json[ "type" ].toString();
     if ( type == "Node2D" )
     {
          info.node_type = _16nar::NodeType::Node2D;
     }
     else if ( type == "SpriteNode" )
     {
          QJsonArray blend = json[ "blend" ].toArray();
          int file_id = json[ "res" ].toArray()[ 0 ].toInt();
          int rsrc_id = json[ "res" ].toArray()[ 1 ].toInt();
          info.node_type = _16nar::NodeType::SpriteNode;
          info.sprite_inf.res.file_id = ( file_id >= 0 && file_id < max_id ) ? file_id : max_id;
          info.sprite_inf.res.rsrc_id = ( rsrc_id >= 0 && rsrc_id < max_id ) ? rsrc_id : max_id;
          file_id = json[ "shader" ].toArray()[ 0 ].toInt();
          rsrc_id = json[ "shader" ].toArray()[ 1 ].toInt();
          info.sprite_inf.shader.file_id = ( file_id >= 0 && file_id < max_id ) ? file_id : max_id;
          info.sprite_inf.shader.rsrc_id = ( rsrc_id >= 0 && rsrc_id < max_id ) ? rsrc_id : max_id;
          for ( size_t i = 0; i < sizeof( info.sprite_inf.blend ) / sizeof( int ); i++ )
          {
               info.sprite_inf.blend[ i ] = blend[ i ].toInt();
          }
          info.sprite_inf.color = json[ "color" ].toString().toUInt( nullptr, 16 );        // number in hex
          info.sprite_inf.layer = json[ "layer" ].toInt();
          info.sprite_inf.visible = json[ "visible" ].toBool();
          info.sprite_inf.rect_coords[ 0 ] = json[ "rect_coords" ].toArray()[ 0 ].toInt();
          info.sprite_inf.rect_coords[ 1 ] = json[ "rect_coords" ].toArray()[ 1 ].toInt();
          info.sprite_inf.rect_size[ 0 ] = json[ "rect_size" ].toArray()[ 0 ].toInt();
          info.sprite_inf.rect_size[ 1 ] = json[ "rect_size" ].toArray()[ 1 ].toInt();
     }
     else if ( type == "SoundNode" )
     {
          int file_id = json[ "res" ].toArray()[ 0 ].toInt();
          int rsrc_id = json[ "res" ].toArray()[ 1 ].toInt();
          info.node_type = _16nar::NodeType::SoundNode;
          info.sound_inf.res.file_id = ( file_id >= 0 && file_id < max_id ) ? file_id : max_id;
          info.sound_inf.res.rsrc_id = ( rsrc_id >= 0 && rsrc_id < max_id ) ? rsrc_id : max_id;
          info.sound_inf.z_coord = json[ "z_coord" ].toDouble();
          info.sound_inf.offset = json[ "offset" ].toDouble();
          info.sound_inf.min_distance = json[ "min_distance" ].toDouble();
          info.sound_inf.volume = json[ "volume" ].toDouble();
          info.sound_inf.pitch = json[ "pitch" ].toDouble();
          info.sound_inf.attenuation = json[ "attenuation" ].toDouble();
          info.sound_inf.relative_to_listener = json[ "relative_to_listener" ].toBool();
          info.sound_inf.loop = json[ "loop" ].toBool();
     }
     else if ( type == "TextNode" )
     {
          QJsonArray blend = json[ "blend" ].toArray();
          int file_id = json[ "res" ].toArray()[ 0 ].toInt();
          int rsrc_id = json[ "res" ].toArray()[ 1 ].toInt();
          info.node_type = _16nar::NodeType::TextNode;
          info.text_inf.res.file_id = ( file_id >= 0 && file_id < max_id ) ? file_id : max_id;
          info.text_inf.res.rsrc_id = ( rsrc_id >= 0 && rsrc_id < max_id ) ? rsrc_id : max_id;
          file_id = json[ "shader" ].toArray()[ 0 ].toInt();
          rsrc_id = json[ "shader" ].toArray()[ 1 ].toInt();
          info.sprite_inf.shader.file_id = ( file_id >= 0 && file_id < max_id ) ? file_id : max_id;
          info.sprite_inf.shader.rsrc_id = ( rsrc_id >= 0 && rsrc_id < max_id ) ? rsrc_id : max_id;
          for ( size_t i = 0; i < sizeof( info.sprite_inf.blend ) / sizeof( int ); i++ )
          {
               info.sprite_inf.blend[ i ] = blend[ i ].toInt();
          }
          info.text_inf.color = json[ "color" ].toString().toUInt( nullptr, 16 );        // number in hex
          info.text_inf.layer = json[ "layer" ].toInt();
          info.text_inf.visible = json[ "visible" ].toBool();
          info.text_inf.string_offset = save_data( json[ "string" ].toString() );
          info.text_inf.char_size = json[ "char_size" ].toInt();
          info.text_inf.style = json[ "style" ].toInt();
          info.text_inf.outline_color = json[ "outline_color" ].toString().toUInt( nullptr, 16 ); // number in hex
          info.text_inf.outline_thickness = json[ "outline_thickness" ].toDouble();
          info.text_inf.line_spacing = json[ "line_spacing" ].toDouble();
          info.text_inf.letter_spacing = json[ "letter_spacing" ].toDouble();
     }
     else if ( type == "TilemapNode" )
     {
          QJsonArray blend = json[ "blend" ].toArray();
          int file_id = json[ "res" ].toArray()[ 0 ].toInt();
          int rsrc_id = json[ "res" ].toArray()[ 1 ].toInt();
          info.node_type = _16nar::NodeType::TilemapNode;
          info.tilemap_inf.res.file_id = ( file_id >= 0 && file_id < max_id ) ? file_id : max_id;
          info.tilemap_inf.res.rsrc_id = ( rsrc_id >= 0 && rsrc_id < max_id ) ? rsrc_id : max_id;
          file_id = json[ "shader" ].toArray()[ 0 ].toInt();
          rsrc_id = json[ "shader" ].toArray()[ 1 ].toInt();
          info.tilemap_inf.shader.file_id = ( file_id >= 0 && file_id < max_id ) ? file_id : max_id;
          info.tilemap_inf.shader.rsrc_id = ( rsrc_id >= 0 && rsrc_id < max_id ) ? rsrc_id : max_id;
          for ( size_t i = 0; i < sizeof( info.tilemap_inf.blend ) / sizeof( int ); i++ )
          {
               info.tilemap_inf.blend[ i ] = blend[ i ].toInt();
          }
          info.tilemap_inf.color = json[ "color" ].toString().toUInt( nullptr, 16 );        // number in hex
          info.tilemap_inf.layer = json[ "layer" ].toInt();
          info.tilemap_inf.visible = json[ "visible" ].toBool();
          info.tilemap_inf.tiles_count = json[ "tiles" ].toArray().size();
          info.tilemap_inf.tiles_offset = save_tiles( json );
     }
}


uint32_t Compiler::save_tiles( QJsonObject& json )
{
     uint32_t ret = data_pos_;
     QJsonArray tiles = json[ "tiles" ].toArray();
     for ( const auto& tile_json_ref : tiles )
     {
          QJsonObject tile_json = tile_json_ref.toObject();
          QJsonArray vertices = tile_json[ "vertices" ].toArray();
          QJsonArray positions = tile_json[ "positions" ].toArray();
          _16nar::TileInfo info{};
          info.vertex_count = vertices.size();
          info.copy_count = positions.size();
          info.type = tile_json[ "type" ].toInt();
          save_data( info );

          auto vert_data = save_array< float[ 2 ] >( vertices.size() ).first;
          for ( uint32_t i = 0; i < vertices.size(); i++ )
          {
               vert_data[ i ][ 0 ] = vertices[ i ].toArray()[ 0 ].toDouble();
               vert_data[ i ][ 1 ] = vertices[ i ].toArray()[ 1 ].toDouble();
          }

          auto copy_data = save_array< float[ 2 ] >( positions.size() ).first;
          for ( uint32_t i = 0; i < positions.size(); i++ )
          {
               copy_data[ i ][ 0 ] = positions[ i ].toArray()[ 0 ].toDouble();
               copy_data[ i ][ 1 ] = positions[ i ].toArray()[ 1 ].toDouble();
          }
     }
     return ret;
}


void Compiler::write_data()
{
     for ( const auto& ptr : data_ )
     {
          ptr->write_binary( out_file_ );
     }
}
