#include <16nar/core/assets/json/json_asset_file_processor.h>

#include <16nar/platform/logger/logger.h>

#include <16nar/core/assets/json/json_asset_reader.h>
#include <16nar/core/assets/json/json_asset_writer.h>
#include <16nar/core/assets/json/literals.h>

#include <cstdio> // sscanf

namespace _16nar::assets
{

JsonAssetFileProcessor::JsonAssetFileProcessor( std::pmr::memory_resource& resource,
     std::pmr::memory_resource& big_resource ) noexcept:
     resource_{ resource },
     big_resource_{ big_resource }
{}


memory::SharedBufferPtr JsonAssetFileProcessor::read_asset_data( const system::File& file )
{
     const auto file_size = file.get_size();
     if ( file_size == 0 )
     {
          LOG_16NAR_ERROR( "JSON asset file is empty" );
          return memory::SharedBufferPtr{};
     }
     auto ret = memory::SharedBufferPtr::allocate( resource_, file_size );
     if ( file_size != file.read( ret.get_view() ) )
     {
          LOG_16NAR_ERROR( "Cannot read JSON asset file of size %zu", file_size );
          return memory::SharedBufferPtr{};
     }

     try
     {
          const auto json = nlohmann::json::parse( std::string_view{
               reinterpret_cast< const char * >( ret.get_view().data ), ret.get_view().size } );
          const std::string& version = json.at( version_label );
          std::uint8_t major{};
          std::uint8_t minor{};
          std::uint8_t patch{};
          if ( std::sscanf( version.c_str(), "%hhu.%hhu.%hhu", &major, &minor, &patch ) != 3 )
          {
               LOG_16NAR_ERROR( "Cannot read version of JSON asset" );
               return memory::SharedBufferPtr{};
          }
          const std::uint32_t version_num = NARENGINE_VERSION_TO_UINT32( major, minor, patch );
          if ( !NARENGINE_VERSION_CHECK( version_num, NARENGINE_ASSET_VERSION_UINT32 ) )
          {
               LOG_16NAR_ERROR( "Incompatible JSON asset version %hhu.%hhu.%hhu", major, minor, patch );
               return memory::SharedBufferPtr{};
          }
     }
     catch ( const nlohmann::json::exception& ex )
     {
          LOG_16NAR_ERROR( "Cannot read JSON asset: %s", ex.what() );
          return memory::SharedBufferPtr{};
     }
     return ret;
}


bool JsonAssetFileProcessor::write_asset_data( memory::ConstByteView buffer, system::File& file )
{
     if ( !buffer )
     {
          LOG_16NAR_ERROR( "Cannot write JSON asset: passed empty buffer" );
          return false;
     }

     std::string dumped{};
     try
     {
          const std::string version = std::to_string( ( NARENGINE_ASSET_VERSION_UINT32 >> 16u ) & 0xff )
               + '.' + std::to_string( ( NARENGINE_ASSET_VERSION_UINT32 >> 8u ) & 0xff )
               + '.' + std::to_string( NARENGINE_ASSET_VERSION_UINT32 & 0xff );
          auto json = nlohmann::json::parse( std::string_view{
                    reinterpret_cast< const char * >( buffer.data ), buffer.size } );
          json[ version_label ] = version;
          dumped = json.dump();
     }
     catch ( const nlohmann::json::exception& ex )
     {
          LOG_16NAR_ERROR( "Cannot write JSON asset: %s", ex.what() );
          return false;
     }

     if ( dumped.size() != file.write( memory::ConstByteView{
          reinterpret_cast< const std::byte * >( dumped.c_str() ), dumped.size() } ) )
     {
          LOG_16NAR_ERROR( "Cannot write JSON asset file of size %zu", dumped.size() );
          return false;
     }
     return true;
}


IAssetReaderPtr JsonAssetFileProcessor::make_asset_reader()
{
     return std::allocate_shared< JsonAssetReader >(
          std::pmr::polymorphic_allocator< JsonAssetReader >( &resource_ ),
          resource_
     );
}


IAssetWriterPtr JsonAssetFileProcessor::make_asset_writer()
{
     return std::allocate_shared< JsonAssetWriter >(
          std::pmr::polymorphic_allocator< JsonAssetWriter >( &resource_ ),
          resource_, big_resource_
     );
}

} // namespace _16nar::assets
