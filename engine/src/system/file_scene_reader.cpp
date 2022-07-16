#include <system/file_scene_reader.h>
#include <render/quadrant.h>

namespace _16nar
{

void FileSceneReader::set_scene( const std::string& name )
{
     file_stream_.open( name, std::ios::in | std::ios::binary );
}


void FileSceneReader::load_scene( WorldNode& world, SetupFuncPtr& setup_ptr, LoopFuncPtr& loop_ptr )
{
     SceneHeader hdr;
     file_stream_.read( reinterpret_cast< char * >( &hdr ), sizeof( SceneHeader ) );

     auto states = std::make_unique< StateInfo[] >( hdr.state_count );
     file_stream_.seekg( hdr.sections.state_off );
     file_stream_.read( reinterpret_cast< char * >( states.get() ), sizeof( StateInfo ) * hdr.state_count );

     load_packages( hdr );
     load_code( hdr, setup_ptr, loop_ptr );

     file_stream_.seekg( hdr.sections.tree_off );
     for ( uint32_t i = 0; i < hdr.state_count; i++ )
     {
          auto state_ptr = std::make_unique< SceneState >( states[ i ].updating, states[ i ].rendering );

          FloatRect area( { states[ i ].q_start[ 0 ], states[ i ].q_start[ 1 ],
                    states[ i ].scene_width  * states[ i ].q_size[ 0 ],
                    states[ i ].scene_height * states[ i ].q_size[ 1 ] } );
          Quadrant& root = state_ptr->get_root_quadrant();
          root = Quadrant( area );

          make_quadrants( root, Vector2f{ area.left, area.top },
                    states[ i ].q_size[ 0 ], states[ i ].q_size[ 1 ],
                    states[ i ].scene_width, states[ i ].scene_height);

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
     ResourceFileHeader hdr;
     res_file.open( filename, std::ios::in | std::ios::binary );
     res_file.read( reinterpret_cast< char * >( &hdr ), sizeof( ResourceFileHeader ) );

     auto signs = std::make_unique< ResourceSign[] >( hdr.res_count );
     res_file.read( reinterpret_cast< char * >( signs.get() ), sizeof( ResourceSign ) * hdr.res_count );

     for ( uint16_t i = 0; i < res_count; i++ )
     {
          auto raw = std::make_unique< char >( signs[ res_ids[ i ] ].size );
          res_file.seekg( signs[ res_ids[ i ] ].offset );
          res_file.read( reinterpret_cast< char * >( raw.get() ), signs[ res_ids[ i ] ].size );
          switch ( signs[ res_ids[ i ] ].type )
          {
               case ResourceType::Texture:
               {
                    textures_[ { file_id, res_ids[ i ] } ] = Texture{};
                    if ( !textures_[ { file_id, res_ids[ i ] } ]
                         .loadFromMemory( raw.get(), signs[ res_ids[ i ] ].size ) )
                    {
                         throw std::runtime_error{ "cannot load resource from file " + filename };
                    }
			}
			break;
			default:
                    throw std::runtime_error{ "unknown resource type in file " + filename };
                    break;
          }
     }
}


void FileSceneReader::load_code( const SceneHeader& hdr, SetupFuncPtr& setup_ptr, LoopFuncPtr& loop_ptr )
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
          setup_ptr = reinterpret_cast< SetupFuncPtr >( libs_.at( hdr.main_code_file ).get_symbol( setup_name ) );
	}

     if ( hdr.loop_off )
     {
          auto loop_name = read_string( hdr.loop_off );
          loop_ptr = reinterpret_cast< LoopFuncPtr >( libs_.at( hdr.main_code_file ).get_symbol( loop_name ) );
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
          for ( uint32_t i = 0; i < current_hcount; i += 2 )
          {
               for ( uint32_t j = 0; j < current_wcount; j += 2 )
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
     // read name
     // node_offsets_[ offset ] = pointer to node
     // if parent is nullptr, add node to state
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
     file_stream_ >> result;
     file_stream_.seekg( current_pos );
     return result;
}

} // namespace _16nar