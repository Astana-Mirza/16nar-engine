#include <catch2/catch_test_macros.hpp>

#include <16nar/core/ecs/ecs_storage.h>

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