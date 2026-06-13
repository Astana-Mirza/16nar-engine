#include <catch2/catch_test_macros.hpp>

#include <16nar/platform/system/file.h>

#include <cstring>

TEST_CASE( "File read", "[system_file]" )
{
     char raw[ 8 ];
     _16nar::memory::ByteView buf{ reinterpret_cast< std::byte * >( &raw[ 0 ] ), sizeof( raw ) };

     _16nar::system::File file;
     REQUIRE( file.open( "data/file.txt" ) );
     REQUIRE( file.is_open() );
     CHECK( file.tell() == 0 );
     CHECK( file.get_size() == 80 );

     REQUIRE( file.read( buf ) == buf.size );
     CHECK( file.tell() == 8 );
     CHECK( std::memcmp( "abcdefgh", buf.data, buf.size ) == 0 );
     CHECK( file.seek( 2, _16nar::system::File::SeekOrigin::Set ) );
     CHECK( file.tell() == 2 );
     REQUIRE( file.read( buf ) == buf.size );
     CHECK( std::memcmp( "cdefghab", buf.data, buf.size ) == 0 );
     CHECK( file.seek( 5, _16nar::system::File::SeekOrigin::Cur ) );
     CHECK( file.tell() == 15 );
     REQUIRE( file.read( buf ) == buf.size );
     CHECK( std::memcmp( "habcdefg", buf.data, buf.size ) == 0 );
     CHECK( file.seek( -3, _16nar::system::File::SeekOrigin::End ) );
     CHECK( file.tell() == 77 );
     REQUIRE( file.read( buf ) == 3 );
     CHECK( std::memcmp( "fgh", buf.data, 3 ) == 0 );

     // ensure that get_size() works correctly regardless of seek position
     CHECK( file.get_size() == 80 );

     file.close();
     CHECK( !file.is_open() );
}


TEST_CASE( "File write", "[system_file]" )
{
     const char raw[] = "abcdefgh";
     _16nar::memory::ConstByteView buf{
          reinterpret_cast< const std::byte * >( &raw[ 0 ] ), sizeof( raw ) - 1 };

     _16nar::system::File file;
     REQUIRE( file.open( "data/file_out.txt", true ) );
     REQUIRE( file.is_open() );

     CHECK( file.write( buf ) == buf.size );
     CHECK( file.tell() == 8 );
     CHECK( file.get_size() == 8 );

     CHECK( file.seek( 2, _16nar::system::File::SeekOrigin::Set ) );
     CHECK( file.write( buf ) == buf.size );
     CHECK( file.tell() == 10 );
     CHECK( file.get_size() == 10 );

     file.close();
     CHECK( !file.is_open() );

     char raw_read[ 10 ];
     _16nar::memory::ByteView read_buf{
          reinterpret_cast< std::byte * >( &raw_read[ 0 ] ), sizeof( raw_read ) };

     _16nar::system::File read_file;
     REQUIRE( read_file.open( "data/file_out.txt" ) );
     REQUIRE( read_file.read( read_buf ) == read_buf.size );
     CHECK( std::memcmp( "ababcdefgh", read_buf.data, read_buf.size ) == 0 );
}
