#include <16nar/core/assets/json/json_asset_writer.h>

#include <16nar/core/assets/json/literals.h>

namespace _16nar::assets
{

JsonAssetWriter::JsonAssetWriter( std::pmr::memory_resource& resource,
     std::pmr::memory_resource& big_resource ):
     ids_( &resource ), result_( &big_resource ), current_id_{}
{}


std::uint32_t JsonAssetWriter::write_asset( std::string_view name, AssetData content,
          const std::uint32_t *children, std::uint32_t children_count, bool is_array )
{
     if ( !result_.empty() )
     {
          // already finished
          return 0;
     }

     nlohmann::json asset;
     nlohmann::json children_set{};
     nlohmann::json children_arr = nlohmann::json::array();
     const std::uint32_t children_total = children ? children_count : 0;
     for( std::uint32_t i = 0; i < children_total; ++i )
     {
          const auto id = children[ i ];
          auto iter = ids_.find( id );
          if ( iter == ids_.cend() )
          {
               return 0;
          }

          if ( is_array )
          {
               children_arr.push_back( std::move( iter->second ) );
          }
          else
          {
               const auto name_iter = iter->second.find( name_label );
               if ( name_iter == iter->second.cend() || !name_iter->is_string() )
               {
                    return 0;
               }
               const auto& child_name = name_iter->get< std::string >();
               if ( child_name.empty() )
               {
                    return 0;
               }
               if ( !children_set.emplace( child_name, std::move( *name_iter ) ).second )
               {
                    return 0;
               }
          }

          ids_.erase( iter );
     }

     asset[ is_array_label ] = is_array;
     asset[ children_label ] = is_array ? children_arr : children_set;
     asset[ name_label ] = name;

     if ( content.data )
     {
          nlohmann::json content_json;
          content_json[ type_id_label ] = content.type_id.hash;
          content_json[ data_label ] = std::string{
               reinterpret_cast< const char * >( content.data.data ), content.data.size };
          asset[ content_label ] = std::move( content_json );
     }
     else
     {
          asset[ content_label ] = nlohmann::json{};
     }

     const auto result = ++current_id_;
     ids_[ result ] = asset;
     return result;
}


memory::ConstByteView JsonAssetWriter::finish( std::uint32_t root_id )
{
     if ( !result_.empty() )
     {
          // already finished
          return memory::ConstByteView{};
     }

     const auto iter = ids_.find( root_id );
     if ( iter == ids_.cend() )
     {
          return memory::ConstByteView{};
     }

     result_ = iter->second.dump();
     return memory::ConstByteView{ reinterpret_cast< const std::byte * >( result_.c_str() ), result_.size() };
}


void JsonAssetWriter::reset()
{
     ids_.clear();
     result_.clear();
     current_id_ = 0;
}

} // namespace _16nar::assets
