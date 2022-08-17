#include <system/file_scene_reader.h>
#include <render/quadrant.h>
#include <game.h>

#include <constructor/node_2d.h>
#include <constructor/sprite_node.h>
#include <constructor/sound_node.h>
#include <constructor/text_node.h>

namespace _16nar
{

void FileSceneReader::set_scene( const std::string& name )
{
     file_stream_.open( name, std::ios::in | std::ios::binary );
}


void FileSceneReader::load_scene( WorldNode& world,
                                  WorldNode::SetupFuncPtr& setup_ptr,
                                  WorldNode::LoopFuncPtr& loop_ptr )
{
     IOGuard guard( file_stream_, std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit );
     SceneHeader hdr{};
     file_stream_.read( reinterpret_cast< char * >( &hdr ), sizeof( SceneHeader ) );

     auto states = std::make_unique< StateInfo[] >( hdr.state_count );
     file_stream_.seekg( hdr.sections.state_off );
     file_stream_.read( reinterpret_cast< char * >( states.get() ), sizeof( StateInfo ) * hdr.state_count );

     load_packages( hdr );
     load_code( hdr, setup_ptr, loop_ptr );

     file_stream_.seekg( hdr.sections.tree_off );
     for ( uint32_t i = 0; i < hdr.state_count; i++ )
     {
          FloatRect rect{ states[ i ].view_rect_pos[ 0 ], states[ i ].view_rect_pos[ 1 ],
                          states[ i ].view_rect_size[ 0 ], states[ i ].view_rect_size[ 1 ] };
          auto state_ptr = std::make_unique< SceneState >( rect, states[ i ].updating, states[ i ].rendering );

          FloatRect area( { states[ i ].q_start[ 0 ], states[ i ].q_start[ 1 ],
                    states[ i ].scene_size[ 0 ] * states[ i ].q_size[ 0 ],
                    states[ i ].scene_size[ 1 ] * states[ i ].q_size[ 1 ] } );
          Quadrant& root = state_ptr->get_root_quadrant();
          root = Quadrant( area );

          make_quadrants( root, Vector2f{ area.left, area.top },
                    states[ i ].q_size[ 0 ], states[ i ].q_size[ 1 ],
                    states[ i ].scene_size[ 0 ], states[ i ].scene_size[ 1 ] );

          load_state_nodes( states[i].node_count, root, *state_ptr );
          world.register_state( states[ i ].order, std::move( state_ptr ) );
     }
     node_offsets_.clear();
}


void FileSceneReader::load_packages( const SceneHeader& hdr )
{
     file_stream_.seekg( hdr.sections.rsrc_off );
     auto res = std::make_unique< ResourceTableEntry[] >( hdr.resource_file_count );
     file_stream_.read( reinterpret_cast< char * >( res.get() ),
                        sizeof( ResourceTableEntry ) * hdr.resource_file_count);

     for ( uint32_t i = 0; i < hdr.resource_file_count; i++ )
     {
          auto res_ids = std::make_shared< uint16_t[] >( res[ i ].res_count );
          file_stream_.seekg( res[ i ].res_array_off );
          file_stream_.read( reinterpret_cast< char * >( res_ids.get() ), sizeof( uint16_t ) * res[ i ].res_count );
          load_resources( read_string( res[ i ].filename_off ), res[ i ].file_id, res[ i ].res_count, res_ids );
     }
}


void FileSceneReader::load_resources( const std::string& filename, uint16_t file_id,
                                      uint16_t res_count, std::shared_ptr< uint16_t[] > res_ids )
{
     std::ifstream res_file;
     ResourceFileHeader hdr{};
     res_file.open( filename, std::ios::in | std::ios::binary );
     res_file.read( reinterpret_cast< char * >( &hdr ), sizeof( ResourceFileHeader ) );

     auto signs = std::make_unique< ResourceSign[] >( hdr.res_count );
     res_file.read( reinterpret_cast< char * >( signs.get() ), sizeof( ResourceSign ) * hdr.res_count );

     for ( uint16_t i = 0; i < res_count; i++ )
     {
          auto raw = std::make_unique< char[] >(signs[res_ids[i]].size);
          res_file.seekg( signs[ res_ids[ i ] ].offset );
          res_file.read( reinterpret_cast< char * >( raw.get() ), signs[ res_ids[ i ] ].size );
          switch ( signs[ res_ids[ i ] ].type )
          {
#ifdef USE_SFML
               case ResourceType::Texture:
               {
                    textures_[ { file_id, res_ids[ i ] } ] = Texture{};
                    if ( !textures_[ { file_id, res_ids[ i ] } ]
                         .loadFromMemory( raw.get(), signs[ res_ids[ i ] ].size ) )
                    {
                         throw std::runtime_error{ "cannot load texture from file " + filename };
                    }
			}
			break;
               case ResourceType::Sound:
               {
                    sounds_[ { file_id, res_ids[ i ] } ] = SoundBuffer{};
                    if ( !sounds_[ { file_id, res_ids[ i ] } ]
                         .loadFromMemory( raw.get(), signs[ res_ids[ i ] ].size ) )
                    {
                         throw std::runtime_error{ "cannot load sound buffer from file " + filename };
                    }
               }
               break;
               case ResourceType::Font:
               {
                    fonts_[ { file_id, res_ids[ i ] } ] = Font{};
                    if ( !fonts_[ { file_id, res_ids[ i ] } ]
                         .loadFromMemory( raw.get(), signs[ res_ids[ i ] ].size ) )
                    {
                         throw std::runtime_error{ "cannot load font from file " + filename };
                    }
               }
               break;
			default:
                    throw std::runtime_error{ "unknown resource type in file " + filename };
               break;
#endif // #ifdef USE_SFML
          }
     }
}


void FileSceneReader::load_code( const SceneHeader& hdr,
                                 WorldNode::SetupFuncPtr& setup_ptr,
                                 WorldNode::LoopFuncPtr& loop_ptr)
{
     if ( !hdr.sections.code_off )
     {
          return;
     }
     auto lib_offsets = std::make_shared< uint32_t[] >( hdr.code_file_count );
     file_stream_.seekg( hdr.sections.code_off );
     file_stream_.read( reinterpret_cast< char * >( lib_offsets.get() ), sizeof( uint32_t ) * hdr.code_file_count );
     load_libs( lib_offsets, hdr.code_file_count );
     lib_offsets.reset();          // free memory, it won't be used anymore

     // fill game's function pointers
     if ( hdr.setup_off )
     {
          auto setup_name = read_string( hdr.setup_off );
          setup_ptr = reinterpret_cast< WorldNode::SetupFuncPtr >(
               libs_.at( hdr.main_code_file ).get_symbol( setup_name ) );
	}

     if ( hdr.loop_off )
     {
          auto loop_name = read_string( hdr.loop_off );
          loop_ptr = reinterpret_cast< WorldNode::LoopFuncPtr >(
               libs_.at( hdr.main_code_file ).get_symbol( loop_name ) );
     }
}


void FileSceneReader::load_state_nodes( uint32_t count, Quadrant& quad, SceneState& state )
{
     auto nodes = std::make_unique< NodeInfo[] >( count );
     file_stream_.read( reinterpret_cast< char * >( nodes.get() ), sizeof( NodeInfo ) * count );
     for ( uint32_t i = 0; i < count; i++ )
     {
          create_node( nodes[ i ], static_cast< uint32_t >( file_stream_.tellg() ) +
                                   i * sizeof( NodeInfo ), quad, state );
     }
}


void FileSceneReader::make_quadrants( Quadrant& parent, const Vector2f& start,
                                      float q_width, float q_height,
                                      uint32_t wcount, uint32_t hcount )
{
     if ( wcount * hcount <= 1 )
     {
          return;
     }
     using QuadArrD1 = std::unique_ptr< Quadrant*[] >;
     using QuadArrD2 = std::unique_ptr< QuadArrD1[] >;
     // construction of quadrants
     QuadArrD2 quads = std::make_unique< QuadArrD1[] >( hcount );
     for ( uint32_t i = 0; i < hcount; i++ )
     {
          quads[ i ] = std::make_unique< Quadrant*[] >( wcount );
          for ( uint32_t j = 0; j < wcount; j++ )
          {
               quads[ i ][ j ] = new Quadrant( FloatRect{ start.x + j * q_width,
                                                          start.y + i * q_height,
                                                          q_width, q_height } );
          }
     }

     uint32_t current_wcount = wcount, current_hcount = hcount;
     while ( current_wcount > 2 || current_hcount > 2 )
     {
          uint32_t x = 0, y = 0;             // indexes of bigger quadrants in 2-dim. array
          uint32_t new_wcount = ( current_wcount + 1 ) / 2;
          uint32_t new_hcount = ( current_hcount + 1 ) / 2;
          uint32_t enlarge = 0;
          for ( size_t i = 0; i < current_hcount; i += 2 )
          {
               for ( size_t j = 0; j < current_wcount; j += 2 )
               {
                    Quadrant *bigger = new Quadrant( FloatRect{ start.x + x * q_width *  ( 2 << enlarge ),
                                                                start.y + y * q_height * ( 2 << enlarge ),
                                                                q_width * ( 2 << enlarge ),
                                                                q_height * ( 2 << enlarge ) } );
                    bigger->add_child( quads[ i ][ j ], 0 );
                    if ( j + 1 < current_wcount )
                    {
                         bigger->add_child( quads[ i ][ j + 1 ], 1 );
                    }
				if ( i + 1 < current_hcount )
                    {
                         bigger->add_child( quads[ i + 1 ][ j ], 2 );
                    }
                    if ( i + 1 < current_hcount && j + 1 < current_wcount )
                    {
                         bigger->add_child( quads[ i + 1 ][ j + 1 ], 3 );
                    }
                    quads[ x ][ y ] = bigger;
               }
               x++;
               if ( x == new_wcount )
               {
                    x = 0;
                    y++;
               }
          }
          enlarge++;
          current_wcount = new_wcount;
          current_hcount = new_hcount;
     }
     parent.add_child( quads[ 0 ][ 0 ], 0 );
     if ( 1 < current_wcount )
     {
          parent.add_child( quads[ 0 ][ 1 ], 1 );
     }
     if ( 1 < current_hcount )
     {
          parent.add_child( quads[ 1 ][ 0 ], 2 );
     }
     if ( 1 < current_hcount && 1 < current_wcount )
     {
          parent.add_child( quads[ 1 ][ 1 ], 3 );
     }
}


void FileSceneReader::create_node( const NodeInfo& info, uint32_t offset, Quadrant& quad, SceneState& state )
{
     Node *node = nullptr;
     switch ( info.node_type )
     {
          case NodeType::SpriteNode:
          {
               SpriteNode* sp_node = nullptr;
               IntRect texture_rect{ info.sprite_inf.rect_coords[ 0 ], info.sprite_inf.rect_coords[ 1 ],
                                     info.sprite_inf.rect_size[ 0 ], info.sprite_inf.rect_size[ 1 ] };
               if ( 0 != info.creator_name_off )
               {
                    void *func = libs_.at( info.code_file_num ).get_symbol( read_string( info.creator_name_off ) );
                    if ( check_resource( info.sprite_inf.res ) )
                    {
                         auto creator = reinterpret_cast< SpriteNode * ( * )( Quadrant *,
                                                                              const Texture&,
                                                                              const IntRect& ) >( func );
                         sp_node = creator( &quad, textures_.at( info.sprite_inf.res ), texture_rect );
                    }
                    else
                    {
                         auto creator = reinterpret_cast< SpriteNode * ( * )( Quadrant * ) >( func );
                         sp_node = creator( &quad );
                    }
               }
               else
               {
                    if ( !check_resource( info.sprite_inf.res ) )
                    {
                         sp_node = new SpriteNode( &quad );
                    }
                    else if ( texture_rect.height == 0 || texture_rect.width == 0 )
                    {
                         sp_node = new SpriteNode( &quad, textures_.at( info.sprite_inf.res ) );
                    }
                    else
                    {
                         sp_node = new SpriteNode( &quad, textures_.at( info.sprite_inf.res ), texture_rect );
                    }
               }
               sp_node->set_layer( info.sprite_inf.layer );
               sp_node->set_visible( info.sprite_inf.visible );
               sp_node->set_color( Color( info.sprite_inf.color ) );
               node = sp_node;
          }
          break;
          case NodeType::SoundNode:
          {
               SoundNode* so_node = nullptr;
               if ( 0 != info.creator_name_off )
               {
                    void* func = libs_.at( info.code_file_num ).get_symbol( read_string( info.creator_name_off ) );
                    if ( check_resource( info.sound_inf.res ) )
                    {
                         auto creator = reinterpret_cast< SoundNode * ( * )( const SoundBuffer& ) >( func );
                         so_node = creator( sounds_.at( info.sound_inf.res ) );
                    }
                    else
                    {
                         auto creator = reinterpret_cast< SoundNode * ( * )() >( func );
                         so_node = creator();
                    }
               }
               else if ( check_resource( info.sound_inf.res ) )
               {
                    so_node = new SoundNode( sounds_.at( info.sound_inf.res ) );
               }
               else
               {
                    so_node = new SoundNode();
               }
               so_node->set_attenuation( info.sound_inf.attenuation );
               so_node->set_loop( info.sound_inf.loop );
               so_node->set_min_distance( info.sound_inf.min_distance );
               so_node->set_offset( info.sound_inf.offset );
               so_node->set_pitch( info.sound_inf.pitch );
               so_node->set_relative_to_listener( info.sound_inf.relative_to_listener );
               so_node->set_volume( info.sound_inf.volume );
               so_node->set_z( info.sound_inf.z_coord );
               node = so_node;
          }
          break;
          case NodeType::TextNode:
          {
               TextNode* t_node = nullptr;
               if ( 0 != info.creator_name_off )
               {
                    void* func = libs_.at( info.code_file_num ).get_symbol( read_string( info.creator_name_off ) );
                    if ( check_resource( info.text_inf.res ) )
                    {
                         auto creator = reinterpret_cast< TextNode * ( * )( Quadrant *, const std::string&,
                                                                            const Font&, uint32_t ) >( func );
                         t_node = creator( &quad, read_string( info.text_inf.string_offset ),
                                           fonts_.at( info.text_inf.res ),
                                           info.text_inf.char_size );
                    }
                    else
                    {
                         auto creator = reinterpret_cast< TextNode * ( * )( Quadrant * )>( func );
                         t_node = creator( &quad );
                    }
               }
               else if ( check_resource( info.text_inf.res ) )
               {
                    t_node = new TextNode( &quad, read_string( info.text_inf.string_offset ),
                                           fonts_.at( info.text_inf.res ),
                                           info.text_inf.char_size );
               }
               else
               {
                    t_node = new TextNode( &quad );
               }
               t_node->set_layer( info.text_inf.layer );
               t_node->set_visible( info.text_inf.visible );
               t_node->set_color( Color( info.text_inf.color ) );
               t_node->set_style( info.text_inf.style );
               t_node->set_outline_color( Color{ info.text_inf.outline_color } );
               t_node->set_outline_thickness( info.text_inf.outline_thickness );
               t_node->set_line_spacing( info.text_inf.line_spacing );
               t_node->set_letter_spacing( info.text_inf.letter_spacing );
               node = t_node;
          }
          break;
          case NodeType::Node2D:
          {
               if ( 0 != info.creator_name_off )
               {
                    void* func = libs_.at( info.code_file_num ).get_symbol( read_string( info.creator_name_off ) );
                    auto creator = reinterpret_cast< Node2D * ( * )() >( func );
                    node = creator();
               }
               else
               {
                    node = new Node2D;
               }
          }
          break;
     }
     if ( 0 != info.name_off )
     {
          Game::get_world().set_node_name( node, read_string( info.name_off ) );
     }
     if ( 0 != info.parent )
     {
          node->set_parent( node_offsets_.at( info.parent ) );
     }
     else
     {
          state.add_node( node );
     }
     node_offsets_[ offset ] = node;
     node->set_origin({info.origin[0], info.origin[1]});
     node->set_position( { info.pos[ 0 ], info.pos[ 1 ] } );
     node->set_rotation( info.rotation );
     node->set_scale( { info.scale[ 0 ], info.scale[ 1 ] } );
}


void FileSceneReader::load_libs( std::shared_ptr< uint32_t[] > lib_offsets, uint32_t count )
{
     libs_.reserve( count );
     for ( uint32_t i = 0; i < count; i++ )
     {
          libs_.push_back( std::move( DynamicLib( read_string( lib_offsets[ i ] ) ) ) );
     }
}


std::string FileSceneReader::read_string( uint32_t offset )
{
     auto current_pos = file_stream_.tellg();
     file_stream_.seekg( offset );
     std::string result;
     std::getline( file_stream_, result, '\0' );
     file_stream_.seekg( current_pos );
     return result;
}

} // namespace _16nar
