#include <catch2/catch_test_macros.hpp>

#include <16nar/platform/strings/static_name.h>

TEST_CASE( "Static name default state", "[strings_static_name]" )
{
     _16nar::strings::StaticName name;
     CHECK( name.empty() );
     CHECK( name.hash == 0 );
     CHECK( name == _16nar::strings::StaticName{ "" } );
}


TEST_CASE( "Static name comparison", "[strings_static_name]" )
{
     _16nar::strings::StaticName name1, name2;
     name1.hash = 1;
     name2.hash = 2;
     CHECK( name1 == name1 );
     CHECK( name1 != name2 );
     CHECK( name1 <  name2 );
     CHECK( name1 <= name2 );
     CHECK( name1 <= name1 );
     CHECK( name2 >  name1 );
     CHECK( name2 >= name2 );
}


TEST_CASE( "Static name hash", "[strings_static_name]" )
{
     _16nar::strings::StaticName name{ "test_string" };
     CHECK( name.hash == _16nar::strings::str_hash64( "test_string" ) );
     CHECK( name.hash == 0xa74a9df432a5ae3f );
     CHECK( _16nar::strings::str_hash32( "test_string" ) == 0x8162e7ff );
}
