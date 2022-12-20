#include <compiler.h>

#include <numeric>

namespace
{
    constexpr static int MAX_ID = static_cast< int >( std::numeric_limits< uint16_t >::max() );
}

Compiler::Compiler( const std::string& input_file ) : data_pos_{ 0 }
{
     using namespace std::placeholders;

     std::ifstream file( input_file );
     if ( !file.is_open() )
     {
         throw std::runtime_error{ "cannot open file " + input_file };
     }
     main_object_ = nl::json::parse( file );

     node_compilers_[ "Node2D" ] = std::bind( std::mem_fn( &Compiler::fill_node2d ), this, _1, _2 );
     node_compilers_[ "SpriteNode" ] = std::bind( std::mem_fn( &Compiler::fill_sprite_node ), this, _1, _2 );
     node_compilers_[ "SoundNode" ] = std::bind( std::mem_fn( &Compiler::fill_sound_node ), this, _1, _2 );
     node_compilers_[ "TextNode" ] = std::bind( std::mem_fn( &Compiler::fill_text_node ), this, _1, _2 );
     node_compilers_[ "TilemapNode" ] = std::bind( std::mem_fn( &Compiler::fill_tilemap_node ), this, _1, _2 );
}


template <>
uint32_t Compiler::save_data< std::string >( const std::string& data )
{
     if ( data.empty() )
     {
          return 0;
     }
     uint32_t ret = data_pos_;
     data_.push_back( std::make_unique< SceneData< std::string > >( data ) );
     data_pos_ += data_.back()->size();
     return ret;
}


void Compiler::compile_package( const std::string& output_file )
{
     out_file_.open( output_file, std::ios_base::binary );

     _16nar::ResourceFileHeader hdr{};
     auto resources = main_object_[ "resources" ].get< std::vector< nl::json > >();
     hdr.res_count = static_cast< uint32_t >( resources.size() );

     out_file_.write( reinterpret_cast< char * >( &hdr ), sizeof( hdr ) );

     uint32_t res_offset = sizeof( hdr ) + hdr.res_count * sizeof( _16nar::ResourceSign );
     for ( const auto& res : resources )
     {
          _16nar::ResourceSign info{};
          std::string type = res[ "type" ].get< std::string >();
          info.offset = res_offset;
          info.size = 0;
          if ( type == "VFShader" || type == "VGFShader" )
          {
               auto files = res[ "file" ].get< std::vector< std::string > >();
               for ( const auto& filename : files )
               {
                    info.size += get_file_size( filename ) + 1; // +1 for \0, because shader is a string
               }
          }
          else
          {
               info.size = get_file_size( res[ "file" ].get< std::string >() );
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
          auto type = res[ "type" ].get< std::string >();
          if ( type == "VFShader" || type == "VGFShader" )
          {
               auto files = res[ "file" ].get< std::vector< std::string > >();
               for ( const auto& filename : files )
               {
                    std::ifstream res_file( filename, std::ios::binary );
                    if ( !res_file.is_open() )
                    {
                         throw std::runtime_error{ "cannot open file " + filename };
                    }
                    std::string content{ std::istreambuf_iterator< char >( res_file ), std::istreambuf_iterator< char >() };

                    out_file_.write( content.data(), content.size() );
                    out_file_.put( '\0' );
               }
          }
          else
          {
               std::ifstream res_file( res[ "file" ].get< std::string >(), std::ios::binary );
               if ( !res_file.is_open() )
               {
                   throw std::runtime_error{ "cannot open file " + res[ "file" ].get< std::string >() };
               }

               std::string content{ std::istreambuf_iterator< char >( res_file ), std::istreambuf_iterator< char >() };
               out_file_.write( content.data(), content.size() );
               if ( type.find( "Shader" ) != std::string::npos )
               {
                    out_file_.put( '\0' );
               }
          }
     }
     out_file_.close();
}


void Compiler::compile_scene( const std::string& output_file )
{
     _16nar::SceneHeader hdr{};
     hdr.main_code_file = main_object_[ "main_code_file" ].get< uint32_t >();
     hdr.code_file_count = static_cast< uint32_t >( main_object_[ "code_files" ]
                                                    .get< std::vector< nl::json > >().size() );
     hdr.resource_file_count = static_cast< uint32_t >( main_object_[ "resource_packages" ]
                                                      .get< std::vector< nl::json > >().size() );
     hdr.state_count = static_cast< uint32_t >( main_object_[ "states" ]
                                                .get< std::vector< nl::json > >().size() );

     count_offsets( hdr );
     hdr.setup_off = save_data( main_object_[ "setup_func" ].get< std::string >() );
     hdr.loop_off = save_data( main_object_[ "loop_func" ].get< std::string >() );

     out_file_.open( output_file, std::ios_base::binary );

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

     auto state_infos = main_object_[ "states" ].get< std::vector< nl::json > >();     // skip states section: n StateInfo structures
     offset += static_cast< uint32_t >( state_infos.size() * sizeof( _16nar::StateInfo ) );
     hdr.sections.tree_off = offset;
     for ( const auto& state : state_infos )              // skip tree section: n NodeInfo structures for each state
     {
          offset += static_cast< uint32_t >( state[ "nodes" ].get< std::vector< nl::json > >().size() *
                                             sizeof( _16nar::NodeInfo ) );
     }
     hdr.sections.code_off = offset;

     // skip code section: n offsets to code names, each offset is uint32_t
     offset += static_cast< uint32_t >( main_object_[ "code_files" ].get< std::vector< nl::json > >().size() *
                                        sizeof( uint32_t ) );
     hdr.sections.rsrc_off = offset;

     // skip rsrc section: n ResourceTableEntry structures, m resource IDs (uint16_t) from each table entry
     auto package_infos = main_object_[ "resource_packages" ].get< std::vector< nl::json > >();
     offset += static_cast< uint32_t >( package_infos.size() * sizeof( _16nar::ResourceTableEntry ) );
     for ( const auto& package : package_infos )
     {
          offset += static_cast< uint32_t >( package[ "resources" ].get< std::vector< nl::json > >().size() *
                                             sizeof( uint16_t ) );
     }

     data_pos_ = offset;
}


void Compiler::write_states()
{
     auto state_infos = main_object_[ "states" ].get< std::vector< nl::json > >();
     for ( const auto& state : state_infos )
     {
          _16nar::StateInfo info{};
          info.q_start[ 0 ] = state[ "q_start" ].get< std::vector< float > >()[ 0 ];
          info.q_start[ 1 ] = state[ "q_start" ].get< std::vector< float > >()[ 1 ];
          info.gravity_vec[ 0 ] = state[ "gravity_vec" ].get< std::vector< float > >()[ 0 ];
          info.gravity_vec[ 1 ] = state[ "gravity_vec" ].get< std::vector< float > >()[ 1 ];
          info.q_size[ 0 ] = state[ "q_size" ].get< std::vector< float > >()[ 0 ];
          info.q_size[ 1 ] = state[ "q_size" ].get< std::vector< float > >()[ 1 ];
          info.view_rect_pos[ 0 ] = state[ "view_rect_pos" ].get< std::vector< float > >()[ 0 ];
          info.view_rect_pos[ 1 ] = state[ "view_rect_pos" ].get< std::vector< float > >()[ 1 ];
          info.view_rect_size[ 0 ] = state[ "view_rect_size" ].get< std::vector< float > >()[ 0 ];
          info.view_rect_size[ 1 ] = state[ "view_rect_size" ].get< std::vector< float > >()[ 1 ];
          info.pixels_per_meter = state[ "pixels_per_meter" ].get< float >();
          info.scene_size[ 0 ] = state[ "scene_size" ].get< std::vector< int > >()[ 0 ];
          info.scene_size[ 1 ] = state[ "scene_size" ].get< std::vector< int > >()[ 1 ];
          info.node_count = static_cast< uint32_t >( state[ "nodes" ].get< std::vector< nl::json > >().size() );
          info.order = state[ "order" ].get< int >();
          info.updating = state[ "updating" ].get< bool >();
          info.rendering = state[ "rendering" ].get< bool >();

          out_file_.write( reinterpret_cast< char * >( &info ), sizeof( info ) );
     }
}


void Compiler::write_nodes()
{
     nl::json state_infos = main_object_[ "states" ].get< std::vector< nl::json > >();
     for ( const auto& state : state_infos )
     {
          uint32_t state_subtree_start = static_cast< uint32_t >( out_file_.tellp() );
          nl::json node_array = state[ "nodes" ].get< std::vector< nl::json > >();
          for ( const auto& node : node_array )
          {
               _16nar::NodeInfo info{};
               int parent_index = node[ "parent" ].get< int >();
               info.parent = parent_index == -1 ? 0 : static_cast< uint32_t >( state_subtree_start +
                                                      parent_index * sizeof( _16nar::NodeInfo ) );
               info.name_off = save_data( node[ "name" ].get< std::string >() );
               info.pos[ 0 ] = node[ "pos" ].get< std::vector< float > >()[ 0 ];
               info.pos[ 1 ] = node[ "pos" ].get< std::vector< float > >()[ 1 ];
               info.scale[ 0 ] = node[ "scale" ].get< std::vector< float > >()[ 0 ];
               info.scale[ 1 ] = node[ "scale" ].get< std::vector< float > >()[ 1 ];
               info.origin[ 0 ] = node[ "origin" ].get< std::vector< float > >()[ 0 ];
               info.origin[ 1 ] = node[ "origin" ].get< std::vector< float > >()[ 1 ];
               info.rotation = node[ "rotation" ].get< float >();
               info.code_file_num = node[ "code_file_num" ].get< int >();
               info.creator_name_off = save_data( node[ "creator_name" ].get< std::string >() );

               std::string type = node[ "type" ].get< std::string >();
               node_compilers_.at( type )( info, node );

               out_file_.write( reinterpret_cast< char * >( &info ), sizeof( info ) );
          }
     }
}


void Compiler::write_code_files()
{
     auto code_files = main_object_[ "code_files" ].get< std::vector< std::string > >();
     for ( const auto& filename : code_files )
     {
          uint32_t offset = save_data( filename );
          out_file_.write( reinterpret_cast< char * >( &offset ), sizeof( uint32_t ) );
     }
}


void Compiler::write_resources()
{
     auto package_infos = main_object_[ "resource_packages" ].get< std::vector< nl::json > >();
     uint32_t res_array_offset = static_cast< uint32_t >( out_file_.tellp() ) +
                                 static_cast< uint32_t >( package_infos.size() *
                                                          sizeof( _16nar::ResourceTableEntry ) );
     for ( const auto& package : package_infos )
     {
          _16nar::ResourceTableEntry info{};
          info.filename_off = save_data( package[ "filename" ].get< std::string >() );
          info.res_array_off = res_array_offset;
          info.res_count = static_cast< uint16_t >( package[ "resources" ].get< std::vector< nl::json > >().size() );
          info.file_id = package[ "file_id" ].get< uint16_t >();

          res_array_offset += info.res_count * sizeof( uint16_t );
          out_file_.write( reinterpret_cast< char * >( &info ), sizeof( info ) );
     }

     for ( const auto& package : package_infos )
     {
          auto resources = package[ "resources" ].get< std::vector< uint16_t > >();
          for ( const auto& res_id : resources )
          {
               out_file_.write( reinterpret_cast< const char * >( &res_id), sizeof( res_id ) );
          }
     }
}


void Compiler::fill_node2d( _16nar::NodeInfo& info, const nl::json& node )
{
    info.node_type = _16nar::NodeType::Node2D;
}


void Compiler::fill_drawable_node( _16nar::NodeInfo& info, const nl::json& node )
{
     auto blend  = node[ "blend" ].get< std::vector< int > >();
     int file_id = node[ "res" ].get< std::vector< uint16_t > >()[ 0 ];
     int rsrc_id = node[ "res" ].get< std::vector< uint16_t > >()[ 1 ];

     // fields in union have common base, so we can use sprite_inf here for any drawable node.
     info.sprite_inf.res.file_id = ( file_id >= 0 && file_id < MAX_ID ) ? file_id : MAX_ID;
     info.sprite_inf.res.rsrc_id = ( rsrc_id >= 0 && rsrc_id < MAX_ID ) ? rsrc_id : MAX_ID;
     file_id = node[ "shader" ].get< std::vector< uint16_t > >()[ 0 ];
     rsrc_id = node[ "shader" ].get< std::vector< uint16_t > >()[ 1 ];
     info.sprite_inf.shader.file_id = ( file_id >= 0 && file_id < MAX_ID ) ? file_id : MAX_ID;
     info.sprite_inf.shader.rsrc_id = ( rsrc_id >= 0 && rsrc_id < MAX_ID ) ? rsrc_id : MAX_ID;
     for ( size_t i = 0; i < sizeof( info.sprite_inf.blend ) / sizeof( int ); i++ )
     {
         info.sprite_inf.blend[ i ] = blend[ i ];
     }

     std::stringstream color_ss;
     color_ss << std::hex << node[ "color" ].get< std::string >();
     color_ss >> info.sprite_inf.color;        // number in hex

     info.sprite_inf.layer = node[ "layer" ].get< int >();
     info.sprite_inf.visible = node[ "visible" ].get< bool >();
}


void Compiler::fill_sprite_node( _16nar::NodeInfo& info, const nl::json& node )
{
     fill_drawable_node( info, node );

     info.node_type = _16nar::NodeType::SpriteNode;
     info.sprite_inf.rect_coords[ 0 ] = node[ "rect_coords" ].get< std::vector< int > >()[ 0 ];
     info.sprite_inf.rect_coords[ 1 ] = node[ "rect_coords" ].get< std::vector< int > >()[ 1 ];
     info.sprite_inf.rect_size[ 0 ] = node[ "rect_size" ].get< std::vector< int > >()[ 0 ];
     info.sprite_inf.rect_size[ 1 ] = node[ "rect_size" ].get< std::vector< int > >()[ 1 ];
 }


void Compiler::fill_sound_node( _16nar::NodeInfo& info, const nl::json& node )
{
     info.node_type = _16nar::NodeType::SoundNode;
     int file_id = node[ "res" ].get< std::vector< uint16_t > >()[ 0 ];
     int rsrc_id = node[ "res" ].get< std::vector< uint16_t > >()[ 1 ];
     info.sound_inf.res.file_id = ( file_id >= 0 && file_id < MAX_ID ) ? file_id : MAX_ID;
     info.sound_inf.res.rsrc_id = ( rsrc_id >= 0 && rsrc_id < MAX_ID ) ? rsrc_id : MAX_ID;
     info.sound_inf.z_coord = node[ "z_coord" ].get< float >();
     info.sound_inf.offset = node[ "offset" ].get< float >();
     info.sound_inf.min_distance = node[ "min_distance" ].get< float >();
     info.sound_inf.volume = node[ "volume" ].get< float >();
     info.sound_inf.pitch = node[ "pitch" ].get< float >();
     info.sound_inf.attenuation = node[ "attenuation" ].get< float >();
     info.sound_inf.relative_to_listener = node[ "relative_to_listener" ].get< bool >();
     info.sound_inf.loop = node[ "loop" ].get< bool >();
}


void Compiler::fill_text_node( _16nar::NodeInfo& info, const nl::json& node )
{
     fill_drawable_node( info, node );

     info.node_type = _16nar::NodeType::TextNode;
     info.text_inf.string_offset = save_data( node[ "string" ].get< std::string >() );
     info.text_inf.char_size = node[ "char_size" ].get< int >();
     info.text_inf.style = node[ "style" ].get< int >();

     std::stringstream color_ss;
     color_ss << std::hex << node[ "outline_color" ].get< std::string >();
     color_ss >> info.text_inf.outline_color;        // number in hex
     info.text_inf.outline_thickness = node[ "outline_thickness" ].get< float >();
     info.text_inf.line_spacing = node[ "line_spacing" ].get< float >();
     info.text_inf.letter_spacing = node[ "letter_spacing" ].get< float >();
}


void Compiler::fill_tilemap_node(_16nar::NodeInfo& info, const nl::json& node)
{
     fill_drawable_node( info, node );

     info.node_type = _16nar::NodeType::TilemapNode;
     info.tilemap_inf.tiles_count = static_cast< uint32_t >( node[ "tiles" ].get< std::vector< nl::json > >().size() );
     info.tilemap_inf.tiles_offset = save_tiles( node );
}


uint32_t Compiler::save_tiles( const nl::json& node )
{
     uint32_t ret = data_pos_;
     auto tiles = node[ "tiles" ].get< std::vector< nl::json > >();
     for ( const auto& tile_json : tiles )
     {
          auto vertices = tile_json[ "vertices" ].get< std::vector< nl::json > >();
          auto positions = tile_json[ "positions" ].get< std::vector< nl::json > >();
          _16nar::TileInfo info{};
          info.vertex_count = static_cast< uint32_t >( vertices.size() );
          info.copy_count = static_cast< uint32_t >( positions.size() );
          info.type = tile_json[ "type" ].get< int >();
          save_data( info );

          auto vert_data = save_array< float[ 2 ] >( static_cast< uint32_t >( vertices.size() ) ).first;
          for ( uint32_t i = 0; i < vertices.size(); i++ )
          {
               vert_data[ i ][ 0 ] = vertices[ i ].get< std::vector< float > >()[ 0 ];
               vert_data[ i ][ 1 ] = vertices[ i ].get< std::vector< float > >()[ 1 ];
          }

          auto copy_data = save_array< float[ 2 ] >( static_cast< uint32_t >( positions.size() ) ).first;
          for ( uint32_t i = 0; i < positions.size(); i++ )
          {
               copy_data[ i ][ 0 ] = positions[ i ].get< std::vector< float > >()[ 0 ];
               copy_data[ i ][ 1 ] = positions[ i ].get< std::vector< float > >()[ 1 ];
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


uint32_t Compiler::get_file_size( const std::string& filename ) const
{
    std::ifstream file( filename, std::ios_base::binary | std::ios_base::ate );
    return static_cast< uint32_t >( file.tellg() );
}
