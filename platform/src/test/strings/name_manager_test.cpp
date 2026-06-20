#include <catch2/catch_test_macros.hpp>

#include <16nar/platform/strings/name_manager.h>

#include <memory_resource>

TEST_CASE( "Name table addition and removal", "[strings_name_manager]" )
{
     _16nar::strings::NameManager name_manager{ *std::pmr::get_default_resource() };

     CHECK( name_manager.get_table( "not_exists" ) == nullptr );
     CHECK( name_manager.get_table( "" ) == nullptr );

     auto *name_table1 = name_manager.add_table( "test_table1" );
     CHECK( name_table1 != nullptr );
     CHECK( name_manager.add_table( "test_table1" ) == nullptr );
     CHECK( name_manager.get_table( "test_table1" ) == name_table1 );

     // hash collision
     auto *name_table2 = name_manager.add_table( "8yn0iYCKYHlIj4-BwPqk" );
     CHECK( name_table2 != nullptr );
     CHECK( name_manager.add_table( "GReLUrM4wMqfg9yzV3KQ" ) == nullptr );
     CHECK( name_manager.get_table( "GReLUrM4wMqfg9yzV3KQ" ) == nullptr );

     name_manager.remove_table( "8yn0iYCKYHlIj4-BwPqk" );
     auto *name_table3 = name_manager.add_table( "GReLUrM4wMqfg9yzV3KQ" );
     CHECK( name_table3 != nullptr );

     name_manager.remove_table( "test_table1" );
     CHECK( name_manager.get_table( "test_table1" ) == nullptr );
}
