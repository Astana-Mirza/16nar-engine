#include <16nar/core/assets/flatbuffers/flatbuffers_asset_writer.h>

#include <16nar/gen/flatbuffers/asset_generated.h>

#include <unordered_set>
#include <cassert>

namespace _16nar::assets
{

FlatBuffersAssetWriter::FlatBuffersAssetWriter() noexcept:
     allocator_{}, builder_{}, ids_{}, current_id_{}, finished_{}
{}


FlatBuffersAssetWriter::FlatBuffersAssetWriter(
     std::pmr::memory_resource& resource, std::size_t initial_size, bool check_names ):
     allocator_{ resource }, builder_{ initial_size, &allocator_ },
     asset_names_{}, ids_{}, current_id_{}, finished_{}, check_names_{ check_names }
{}


std::uint32_t FlatBuffersAssetWriter::write_asset( std::string_view name, AssetData content,
          const std::vector< std::uint32_t >& children, bool is_array )
{
     if ( finished_ )
     {
          return 0;
     }

     std::vector< flatbuffers::Offset< _16nar::data::Asset > > children_offsets{};
     flatbuffers::Offset< flatbuffers::Vector<
          flatbuffers::Offset< _16nar::data::Asset > > > children_offset{ 0 };
     flatbuffers::Offset< flatbuffers::String > name_offset{ 0 };
     flatbuffers::Offset< _16nar::data::AssetData > content_offset{ 0 };

     std::unordered_set< std::string > children_names;
     for( const auto id : children )
     {
          const auto iter = ids_.find( id );
          if ( iter == ids_.cend() )
          {
               return 0;
          }

          if ( check_names_ )
          {
               const auto name_iter = asset_names_.find( id );
               if( !is_array )
               {
                    if ( name_iter == asset_names_.cend() && name_iter->second.empty() )
                    {
                         return 0;
                    }
                    if ( !children_names.insert( name_iter->second ).second )
                    {
                         return 0;
                    }
               }
               asset_names_.erase( name_iter );
          }

          children_offsets.emplace_back( iter->second.o );
          ids_.erase( iter );
     }
     if ( !children_offsets.empty() )
     {
          children_offset = is_array ?
               builder_.CreateVector( children_offsets ) :
               builder_.CreateVectorOfSortedTables< _16nar::data::Asset >( &children_offsets );
     }

     if ( !name.empty() )
     {
          name_offset = builder_.CreateString( name.data(), name.size() );
     }

     if ( content.data )
     {
          builder_.ForceVectorAlignment( content.data.size,
               sizeof( std::uint8_t ), alignof( std::max_align_t ) );
          const auto data_offset = builder_.CreateVector< std::uint8_t >(
               reinterpret_cast< const std::uint8_t * >( content.data.data ), content.data.size );
          content_offset = _16nar::data::CreateAssetData( builder_, content.type_id, data_offset );
     }

     const auto asset_offset = _16nar::data::CreateAsset(
          builder_, name_offset, is_array, content_offset, children_offset );
     const auto result = ++current_id_;
     ids_[ result ] = asset_offset.Union();

     if ( check_names_ && !name.empty() )
     {
          asset_names_[ result ] = std::string( name.data(), name.size() );
     }

     return result;
}


memory::ConstByteView FlatBuffersAssetWriter::finish( std::uint32_t root_id )
{
     if ( finished_ )
     {
          return memory::ConstByteView{};
     }

     const auto iter = ids_.find( root_id );
     if ( iter == ids_.cend() )
     {
          return memory::ConstByteView{};
     }

     flatbuffers::Offset< _16nar::data::Asset > asset_offset{ iter->second.o };
     ids_.erase( iter );
     _16nar::data::FinishAssetBuffer( builder_, asset_offset );

     const std::uint8_t *buf = builder_.GetBufferPointer();
     const std::size_t size = builder_.GetSize();
     finished_ = true;

     return memory::ConstByteView{ reinterpret_cast< const std::byte * >( buf ), size };
}


void FlatBuffersAssetWriter::reset()
{
     builder_.Reset();
     ids_.clear();
     current_id_ = 0;
     finished_ = false;
}

} // namespace _16nar::tools
