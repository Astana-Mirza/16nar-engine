/// @file
/// @brief File with FlatBuffersAssetReader class definition.
#ifndef _16NAR_CORE_ASSETS_FLATBUFFERS_ASSET_READER_H
#define _16NAR_CORE_ASSETS_FLATBUFFERS_ASSET_READER_H

#include <16nar/core/assets/iasset_reader.h>

#include <stack>

namespace _16nar::data
{

struct Asset;

} // namespace _16nar::data


namespace _16nar::assets
{

/// @brief Class for reading data asset in flatbuffers format.
class NARENGINE_ASSETS_FB_API FlatBuffersAssetReader : public IAssetReader
{
public:
     /// @brief Default constructor.
     FlatBuffersAssetReader() noexcept;

     FlatBuffersAssetReader( const FlatBuffersAssetReader& ) = delete;
     FlatBuffersAssetReader& operator=( const FlatBuffersAssetReader& ) = delete;

     /// @copydoc IAssetReader::reset(memory::ConstByteView)
     void reset( memory::ConstByteView buffer ) override;

     /// @copydoc IAssetReader::is_empty() const
     bool is_empty() const override;

     /// @copydoc IAssetReader::get_name() const
     std::string_view get_name() const override;

     /// @copydoc IAssetReader::is_array() const
     bool is_array() const override;

     /// @copydoc IAssetReader::get_content() const
     AssetData get_content() const override;

     /// @copydoc IAssetReader::has_child(std::string_view) const
     bool has_child( std::string_view name ) const override;

     /// @copydoc IAssetReader::get_children_count() const
     std::size_t get_children_count() const override;

     /// @copydoc IAssetReader::get_children_names() const
     std::vector< std::string > get_children_names() const override;

     /// @copydoc IAssetReader::to_child(std::string_view)
     bool to_child( std::string_view name ) override;

     /// @copydoc IAssetReader::to_child_index(std::size_t)
     bool to_child_index( std::size_t index ) override;

     /// @copydoc IAssetReader::to_parent()
     bool to_parent() override;

private:
     std::stack< const data::Asset * > stack_;    ///< current stack of parent assets.
     const data::Asset *current_;                 ///< current asset object.
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_FLATBUFFERS_ASSET_READER_H
