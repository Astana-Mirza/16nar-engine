#include <16nar/tools/resources/stb_texture_reader.h>

#include <16nar/tools/logger/logger.h>
#include <16nar/tools/memory_manager.h>
#include <16nar/tools/memory_domain.h>
#include <16nar/tools/unified_storage.h>

#include <cstring>
#include <stdexcept>

#include <stb_image.h>

namespace _16nar::tools
{

StbTextureReader::StbTextureReader( MemoryManager& memory_manager ):
     memory_{}
{
     auto *memory_domain = memory_manager.get_domain( StaticName{ "asset" } );
     if ( !memory_domain )
     {
          throw std::runtime_error{ "asset memory domain is not present" };
     }
     memory_ = &memory_domain->get_resource();
}


SharedBufferPtr StbTextureReader::read( StaticName name, UnifiedStorage& storage )
{
     auto file_data = storage.load( name );
     if ( !file_data )
     {
          LOG_16NAR_ERROR( "Cannot load resource from unified storage" );
          return SharedBufferPtr{};
     }

     const auto src_buf = file_data.get_const_view();
     int width{}, height{}, channels{};

     auto *data = ::stbi_load_from_memory( reinterpret_cast< const ::stbi_uc * >( src_buf.data ),
          static_cast< int >( src_buf.size ), &width, &height, &channels, 0 );
     if ( !data )
     {
          LOG_16NAR_ERROR( "Cannot decode image file data: %s", ::stbi_failure_reason() );
          return SharedBufferPtr{};
     }

     std::unique_ptr< ::stbi_uc, decltype( &::stbi_image_free ) > guard{ data, &::stbi_image_free };
     const std::size_t size = static_cast< std::size_t >( width )
          * static_cast< std::size_t >( height )
          * static_cast< std::size_t >( channels )
          * sizeof( ::stbi_uc );
     auto result = SharedBufferPtr::allocate( *memory_, size );
     std::memcpy( result.get_view().data, data, size );
     return result;
}

} // namespace _16nar::tools
