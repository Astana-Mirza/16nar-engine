#include <16nar/core/assets/json/json_asset_reader.h>

#include <16nar/core/assets/json/literals.h>

namespace _16nar::assets
{
namespace
{

/// @brief Find child asset with given name.
/// @param[in] parent parent asset.
/// @param[in] name name of the child asset.
/// @return child asset or nullptr if it does not exist.
const nlohmann::json::object_t *find_child( const nlohmann::json::object_t *parent, std::string_view name )
{
     if ( !parent )
     {
          return nullptr;
     }
     const auto children_iter = parent->find( children_label );
     if ( children_iter == parent->cend() )
     {
          return nullptr;
     }
     const auto& children = children_iter->second;
     const auto iter = children.find( name );
     if ( iter == children.cend() )
     {
          return nullptr;
     }
     return iter->get_ptr< const nlohmann::json::object_t * >();
}

} // anonymous namespace


JsonAssetReader::JsonAssetReader( std::pmr::memory_resource& resource ) noexcept:
     json_{}, stack_( &resource ), current_{}, resource_{ resource }
{}


void JsonAssetReader::reset( memory::ConstByteView buffer )
{
     current_ = nullptr;
     stack_.clear();
     json_.clear();

     if ( !buffer )
     {
          throw std::runtime_error{ "asset buffer is empty" };
     }
     json_ = nlohmann::json::parse( std::string_view{
          reinterpret_cast< const char * >( buffer.data ), buffer.size } );
     current_ = json_.get_ptr< JsonPtr >();
}


bool JsonAssetReader::is_empty() const
{
     return current_ == nullptr;
}


std::string_view JsonAssetReader::get_name() const
{
     if ( !current_ )
     {
          return std::string_view{};
     }
     const auto iter = current_->find( name_label );
     if ( iter != current_->cend() )
     {
          return iter->second.get< std::string_view >();
     }
     return std::string_view{};
}


bool JsonAssetReader::is_array() const
{
     if ( !current_ )
     {
          return false;
     }
     const auto iter = current_->find( is_array_label );
     return iter != current_->cend() ? iter->second.get< bool >() : false;
}


AssetData JsonAssetReader::get_content() const
{
     if ( !current_ )
     {
          return AssetData{};
     }
     const auto content_iter = current_->find( content_label );
     if ( content_iter == current_->cend() )
     {
          return AssetData{};
     }
     const auto& content_data = content_iter->second;

     AssetData result{};
     result.type_id.hash = content_data.at( type_id_label ).get< std::uint64_t >();
     auto data = content_data.at( data_label ).get< std::string_view >();
     result.data = data.size()
          ? memory::ConstByteView{ reinterpret_cast< const std::byte * >( data.data() ), data.size() }
          : memory::ConstByteView{};
     return result;
}


bool JsonAssetReader::has_child( std::string_view name ) const
{
     const auto *child = find_child( current_, name );
     return child;
}


std::size_t JsonAssetReader::get_children_count() const
{
     if ( !current_ )
     {
          return 0;
     }
     const auto children_iter = current_->find( children_label );
     return ( children_iter != current_->cend() ) ? children_iter->second.size() : 0;
}


std::pmr::vector< std::pmr::string > JsonAssetReader::get_children_names() const
{
     std::pmr::vector< std::pmr::string > result( &resource_ );
     if ( !current_ || is_array() )
     {
          return result;
     }
     const auto children_iter = current_->find( children_label );
     if ( children_iter == current_->cend() )
     {
          return result;
     }

     for ( const auto& [ key, value ] : children_iter->second.items() )
     {
          result.emplace_back( key );
     }

     return result;
}


bool JsonAssetReader::to_child( std::string_view name )
{
     const auto *child = find_child( current_, name );
     if ( !child )
     {
          return false;
     }
     stack_.emplace_back( current_ );
     current_ = child;
     return true;
}


bool JsonAssetReader::to_child_index( std::size_t index )
{
     if ( !current_ )
     {
          return false;
     }
     const auto children_iter = current_->find( children_label );
     if ( children_iter == current_->cend() )
     {
          return false;
     }
     JsonPtr child{};
     const auto& children = children_iter->second;
     if ( index < children.size() )
     {
          if ( !children.is_array() )
          {
               auto iter = children.begin();
               std::advance( iter, index );
               child = iter->get_ptr< JsonPtr >();
          }
          else
          {
               child = children[ index ].get_ptr< JsonPtr >();
          }
     }

     if ( !child )
     {
          return false;
     }
     stack_.emplace_back( current_ );
     current_ = child;
     return true;
}


bool JsonAssetReader::to_parent()
{
     if ( stack_.empty() )
     {
          return false;
     }
     current_ = stack_.back();
     stack_.pop_back();
     return true;
}

} // namespace _16nar::tools
