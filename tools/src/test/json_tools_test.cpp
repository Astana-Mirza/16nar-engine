#include <catch2/catch_test_macros.hpp>
#include <16nar/tools/json/json_asset_reader.h>
#include <16nar/tools/json/json_asset_file_processor.h>

TEST_CASE( "Asset reading", "[json_tools]" )
{
     _16nar::tools::JsonAssetFileProcessor file_processor{ *std::pmr::get_default_resource() };
     _16nar::tools::SharedBufferPtr buffer{};
     {
          _16nar::tools::File file{};
          REQUIRE( file.open( "data/test_asset.json" ) );
          buffer = file_processor.read_asset_data( file );
          REQUIRE( buffer );
     }

     _16nar::tools::AssetData content{};
     {
          _16nar::tools::JsonAssetReader empty_asset_reader{};
          REQUIRE( empty_asset_reader.is_empty() );
          REQUIRE( empty_asset_reader.get_name().empty() );
          REQUIRE( empty_asset_reader.get_children_count() == 0 );
          REQUIRE( !empty_asset_reader.is_array() );
          REQUIRE( !empty_asset_reader.has_child( "test_child3" ) );
          REQUIRE( !empty_asset_reader.to_child( "test_child3" ) );
          REQUIRE( !empty_asset_reader.to_child_index( 0 ) );
          REQUIRE( !empty_asset_reader.to_parent() );

          content = empty_asset_reader.get_content();
          REQUIRE( !content.data );
          REQUIRE( content.type_id == 0 );
     }

     auto asset_reader = file_processor.make_asset_reader( buffer.get_const_view() );
     REQUIRE( asset_reader );

     REQUIRE( !asset_reader->is_empty() );
     REQUIRE( asset_reader->get_name() == "test_container" );
     content = asset_reader->get_content();
     REQUIRE( !content.data );
     REQUIRE( content.type_id == 0 );
     REQUIRE( !asset_reader->is_array() );
     REQUIRE( asset_reader->get_children_count() == 2 );
     REQUIRE( asset_reader->has_child( "test_child1" ) );
     REQUIRE( asset_reader->has_child( "test_child2" ) );

     auto children_names = asset_reader->get_children_names();
     REQUIRE( children_names.size() == 2 );
     REQUIRE( std::find( children_names.cbegin(),
          children_names.cend(), "test_child1" ) != children_names.cend() );
     REQUIRE( std::find( children_names.cbegin(),
          children_names.cend(), "test_child2" ) != children_names.cend() );

     REQUIRE( !asset_reader->has_child( "test_child3" ) );
     REQUIRE( !asset_reader->to_parent() );
     REQUIRE( !asset_reader->to_child( "test_child3" ) );

     REQUIRE( asset_reader->to_child( "test_child1" ) );
     REQUIRE( !asset_reader->is_empty() );
     REQUIRE( asset_reader->get_name() == "test_child1" );
     REQUIRE( !asset_reader->is_array() );
     content = asset_reader->get_content();
     REQUIRE( content.data );
     REQUIRE( content.type_id == 123 );
     REQUIRE( std::string_view{ reinterpret_cast< const char * >(
          content.data.data ), content.data.size } == "testdata1" );
     REQUIRE( asset_reader->get_children_count() == 0 );
     REQUIRE( !asset_reader->to_child( "test_child1" ) );
     REQUIRE( !asset_reader->to_child_index( 1 ) );

     REQUIRE( asset_reader->to_parent() );
     REQUIRE( asset_reader->to_child( "test_child2" ) );
     REQUIRE( !asset_reader->is_empty() );
     REQUIRE( asset_reader->get_name() == "test_child2" );
     REQUIRE( asset_reader->is_array() );

     children_names = asset_reader->get_children_names();
     REQUIRE( children_names.empty() ); // because asset is an array

     REQUIRE( asset_reader->get_children_count() == 2 );
     content = asset_reader->get_content();
     REQUIRE( content.data );
     REQUIRE( content.type_id == 456 );
     REQUIRE( std::string_view{ reinterpret_cast< const char * >(
          content.data.data ), content.data.size } == "testdata2" );

     REQUIRE( asset_reader->to_child_index( 0 ) );
     REQUIRE( !asset_reader->is_empty() );
     REQUIRE( asset_reader->get_name() == "test_child3" );
     REQUIRE( !asset_reader->is_array() );
     REQUIRE( asset_reader->get_children_count() == 0 );

     REQUIRE( asset_reader->to_parent() );
     REQUIRE( !asset_reader->is_empty() );
     REQUIRE( asset_reader->to_child_index( 1 ) );
     REQUIRE( asset_reader->get_name() == "test_child4" );
     content = asset_reader->get_content();
     REQUIRE( content.data );
     REQUIRE( content.type_id == 789 );
     REQUIRE( std::string_view{ reinterpret_cast< const char * >(
          content.data.data ), content.data.size } == "testdata3" );

     REQUIRE( asset_reader->to_parent() );
     REQUIRE( asset_reader->to_parent() );
     REQUIRE( !asset_reader->to_parent() );
}
