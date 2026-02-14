/// @file
/// @brief File with IAssetReader interface definition.
#ifndef _16NAR_TOOLS_IASSET_READER_H
#define _16NAR_TOOLS_IASSET_READER_H

#include <16nar/tools/defs.h>

#include <vector>
#include <string>
#include <string_view>
#include <memory>

namespace _16nar::tools
{

/// @brief Interface for reading data asset.
class ENGINE_API IAssetReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~IAssetReader() = default;

     /// @brief Check if the reader is empty and does not have an asset to read.
     /// @return true if the reader is empty, false otherwise.
     virtual bool is_empty() const = 0;

     /// @brief Get name of the asset.
     /// @return name of the asset, may be empty.
     virtual std::string_view get_name() const = 0;

     /// @brief Check if current asset's children are stored as an array.
     /// @details Children may be stored as an array or as a set. In case of an array, indexed
     /// access should be used. In case of a set, access by name should be used.
     /// @return true if children are stored as an array, false if they are stored as a set.
     virtual bool is_array() const = 0;

     /// @brief Get asset data stored in the asset, if any.
     /// @return binary asset data, may be empty (nullptr and zero size buffer).
     virtual AssetData get_content() const = 0;

     /// @brief Check if the asset has child asset with given name.
     /// @param[in] name name of child asset.
     /// @return true if child with given name exists, false otherwise.
     virtual bool has_child( std::string_view name ) const = 0;

     /// @brief Get count of child assets.
     /// @return count of child assets.
     virtual std::size_t get_children_count() const = 0;

     /// @brief Get names of child assets.
     /// @return names of child assets, empty vector if current node's children are not stored as a set.
     virtual std::vector< std::string > get_children_names() const = 0;

     /// @brief Set current context for reading data of the child asset with given name.
     /// @details Implementation may invalidate data pointed to views returned
     /// by @b get_content() and @b get_name() after calling this function.
     /// Caller should ensure that no dangling references remain.
     /// @param[in] name name of the child.
     /// @return true if context was switched to child asset, false otherwise (e.g. the child does not exist).
     virtual bool to_child( std::string_view name ) = 0;

     /// @brief Set current context for reading data of the child asset with given index.
     /// @details Implementation may invalidate data pointed to views returned
     /// by @b get_content() and @b get_name() after calling this function.
     /// Caller should ensure that no dangling references remain.
     /// @param[in] name name of the child.
     /// @return true if context was switched to child asset, false otherwise (e.g. the child does not exist).
     virtual bool to_child_index( std::size_t index ) = 0;

     /// @brief Set current context for reading data of the parent asset.
     /// @details Implementation may invalidate data pointed to views returned
     /// by @b get_content() and @b get_name() after calling this function.
     /// Caller should ensure that no dangling references remain.
     /// @return true if context was switched to parent asset, false otherwise (e.g. the parent does not exist).
     virtual bool to_parent() = 0;
};

using IAssetReaderPtr = std::shared_ptr< IAssetReader >;

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IASSET_READER_H
