/// @file
/// @brief File with StbTextureReader class definition.
#ifndef _16NAR_CORE_RESOURCES_STB_TEXTURE_READER_H
#define _16NAR_CORE_RESOURCES_STB_TEXTURE_READER_H

#include <16nar/core/resources/defs.h>

#include <16nar/core/resources/iresource_reader.h>

#include <memory_resource>

namespace _16nar::resources
{

/// @brief Reader of textures in various image file formats using stb_image library.
/// @details Texture data is read using stbi_load_from_memory(). The source memory
/// is passed from unified storage.
class NARENGINE_CORE_API StbTextureReader : public IResourceReader
{
public:
     /// @brief Constructor.
     /// @param[in] big_resource memory resource for payload allocations.
     StbTextureReader( std::pmr::memory_resource& big_resource );

     /// @copydoc IResourceReader::read(strings::StaticName, UnifiedStorage&)
     memory::SharedBufferPtr read( strings::StaticName name, UnifiedStorage& storage ) override;

private:
     std::pmr::memory_resource& big_resource_;    ///< memory resource for payload allocations.
};

} // namespace _16nar::resources

#endif // #ifndef _16NAR_CORE_RESOURCES_STB_TEXTURE_READER_H
