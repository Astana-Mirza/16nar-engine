/// @file
/// @brief File with FlatBuffersAssetWriter class definition.
#ifndef _16NAR_CORE_ASSETS_FLATBUFFERS_ASSET_WRITER_H
#define _16NAR_CORE_ASSETS_FLATBUFFERS_ASSET_WRITER_H

#include <16nar/core/assets/iasset_writer.h>

#include <16nar/core/assets/flatbuffers/flatbuffers_pmr_allocator.h>

#include <flatbuffers/flatbuffer_builder.h>

#include <unordered_map>
#include <memory_resource>
#include <string>

namespace _16nar::assets
{

/// @brief Class for reading data asset in flatbuffers format.
class NARENGINE_ASSETS_FB_API FlatBuffersAssetWriter : public IAssetWriter
{
public:
     /// @brief Default constructor.
     FlatBuffersAssetWriter() noexcept;

     /// @brief Constructor.
     /// @param[in] resource memory resource for the allocation.
     /// @param[in] initial_size initial size of builder buffer.
     /// @param[in] check_names true if name uniqueness needs to be
     /// checked while saving children sets (has performance and memory costs), false otherwise.
     FlatBuffersAssetWriter( std::pmr::memory_resource& resource,
          std::size_t initial_size, bool check_names = false );

     /// @copydoc IAssetWriter::write_asset(std::string_view, AssetData, const std::vector<std::uint32_t>&, bool)
     std::uint32_t write_asset( std::string_view name, AssetData content,
          const std::vector< std::uint32_t >& children = {}, bool is_array = false ) override;

     /// @copydoc IAssetWriter::finish(std::uint32_t)
     memory::ConstByteView finish( std::uint32_t root_id ) override;

     /// @copydoc IAssetWriter::reset()
     void reset() override;

private:
     FlatBuffersPmrAllocator allocator_;                              ///< allocator for buffer builder.
     flatbuffers::FlatBufferBuilder builder_;                         ///< asset buffer builder.
     std::unordered_map< std::uint32_t, std::string > asset_names_;   ///< names of assets.
     std::unordered_map< std::uint32_t, flatbuffers::Offset<> > ids_; ///< identifiers of written assets.
     std::uint32_t current_id_;                                       ///< current asset id.
     bool finished_;                                                  ///< flag of finished buffer.
     bool check_names_;                                               ///< check names of children in sets.
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_FLATBUFFERS_ASSET_WRITER_H
