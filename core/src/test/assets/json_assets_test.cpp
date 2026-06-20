#include <catch2/catch_test_macros.hpp>
#include <16nar/core/assets/json/json_asset_reader.h>
#include <16nar/core/assets/json/json_asset_file_processor.h>

#include <memory_resource>

TEST_CASE( "Asset reading", "[json_assets]" )
{
     _16nar::assets::JsonAssetFileProcessor file_processor{
          *std::pmr::get_default_resource(), *std::pmr::get_default_resource()
     };
     _16nar::memory::SharedBufferPtr buffer{};
     {
          _16nar::system::File file{};
          REQUIRE( file.open( "data/test_asset.json" ) );
          buffer = file_processor.read_asset_data( file );
          CHECK( buffer );
     }

     _16nar::assets::AssetData content{};
     {
          _16nar::assets::JsonAssetReader empty_asset_reader{
               *std::pmr::get_default_resource() };
          CHECK( empty_asset_reader.is_empty() );
          CHECK( empty_asset_reader.get_name().empty() );
          CHECK( empty_asset_reader.get_children_count() == 0 );
          CHECK( !empty_asset_reader.is_array() );
          CHECK( !empty_asset_reader.has_child( "test_child3" ) );
          CHECK( !empty_asset_reader.to_child( "test_child3" ) );
          CHECK( !empty_asset_reader.to_child_index( 0 ) );
          CHECK( !empty_asset_reader.to_parent() );

          content = empty_asset_reader.get_content();
          CHECK( !content.data );
          CHECK( content.type_id.empty() );
     }

     auto asset_reader = file_processor.make_asset_reader();
     REQUIRE( asset_reader );
     asset_reader->reset( buffer.get_const_view() );

     CHECK( !asset_reader->is_empty() );
     CHECK( asset_reader->get_name() == "test_container" );
     content = asset_reader->get_content();
     CHECK( !content.data );
     CHECK( content.type_id.empty() );
     CHECK( !asset_reader->is_array() );
     CHECK( asset_reader->get_children_count() == 2 );
     CHECK( asset_reader->has_child( "test_child1" ) );
     CHECK( asset_reader->has_child( "test_child2" ) );

     auto children_names = asset_reader->get_children_names();
     CHECK( children_names.size() == 2 );
     CHECK( std::find( children_names.cbegin(),
          children_names.cend(), "test_child1" ) != children_names.cend() );
     CHECK( std::find( children_names.cbegin(),
          children_names.cend(), "test_child2" ) != children_names.cend() );

     CHECK( !asset_reader->has_child( "test_child3" ) );
     CHECK( !asset_reader->to_parent() );
     CHECK( !asset_reader->to_child( "test_child3" ) );

     CHECK( asset_reader->to_child( "test_child1" ) );
     CHECK( !asset_reader->is_empty() );
     CHECK( asset_reader->get_name() == "test_child1" );
     CHECK( !asset_reader->is_array() );
     content = asset_reader->get_content();
     CHECK( content.data );
     CHECK( content.type_id.hash == 123 );
     CHECK( std::string_view{ reinterpret_cast< const char * >(
          content.data.data ), content.data.size } == "testdata1" );
     CHECK( asset_reader->get_children_count() == 0 );
     CHECK( !asset_reader->to_child( "test_child1" ) );
     CHECK( !asset_reader->to_child_index( 1 ) );

     CHECK( asset_reader->to_parent() );
     CHECK( asset_reader->to_child( "test_child2" ) );
     CHECK( !asset_reader->is_empty() );
     CHECK( asset_reader->get_name() == "test_child2" );
     CHECK( asset_reader->is_array() );

     children_names = asset_reader->get_children_names();
     CHECK( children_names.empty() ); // because asset is an array

     CHECK( asset_reader->get_children_count() == 2 );
     content = asset_reader->get_content();
     CHECK( content.data );
     CHECK( content.type_id.hash == 456 );
     CHECK( std::string_view{ reinterpret_cast< const char * >(
          content.data.data ), content.data.size } == "testdata2" );

     CHECK( asset_reader->to_child_index( 0 ) );
     CHECK( !asset_reader->is_empty() );
     CHECK( asset_reader->get_name() == "test_child3" );
     CHECK( !asset_reader->is_array() );
     CHECK( asset_reader->get_children_count() == 0 );

     CHECK( asset_reader->to_parent() );
     CHECK( !asset_reader->is_empty() );
     CHECK( asset_reader->to_child_index( 1 ) );
     CHECK( asset_reader->get_name() == "test_child4" );
     content = asset_reader->get_content();
     CHECK( content.data );
     CHECK( content.type_id.hash == 789 );
     CHECK( std::string_view{ reinterpret_cast< const char * >(
          content.data.data ), content.data.size } == "testdata3" );

     CHECK( asset_reader->to_parent() );
     CHECK( asset_reader->to_parent() );
     CHECK( !asset_reader->to_parent() );
}
