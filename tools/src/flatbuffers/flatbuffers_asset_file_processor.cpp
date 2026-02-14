#include <16nar/tools/flatbuffers/flatbuffers_asset_file_processor.h>

#include <16nar/tools/flatbuffers/flatbuffers_asset_reader.h>

#include <16nar/gen/flatbuffers/asset_generated.h>

namespace _16nar::tools
{

FlatBuffersAssetFileProcessor::FlatBuffersAssetFileProcessor( std::pmr::memory_resource& memory_resource ) noexcept:
     memory_resource_{ memory_resource }
{}


SharedBufferPtr FlatBuffersAssetFileProcessor::read_asset_data( const File& file )
{
     _16nar::data::FileHeader header{};
     const auto size = file.read( ByteView{ reinterpret_cast< std::byte* >( &header ), sizeof( header ) } );
     if ( size != sizeof( header ) )
     {
          return SharedBufferPtr{};
     }
     if ( !header.size() || !NARENGINE_VERSION_CHECK( header.version(), NARENGINE_ASSET_VERSION_UINT32 ) )
     {
          return SharedBufferPtr{};
     }
     auto ret = SharedBufferPtr::allocate( memory_resource_, header.size() );
     if ( size != file.read( ret.get_view() ) )
     {
          return SharedBufferPtr{};
     }
     return ret;
}


bool FlatBuffersAssetFileProcessor::write_asset_data( ConstByteView buffer, File& file )
{
     if ( !buffer )
     {
          return false;
     }

     _16nar::data::FileHeader header{ NARENGINE_ASSET_VERSION_UINT32,
          static_cast< std::uint32_t >( buffer.size ) };
     const auto size = file.write( ConstByteView{
          reinterpret_cast< const std::byte* >( &header ), sizeof( header ) } );
     if ( size != sizeof( header ) )
     {
          return false;
     }
     if ( buffer.size != file.write( buffer ) )
     {
          return false;
     }
     return true;
}


IAssetReaderPtr FlatBuffersAssetFileProcessor::make_asset_reader( ConstByteView buffer )
{
     return std::make_shared< FlatBuffersAssetReader >( buffer );
}

} // namespace _16nar::tools
