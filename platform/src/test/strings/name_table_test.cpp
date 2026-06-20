#include <catch2/catch_test_macros.hpp>

#include <16nar/platform/strings/name_table.h>

#include <memory_resource>

TEST_CASE( "Name addition and removal", "[strings_name_table]" )
{
     _16nar::strings::NameTable name_table{ *std::pmr::get_default_resource() };

     CHECK( name_table.get_name( _16nar::strings::StaticName{ "not_exists" } ).empty() );
     CHECK( name_table.get_name( _16nar::strings::StaticName{ "not_exists" }, true ) == "[0xfad5ab6316cd0ccf]" );

     auto name1 = name_table.add_name( "test_string" );
     CHECK( !name1.empty() );
     CHECK( name_table.get_name( name1 ) == "test_string" );
     CHECK( name_table.add_name( std::string_view{} ).empty() );
     CHECK( name_table.add_name( "" ).empty() );
     CHECK( name_table.add_name( "test_string" ).empty() );

     // hash collision
     auto name2 = name_table.add_name( "8yn0iYCKYHlIj4-BwPqk" );
     CHECK( !name2.empty() );
     CHECK( name_table.add_name( "GReLUrM4wMqfg9yzV3KQ" ).empty() );

     name_table.remove_name( name2 );
     auto name3 = name_table.add_name( "GReLUrM4wMqfg9yzV3KQ" );
     CHECK( !name3.empty() );
     CHECK( name2 == name3 );

     name_table.remove_name( name1 );
     CHECK( name_table.get_name( name1 ).empty() );
}


TEST_CASE( "Getting static name by string", "[strings_name_table]" )
{
     _16nar::strings::NameTable name_table{ *std::pmr::get_default_resource() };

     auto name1 = name_table.add_name( "test_string" );
     CHECK( name1 == name_table.get_static_name( "test_string" ) );

     // hash collision
     auto name2 = name_table.add_name( "8yn0iYCKYHlIj4-BwPqk" );
     auto name3 = name_table.get_static_name( "GReLUrM4wMqfg9yzV3KQ" );
     CHECK( name2 != name3 );
     CHECK( name3.empty() );

     CHECK( name_table.get_static_name( "" ).empty() );
}
