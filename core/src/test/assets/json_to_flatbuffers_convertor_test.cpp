#include <catch2/catch_test_macros.hpp>

#include <16nar/platform/strings/name_table.h>
#include <16nar/core/assets/json/json_to_flatbuffers_convertor.h>

#include <nlohmann/json.hpp>

#include <fstream>
#include <iterator>
#include <memory_resource>

TEST_CASE( "Asset data convertor", "[json_assets]" )
{
     _16nar::strings::NameTable type_names( *std::pmr::get_default_resource() );
     _16nar::assets::JsonToFlatBuffersConvertor convertor{ type_names };

     auto type_id = type_names.add_name( "test_game.Monster" );
     REQUIRE( !type_id.empty() );

     const std::string schema_path{ "data/schemas/test_schema.fbs" };
     std::ifstream json_input{ "data/json_by_schema.json" };
     std::ifstream schema_input{ schema_path };
     const std::string json_data{
          std::istreambuf_iterator< char >( json_input ), std::istreambuf_iterator< char >() };
     const std::string schema_data{
          std::istreambuf_iterator< char >( schema_input ), std::istreambuf_iterator< char >() };

     CHECK( convertor.add_schema( schema_data, {}, schema_path ) );

     _16nar::assets::AssetData json_content{};
     json_content.type_id = type_id;
     json_content.data = _16nar::memory::ConstByteView{
          reinterpret_cast< const std::byte * >( json_data.c_str() ), json_data.size() };

     const auto fb_content = convertor.convert_forward( json_content );
     CHECK( fb_content.type_id == type_id );
     CHECK( fb_content.data );

     json_content = convertor.convert_backward( fb_content );
     CHECK( json_content.type_id == type_id );
     CHECK( json_content.data );

     const std::string_view json_raw{
          reinterpret_cast< const char * >( json_content.data.data ), json_content.data.size };

     auto etalon = nlohmann::json::parse( json_data );
     auto result = nlohmann::json::parse( json_raw );
     CHECK( etalon == result );
}
