/// @file
/// @brief File with StbTextureReader class definition.
#ifndef _16NAR_TOOLS_STB_TEXTURE_READER_H
#define _16NAR_TOOLS_STB_TEXTURE_READER_H

#include <16nar/tools/defs.h>

#include <16nar/tools/resources/iresource_reader.h>

namespace _16nar::tools
{

/// @brief Reader of textures in various image file formats using stb_image library.
/// @details Texture data is read using stbi_load_from_memory(). The source memory
/// is passed from unified storage.
class ENGINE_API StbTextureReader : public IResourceReader
{
public:
     /// @brief Constructor.
     /// @throws std::runtime_error if no suitable memory domain ("asset") present in @b memory_manager.
     /// @param[in] memory_manager memory manager.
     StbTextureReader( MemoryManager& memory_manager );

     /// @copydoc IResourceReader::read(StaticName, UnifiedStorage&)
     SharedBufferPtr read( StaticName name, UnifiedStorage& storage ) override;

private:
     std::pmr::memory_resource *memory_;     ///< memory resource for asset reading allocations.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_STB_TEXTURE_READER_H
