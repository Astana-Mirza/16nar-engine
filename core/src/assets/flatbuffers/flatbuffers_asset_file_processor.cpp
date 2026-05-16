#include <16nar/core/assets/flatbuffers/flatbuffers_asset_file_processor.h>

#include <16nar/platform/logger/logger.h>

#include <16nar/core/assets/flatbuffers/flatbuffers_asset_reader.h>
#include <16nar/core/assets/flatbuffers/flatbuffers_asset_writer.h>

#include <16nar/gen/flatbuffers/asset_generated.h>

namespace _16nar::assets
{

FlatBuffersAssetFileProcessor::FlatBuffersAssetFileProcessor(
     std::pmr::memory_resource& memory_resource, std::size_t initial_size, bool check_names ) noexcept:
     memory_resource_{ memory_resource },
     initial_size_{ initial_size },
     check_names_{ check_names }
{}


memory::SharedBufferPtr FlatBuffersAssetFileProcessor::read_asset_data( const system::File& file )
{
     _16nar::data::FileHeader header{};
     const auto size = file.read( memory::ByteView{
          reinterpret_cast< std::byte* >( &header ), sizeof( header ) } );
     if ( size != sizeof( header ) )
     {
          LOG_16NAR_ERROR( "Cannot read flatbuffers asset header" );
          return memory::SharedBufferPtr{};
     }
     if ( !header.size() || !NARENGINE_VERSION_CHECK( header.version(), NARENGINE_ASSET_VERSION_UINT32 ) )
     {
          LOG_16NAR_ERROR( "Header of flatbuffers asset is corrupted or has incompatible version" );
          return memory::SharedBufferPtr{};
     }
     auto ret = memory::SharedBufferPtr::allocate( memory_resource_, header.size() );
     if ( header.size() != file.read( ret.get_view() ) )
     {
          LOG_16NAR_ERROR( "Cannot read flatbuffers asset file of size %lu", header.size() );
          return memory::SharedBufferPtr{};
     }
     return ret;
}


bool FlatBuffersAssetFileProcessor::write_asset_data( memory::ConstByteView buffer, system::File& file )
{
     if ( !buffer )
     {
          LOG_16NAR_ERROR( "Cannot write flatbuffers asset: passed empty buffer" );
          return false;
     }

     _16nar::data::FileHeader header{ NARENGINE_ASSET_VERSION_UINT32,
          static_cast< std::uint32_t >( buffer.size ) };
     const auto size = file.write( memory::ConstByteView{
          reinterpret_cast< const std::byte* >( &header ), sizeof( header ) } );
     if ( size != sizeof( header ) )
     {
          LOG_16NAR_ERROR( "Cannot write flatbuffers asset header to file" );
          return false;
     }
     if ( buffer.size != file.write( buffer ) )
     {
          LOG_16NAR_ERROR( "Cannot write flatbuffers asset file of size %zu", buffer.size );
          return false;
     }
     return true;
}


IAssetReaderPtr FlatBuffersAssetFileProcessor::make_asset_reader()
{
     return std::make_shared< FlatBuffersAssetReader >();
}


IAssetWriterPtr FlatBuffersAssetFileProcessor::make_asset_writer()
{
     return std::make_shared< FlatBuffersAssetWriter >( memory_resource_, initial_size_, check_names_ );
}

} // namespace _16nar::assets
