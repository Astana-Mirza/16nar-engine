#include <catch2/catch_test_macros.hpp>

#include <16nar/tools/json/json_to_flatbuffers_convertor.h>

#include <nlohmann/json.hpp>

#include <fstream>
#include <iterator>

TEST_CASE( "Asset data convertor", "[json_tools]" )
{
     _16nar::tools::JsonToFlatBuffersConvertor convertor{};

     const std::string schema_path{ "data/schemas/test_schema.fbs" };
     std::ifstream json_input{ "data/json_by_schema.json" };
     std::ifstream schema_input{ schema_path };
     const std::string json_data{
          std::istreambuf_iterator< char >( json_input ), std::istreambuf_iterator< char >() };
     const std::string schema_data{
          std::istreambuf_iterator< char >( schema_input ), std::istreambuf_iterator< char >() };

     const std::uint32_t type_id{ 1u };
     convertor.set_type_name( type_id, "test_game.Monster" );

     REQUIRE( convertor.add_schema( schema_data, {}, schema_path ) );

     _16nar::tools::AssetData json_content{};
     json_content.type_id = type_id;
     json_content.data = _16nar::tools::ConstByteView{
          reinterpret_cast< const std::byte * >( json_data.c_str() ), json_data.size() };

     const auto fb_content = convertor.convert_forward( json_content );
     REQUIRE( fb_content.type_id == type_id );
     REQUIRE( fb_content.data );

     json_content = convertor.convert_backward( fb_content );
     REQUIRE( json_content.type_id == type_id );
     REQUIRE( json_content.data );

     const std::string_view json_raw{
          reinterpret_cast< const char * >( json_content.data.data ), json_content.data.size };

     auto etalon = nlohmann::json::parse( json_data );
     auto result = nlohmann::json::parse( json_raw );
     REQUIRE( etalon == result );
}
