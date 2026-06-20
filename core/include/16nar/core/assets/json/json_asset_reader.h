/// @file
/// @brief File with JsonAssetReader class definition.
#ifndef _16NAR_CORE_ASSETS_JSON_ASSET_READER_H
#define _16NAR_CORE_ASSETS_JSON_ASSET_READER_H

#include <16nar/core/assets/iasset_reader.h>

#include <stack>

#include <nlohmann/json.hpp>

namespace _16nar::assets
{

/// @brief Class for reading data asset in JSON format.
/// @details Functions may throw implementation-defined exceptions.
class NARENGINE_ASSETS_JSON_API JsonAssetReader : public IAssetReader
{
public:
     /// @brief Default constructor.
     /// @param[in] resource memory resource for utiliy data allocations.
     JsonAssetReader( std::pmr::memory_resource& resource ) noexcept;

     JsonAssetReader( const JsonAssetReader& ) = delete;
     JsonAssetReader& operator=( const JsonAssetReader& ) = delete;

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
     std::pmr::vector< std::pmr::string > get_children_names() const override;

     /// @copydoc IAssetReader::to_child(std::string_view)
     bool to_child( std::string_view name ) override;

     /// @copydoc IAssetReader::to_child_index(std::size_t)
     bool to_child_index( std::size_t index ) override;

     /// @copydoc IAssetReader::to_parent()
     bool to_parent() override;

private:
     using JsonPtr = const nlohmann::json::object_t *;

     nlohmann::json json_;                   ///< JSON with asset data.
     std::pmr::vector< JsonPtr > stack_;     ///< current stack of parent assets.
     JsonPtr current_;                       ///< current asset object.
     std::pmr::memory_resource& resource_;   ///< memory resource for utiliy data allocations.
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_JSON_ASSET_READER_H
