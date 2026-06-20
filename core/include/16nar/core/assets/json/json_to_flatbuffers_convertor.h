/// @file
/// @brief File with JsonToFlatBuffersConvertor class definition.
#ifndef _16NAR_CORE_ASSETS_JSON_TO_FLATBUFFERS_CONVERTOR_H
#define _16NAR_CORE_ASSETS_JSON_TO_FLATBUFFERS_CONVERTOR_H

#include <16nar/core/assets/iasset_data_convertor.h>

#include <16nar/platform/strings/name_table.h>

#include <flatbuffers/idl.h>

#include <vector>
#include <string_view>
#include <unordered_map>

namespace _16nar::assets
{

/// @brief Class for conversion asset data from JSON to flatbuffers format.
/// @details If the name for converted type is not set, convertor will attempt to convert to
/// "root_type" of latest loaded schema. The name must include namespaces, e.g. "my_project.data.my_type".
class NARENGINE_ASSETS_JSON_API JsonToFlatBuffersConvertor : public IAssetDataConvertor
{
public:
     /// @brief Constructor.
     /// @param[in] type_names table of names.
     JsonToFlatBuffersConvertor( strings::NameTable& type_names );

     /// @brief Add flatbuffers schema to convertor.
     /// @details This function resets current converted asset data, if any.
     /// Schemas are accumulated in convertor, so they must not have name conflicts.
     /// All passed strings must be null-terminated.
     /// @param[in] schema_data text data of flatbuffers schema.
     /// @param[in] include_paths optional directory paths for resolving include
     /// statements in schemas (current directory is default).
     /// @param[in] schema_path optional path of the schema file.
     /// @return true on success, false otherwise.
     bool add_schema( const std::string& schema_data,
          const std::vector< std::string >& include_paths = {},
          const std::string& schema_path = {} );

     /// @copydoc IAssetDataConvertor::convert_forward(AssetData)
     AssetData convert_forward( AssetData content ) override;

     /// @copydoc IAssetDataConvertor::convert_backward(AssetData)
     AssetData convert_backward( AssetData content ) override;

     /// @copydoc IAssetDataConvertor::get_error_description()
     std::string_view get_error_description() const override;

private:
     /// @brief Sets root type name to parser if its name is registered.
     /// @param[in] type_id type identifier.
     /// @return true on success or if name is not registered, false on error.
     bool set_root_type( strings::StaticName type_id );

private:
     flatbuffers::Parser parser_;       ///< flatbuffers parser.
     strings::NameTable& type_names_;   ///< type names.
     std::string json_out_;             ///< JSON output of backward conversion.
     std::string error_;                ///< latest error message.
};

} // namespace _16nar::assets

#endif // #ifndef _16NAR_CORE_ASSETS_JSON_TO_FLATBUFFERS_CONVERTOR_H
