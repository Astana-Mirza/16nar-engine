#include <catch2/catch_test_macros.hpp>

#include <16nar/core/ecs/ecs_storage.h>
#include <16nar/core/ecs/query.h>

#include <string>

namespace
{

struct TestComp1
{
     static int constructed;
     static int destructed;

     static void construct( std::byte *data )
     {
          auto *ptr = new ( data ) TestComp1{};
          ptr->str = "component_" + std::to_string( ++constructed );
     }

     static void destruct( std::byte *data )
     {
          auto *ptr = reinterpret_cast< TestComp1 * >( data );
          ptr->~TestComp1();
          ++destructed;
     }

     // It's just for test! In real code, try to avoid allocations like std::string below
     std::string str;
};

int TestComp1::constructed = 0;
int TestComp1::destructed = 0;

using TestComp2 = std::uint32_t;

} // anonymous namespace


TEST_CASE( "Entities and components creation", "[ecs_storage]" )
{
     TestComp1::constructed = 0;
     TestComp1::destructed = 0;
     _16nar::strings::NameTable name_table{ *std::pmr::get_default_resource() };
     _16nar::ecs::EcsStorage ecs{ name_table,
          *std::pmr::get_default_resource(), *std::pmr::get_default_resource() };

     _16nar::strings::StaticName test_comp1{ "TestComp1" };
     _16nar::strings::StaticName test_comp2{ "TestComp2" };
     _16nar::strings::StaticName test_comp3{ "TestComp3" };
     _16nar::strings::StaticName quasitype1{ "Quasitype1" };
     _16nar::strings::StaticName quasitype2{ "Quasitype2" };

     CHECK( ecs.register_component_type( test_comp1, sizeof( TestComp1 ),
          &TestComp1::construct, &TestComp1::destruct ) );
     CHECK( ecs.register_component_type( test_comp2, sizeof( TestComp2 ) ) );
     CHECK( ecs.register_component_type( test_comp3 ) );

     CHECK( TestComp1::constructed == 0 );
     CHECK( TestComp1::destructed == 0 );

     auto entity1 = ecs.add_entity( quasitype1 );
     CHECK( entity1 );
     CHECK( ecs.has_entity( entity1 ) );
     ecs.delete_entity( entity1 );
     CHECK( !ecs.has_entity( entity1 ) );
     ecs.commit();
     CHECK( !ecs.has_entity( entity1 ) );

     entity1 = ecs.add_entity( quasitype1 );
     CHECK( entity1 );
     CHECK( ecs.has_entity( entity1 ) );

     auto *comp1 = ecs.add_component< TestComp1 >( entity1, test_comp1 );
     CHECK( ecs.has_component( entity1, test_comp1 ) );
     CHECK( comp1 == ecs.get_component< TestComp1 >( entity1, test_comp1 ) );
     REQUIRE( comp1 );
     CHECK( comp1->str == "component_1" );
     CHECK( TestComp1::constructed == 1 );
     CHECK( TestComp1::destructed == 0 );

     auto *comp2 = ecs.add_component< TestComp2 >( entity1, test_comp2 );
     CHECK( comp2 );
     CHECK( ecs.has_component( entity1, test_comp2 ) );
     CHECK( comp2 == ecs.get_component< TestComp2 >( entity1, test_comp2 ) );

     CHECK( !ecs.add_component< bool >( entity1, test_comp3 ) );
     CHECK( !ecs.has_component( entity1, test_comp3 ) );
     CHECK( ecs.add_flag_component( entity1, test_comp3 ) );
     CHECK( ecs.has_component( entity1, test_comp3 ) );

     ecs.delete_component( entity1, test_comp1 );
     CHECK( !ecs.has_component( entity1, test_comp1 ) );
     CHECK( ecs.has_entity( entity1 ) );
     CHECK( TestComp1::destructed == 0 );    // not commited yet
     ecs.commit();
     CHECK( !ecs.has_component( entity1, test_comp1 ) );
     CHECK( ecs.has_entity( entity1 ) );
     CHECK( TestComp1::destructed == 1 );

     ecs.preassign_pages( quasitype1, 63 );
     ecs.preassign_pages( quasitype2, 2 );

     auto entity2 = ecs.add_entity( quasitype2 );
     CHECK( !ecs.get_component< TestComp1 >( entity2, test_comp1 ) );
     CHECK( ecs.add_component< TestComp1 >( entity2, test_comp1 ) );
     CHECK( ecs.add_component< TestComp2 >( entity2, test_comp2 ) );
     CHECK( ecs.add_flag_component( entity2, test_comp3 ) );

     CHECK( TestComp1::constructed == 2 );
     CHECK( TestComp1::destructed == 1 );

     ecs.clear();

     CHECK( TestComp1::constructed == 2 );
     CHECK( TestComp1::destructed == 2 );
}


TEST_CASE( "Queries into ECS", "[ecs_query]" )
{
     TestComp1::constructed = 0;
     TestComp1::destructed = 0;
     _16nar::strings::NameTable name_table{ *std::pmr::get_default_resource() };
     _16nar::ecs::EcsStorage ecs{ name_table,
          *std::pmr::get_default_resource(), *std::pmr::get_default_resource() };

     _16nar::strings::StaticName test_comp1{ "TestComp1" };
     _16nar::strings::StaticName test_comp2{ "TestComp2" };
     _16nar::strings::StaticName test_comp3{ "TestComp3" };
     _16nar::strings::StaticName quasitype1{ "Quasitype1" };
     _16nar::strings::StaticName quasitype2{ "Quasitype2" };

     ecs.register_component_type( test_comp1, sizeof( TestComp1 ),
          &TestComp1::construct, &TestComp1::destruct );
     ecs.register_component_type( test_comp2, sizeof( TestComp2 ) );
     ecs.register_component_type( test_comp3 );

     ecs.preassign_pages( quasitype1, 64 );
     ecs.preassign_pages( quasitype2, 1 );

     auto entity1 = ecs.add_entity( quasitype1 );
     auto entity2 = ecs.add_entity( quasitype2 );
     auto entity3 = ecs.add_entity( quasitype2 );
     auto entity4 = ecs.add_entity( quasitype1 );

     auto *comp2 = ecs.add_component< TestComp2 >( entity1, test_comp2 );
     REQUIRE( comp2 );
     *comp2 = 1;
     comp2 = ecs.add_component< TestComp2 >( entity2, test_comp2 );
     REQUIRE( comp2 );
     *comp2 = 2;
     comp2 = ecs.add_component< TestComp2 >( entity3, test_comp2 );
     REQUIRE( comp2 );
     *comp2 = 3;
     comp2 = ecs.add_component< TestComp2 >( entity4, test_comp2 );
     REQUIRE( comp2 );
     *comp2 = 4;

     ecs.add_component< TestComp1 >( entity2, test_comp1 );
     ecs.add_flag_component( entity1, test_comp3 );
     ecs.add_flag_component( entity4, test_comp3 );

     _16nar::ecs::Query query1{ ecs, *std::pmr::get_default_resource() };
     std::size_t comp2_index{}, comp1_index{};

     query1
          .get( test_comp2, comp2_index )
          .get_optional( test_comp1, comp1_index );

     REQUIRE( query1.valid() );
     CHECK( query1.finished() );
     CHECK( query1.get_result().begin() == query1.get_result().end() );

     std::size_t total = 0;
     std::uint32_t sum = 0;
     bool comp1_found = false;
     for ( auto& result : query1.execute() )
     {
          ++total;

          CHECK( query1.valid() );
          CHECK( !query1.finished() );

          comp2 = result.get_component< TestComp2 >( comp2_index );
          REQUIRE( comp2 );
          CHECK( *comp2 > 0 );
          sum += *comp2;

          if ( result.has_component( comp1_index ) )
          {
               comp1_found = true;
               CHECK( result.get_entity_id() == entity2 );
               auto *comp1 = result.get_component< TestComp1 >( comp1_index );
               REQUIRE( comp1 );
               CHECK( comp1->str == "component_1" );
          }
          else
          {
               CHECK( result.get_component< TestComp1 >( comp1_index ) == nullptr );
          }
     }

     CHECK( query1.finished() );
     CHECK( query1.valid() );
     CHECK( total == 4 );
     CHECK( sum == 10 );
     CHECK( comp1_found );

     // unavailable if query is finished
     CHECK( query1.get_result().get_component< TestComp2 >( comp2_index ) == nullptr );
     CHECK( !query1.get_result().has_component( comp2_index ) );

     ecs.delete_component( entity4, test_comp2 );

     _16nar::ecs::Query query2{ ecs, *std::pmr::get_default_resource() };
     query2
          .has( test_comp3 )
          .has_optional( test_comp2, comp2_index );

     REQUIRE( query2.valid() );
     CHECK( query2.finished() );

     total = 0;
     for ( auto& result : query2.execute() )
     {
          ++total;
          if ( result.get_entity_id() != entity4 )
          {
               CHECK( result.has_component( comp2_index ) );
          }
          else
          {
               CHECK( !result.has_component( comp2_index ) );
          }

          // data was not requested
          CHECK( result.get_component< TestComp2 >( comp2_index ) == nullptr );
     }

     CHECK( query2.finished() );
     CHECK( query2.valid() );
     CHECK( total == 2 );
}
