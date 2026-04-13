#include <16nar/tools/json/json_asset_file_processor.h>

#include <16nar/tools/json/json_asset_reader.h>
#include <16nar/tools/json/json_asset_writer.h>
#include <16nar/tools/json/literals.h>

#include <cstdio> // sscanf

namespace _16nar::tools
{

JsonAssetFileProcessor::JsonAssetFileProcessor( std::pmr::memory_resource& memory_resource ) noexcept:
     memory_resource_{ memory_resource }
{}


SharedBufferPtr JsonAssetFileProcessor::read_asset_data( const File& file )
{
     const auto file_size = file.get_size();
     if ( file_size == 0 )
     {
          return SharedBufferPtr{};
     }
     auto ret = SharedBufferPtr::allocate( memory_resource_, file_size );
     if ( file_size != file.read( ret.get_view() ) )
     {
          return SharedBufferPtr{};
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
               return SharedBufferPtr{};
          }
          const std::uint32_t version_num = NARENGINE_VERSION_TO_UINT32( major, minor, patch );
          if ( !NARENGINE_VERSION_CHECK( version_num, NARENGINE_ASSET_VERSION_UINT32 ) )
          {
               return SharedBufferPtr{};
          }
     }
     catch ( const nlohmann::json::exception& )
     {
          return SharedBufferPtr{};
     }
     return ret;
}


bool JsonAssetFileProcessor::write_asset_data( ConstByteView buffer, File& file )
{
     if ( !buffer )
     {
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
     catch ( const nlohmann::json::exception& )
     {
          return false;
     }

     if ( dumped.size() != file.write( ConstByteView{
          reinterpret_cast< const std::byte * >( dumped.c_str() ), dumped.size() } ) )
     {
          return false;
     }
     return true;
}


IAssetReaderPtr JsonAssetFileProcessor::make_asset_reader( ConstByteView buffer )
{
     return std::make_shared< JsonAssetReader >( buffer );
}


IAssetWriterPtr JsonAssetFileProcessor::make_asset_writer()
{
     return std::make_shared< JsonAssetWriter >();
}

} // namespace _16nar::tools
