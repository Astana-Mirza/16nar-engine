#include <16nar/core/resources/stb_texture_reader.h>

#include <16nar/platform/logger/logger.h>
#include <16nar/platform/memory/memory_manager.h>
#include <16nar/platform/memory/imemory_domain.h>

#include <16nar/core/resources/unified_storage.h>

#include <cstring>
#include <stdexcept>

#include <stb_image.h>

namespace _16nar::resources
{

StbTextureReader::StbTextureReader( std::pmr::memory_resource& big_resource ):
     big_resource_{ big_resource }
{}


memory::SharedBufferPtr StbTextureReader::read( strings::StaticName name, UnifiedStorage& storage )
{
     auto file_data = storage.load( name );
     if ( !file_data )
     {
          LOG_16NAR_ERROR( "Cannot load resource from unified storage" );
          return memory::SharedBufferPtr{};
     }

     const auto src_buf = file_data.get_const_view();
     int width{}, height{}, channels{};

     auto *data = ::stbi_load_from_memory( reinterpret_cast< const ::stbi_uc * >( src_buf.data ),
          static_cast< int >( src_buf.size ), &width, &height, &channels, 0 );
     if ( !data )
     {
          LOG_16NAR_ERROR( "Cannot decode image file data: %s", ::stbi_failure_reason() );
          return memory::SharedBufferPtr{};
     }

     std::unique_ptr< ::stbi_uc, decltype( &::stbi_image_free ) > guard{ data, &::stbi_image_free };
     const std::size_t size = static_cast< std::size_t >( width )
          * static_cast< std::size_t >( height )
          * static_cast< std::size_t >( channels )
          * sizeof( ::stbi_uc );
     auto result = memory::SharedBufferPtr::allocate( big_resource_, size );
     std::memcpy( result.get_view().data, data, size );
     return result;
}

} // namespace _16nar::resources
