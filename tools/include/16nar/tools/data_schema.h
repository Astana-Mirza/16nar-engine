/// @file
/// @brief File with definition of DataSchema structure.
#ifndef _16NAR_TOOLS_DATA_SCHEMA_H
#define _16NAR_TOOLS_DATA_SCHEMA_H

#include <16nar/tools/scene_defs.h>
#include <16nar/tools/assets/iprops_reader.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace _16nar::tools
{

/// @brief Item of data schema.
struct DataItem
{
     std::uint16_t index{};                            ///< index of item in data schema.
     StoredDataType type = StoredDataType::Unknown;    ///< data type.
     bool mandatory{};                                 ///< necessity of presense in stored data.
};


/// @brief Schema of stored binary data.
struct DataSchema
{
     /// @brief Set data reader of default values.
     /// @param default_vals reader of default values.
     inline void set_default_vals( std::shared_ptr< IPropsReader > default_vals ) noexcept
     {
          default_vals_ = default_vals;
     }

     /// @brief Get data reader of default values.
     /// @return reader of default values.
     inline std::shared_ptr< IPropsReader > get_default_vals() const noexcept
     {
          if ( default_vals_ )
          {
               default_vals_->set_data_schema( *this );
          }
          return default_vals_;
     }

     std::unordered_map< std::string, DataItem > items;     ///< data items.
     std::vector< std::string > ordered_items;              ///< names of data items in original order.

private:
     std::shared_ptr< IPropsReader > default_vals_;                   ///< default values reader.
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_DATA_SCHEMA_H
