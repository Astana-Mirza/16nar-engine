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
     REQUIRE( file.tell() == 0 );
     REQUIRE( file.get_size() == 80 );

     REQUIRE( file.read( buf ) == buf.size );
     REQUIRE( file.tell() == 8 );
     REQUIRE( std::memcmp( "abcdefgh", buf.data, buf.size ) == 0 );
     REQUIRE( file.seek( 2, _16nar::system::File::SeekOrigin::Set ) );
     REQUIRE( file.tell() == 2 );
     REQUIRE( file.read( buf ) == buf.size );
     REQUIRE( std::memcmp( "cdefghab", buf.data, buf.size ) == 0 );
     REQUIRE( file.seek( 5, _16nar::system::File::SeekOrigin::Cur ) );
     REQUIRE( file.tell() == 15 );
     REQUIRE( file.read( buf ) == buf.size );
     REQUIRE( std::memcmp( "habcdefg", buf.data, buf.size ) == 0 );
     REQUIRE( file.seek( -3, _16nar::system::File::SeekOrigin::End ) );
     REQUIRE( file.tell() == 77 );
     REQUIRE( file.read( buf ) == 3 );
     REQUIRE( std::memcmp( "fgh", buf.data, 3 ) == 0 );

     // ensure that get_size() works correctly regardless of seek position
     REQUIRE( file.get_size() == 80 );

     file.close();
     REQUIRE( !file.is_open() );
}


TEST_CASE( "File write", "[system_file]" )
{
     const char raw[] = "abcdefgh";
     _16nar::memory::ConstByteView buf{
          reinterpret_cast< const std::byte * >( &raw[ 0 ] ), sizeof( raw ) - 1 };

     _16nar::system::File file;
     REQUIRE( file.open( "data/file_out.txt", true ) );
     REQUIRE( file.is_open() );

     REQUIRE( file.write( buf ) == buf.size );
     REQUIRE( file.tell() == 8 );
     REQUIRE( file.get_size() == 8 );

     REQUIRE( file.seek( 2, _16nar::system::File::SeekOrigin::Set ) );
     REQUIRE( file.write( buf ) == buf.size );
     REQUIRE( file.tell() == 10 );
     REQUIRE( file.get_size() == 10 );

     file.close();
     REQUIRE( !file.is_open() );

     char raw_read[ 10 ];
     _16nar::memory::ByteView read_buf{
          reinterpret_cast< std::byte * >( &raw_read[ 0 ] ), sizeof( raw_read ) };

     _16nar::system::File read_file;
     REQUIRE( read_file.open( "data/file_out.txt" ) );
     REQUIRE( read_file.read( read_buf ) == read_buf.size );
     REQUIRE( std::memcmp( "ababcdefgh", read_buf.data, read_buf.size ) == 0 );
}
