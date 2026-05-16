#include <16nar/core/assets/flatbuffers/flatbuffers_asset_reader.h>

#include <16nar/gen/flatbuffers/asset_generated.h>

#include <cassert>
#include <stdexcept>

namespace _16nar::assets
{
namespace
{

/// @brief Find child asset with given name.
/// @param[in] parent parent asset.
/// @param[in] name name of the child asset.
/// @return child asset or nullptr if it does not exist.
const _16nar::data::Asset *find_child( const _16nar::data::Asset *parent, std::string_view name )
{
     if ( !parent )
     {
          return nullptr;
     }
     const auto *children = parent->children();
     if ( !children )
     {
          return nullptr;
     }
     return children->LookupByKey( name );
}

} // anonymous namespace


FlatBuffersAssetReader::FlatBuffersAssetReader() noexcept:
     stack_{}, current_{}
{}


void FlatBuffersAssetReader::reset( memory::ConstByteView buffer )
{
     current_ = nullptr;
     stack_ = std::stack< const data::Asset * >{};

     if ( !buffer )
     {
          throw std::runtime_error{ "asset buffer is empty" };
     }

     const auto *data = reinterpret_cast< const std::uint8_t * >( buffer.data );
     flatbuffers::Verifier verifier{ data, buffer.size };
     if ( !_16nar::data::VerifyAssetBuffer( verifier ) )
     {
          throw std::runtime_error{ "asset buffer is corrupted" };
     }
     current_ = _16nar::data::GetAsset( data );
}


bool FlatBuffersAssetReader::is_empty() const
{
     return current_ == nullptr;
}


std::string_view FlatBuffersAssetReader::get_name() const
{
     if ( !current_ )
     {
          return std::string_view{};
     }
     const auto *str = current_->name();
     if ( str )
     {
          assert( str->c_str() != nullptr );
          return std::string_view( str->c_str(), str->size() );
     }
     return std::string_view{};
}


bool FlatBuffersAssetReader::is_array() const
{
     return current_ ? current_->is_array() : false;
}


AssetData FlatBuffersAssetReader::get_content() const
{
     if ( !current_ )
     {
          return AssetData{};
     }
     const auto *content = current_->content();
     if ( !content )
     {
          return AssetData{};
     }
     const auto *content_data = content->data();
     assert( content_data != nullptr );

     AssetData result{};
     result.type_id = content->type_id();
     result.data = content_data->size() ?
          memory::ConstByteView{ reinterpret_cast< const std::byte * >( content_data->data() ),
          content_data->size() } : memory::ConstByteView{};
     return result;
}


bool FlatBuffersAssetReader::has_child( std::string_view name ) const
{
     const auto *child = find_child( current_, name );
     return child;
}


std::size_t FlatBuffersAssetReader::get_children_count() const
{
     if ( !current_ )
     {
          return 0;
     }
     const auto *children = current_->children();
     return children ? children->size() : 0;
}


std::vector< std::string > FlatBuffersAssetReader::get_children_names() const
{
     std::vector< std::string > result{};
     if ( !current_ || is_array() )
     {
          return result;
     }

     const auto *children = current_->children();
     if ( !children )
     {
          return result;
     }

     for ( std::size_t i = 0; i < children->size(); ++i )
     {
          const auto *child = children->Get( i );
          if ( !child )
          {
               continue;
          }
          const auto *str = child->name();
          if ( str )
          {
               assert( str->c_str() != nullptr );
               result.emplace_back( str->c_str() );
          }
     }

     return result;
}


bool FlatBuffersAssetReader::to_child( std::string_view name )
{
     const auto *child = find_child( current_, name );
     if ( !child )
     {
          return false;
     }
     stack_.emplace( current_ );
     current_ = child;
     return true;
}


bool FlatBuffersAssetReader::to_child_index( std::size_t index )
{
     if ( !current_ )
     {
          return false;
     }
     const auto *children = current_->children();
     if ( !children || children->size() <= index )
     {
          return false;
     }
     const auto *child = children->Get( index );
     if ( !child )
     {
          return false;
     }
     stack_.emplace( current_ );
     current_ = child;
     return true;
}


bool FlatBuffersAssetReader::to_parent()
{
     if ( stack_.empty() )
     {
          return false;
     }
     current_ = stack_.top();
     stack_.pop();
     return true;
}

} // namespace _16nar::assets
