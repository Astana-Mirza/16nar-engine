#include <16nar/tools/unified_storage.h>

#include <16nar/tools/name_table.h>
#include <16nar/tools/resource_pkg_visitor.h>
#include <16nar/tools/memory_manager.h>
#include <16nar/tools/memory_domain.h>
#include <16nar/tools/iasset_file_processor.h>

#include <stdexcept>

namespace _16nar::tools
{
namespace
{

class ResourceParser : public ResourcePkgVisitor
{
public:
     ResourceParser( UnifiedStorage& storage ):
          storage_{ storage }
     {}

     void visit( AssetActionId action_id, const IAssetReader& reader ) override
     {
          switch ( action_id )
          {
               case ResourcePkgVisitor::PackageBegin:
                    break;
               case ResourcePkgVisitor::ResourceEnter:
                    break;
               case ResourcePkgVisitor::PackageEnd:
                    break;
               case ResourcePkgVisitor::ResourceBegin:
                    break;
               case ResourcePkgVisitor::ResourceEnd:
                    break;
               default:
                    break;
          }
     }

private:
     UnifiedStorage& storage_;
};

} // anonymous namespace


UnifiedStorage::UnifiedStorage(
     const std::filesystem::path& base_dir,
     MemoryManager& memory_manager,
     NameTablePtr name_table,
     IAssetFileProcessorPtr file_processor ):
     resources_{}, packages_{}, base_dir_{ base_dir },
     name_table_{ name_table },
     file_processor_{ file_processor },
     convertor_{}, memory_{}, unpacked_{}
{
     if ( !name_table_ || !file_processor_ )
     {
          throw std::runtime_error{ "nullptr passed into unified storage constructor" };
     }

     memory_ = memory_manager.get_domain( StaticName{ "asset" } );
     if ( !memory_ )
     {
          throw std::runtime_error{ "asset memory domain is not present" };
     }
}


void UnifiedStorage::set_unpacked_mode( bool mode ) noexcept
{
     unpacked_ = mode;
}


bool UnifiedStorage::get_unpacked_mode() const noexcept
{
     return unpacked_;
}


const std::filesystem::path& UnifiedStorage::get_base_dir() const noexcept
{
     return base_dir_;
}


std::filesystem::path UnifiedStorage::get_path( StaticName name ) const
{
     std::filesystem::path result{};
     const auto data = name_table_->get_name( name );
     if ( !data.empty() )
     {
          result = base_dir_ / data;
     }
     return result;
}


SharedBufferPtr UnifiedStorage::load( StaticName name ) const
{
     return SharedBufferPtr{};
}


bool UnifiedStorage::mount( const std::filesystem::path& path )
{
     if ( unpacked_ )
     {
          return false;
     }

     ResourcePackage package{};
     const auto full_path{ base_dir_ / path };
     File asset_file{};
     if ( !asset_file.open( full_path ) )
     {
          return false;
     }

     auto data = file_processor_->read_asset_data( asset_file );
     if ( !data )
     {
          return false;
     }

     try
     {
          auto asset_reader = file_processor_->make_asset_reader( data.get_const_view() );
          ResourceParser parser{ *this };
          parser.process_resource_package( *asset_reader );
     }
     catch ( const std::exception& )
     {
          return false;
     }
     return true;
}


void UnifiedStorage::set_convertor( IAssetDataConvertorPtr convertor ) noexcept
{
     convertor_ = convertor;
}


IAssetFileProcessorPtr UnifiedStorage::get_file_processor() const noexcept
{
     return file_processor_;
}


IAssetDataConvertorPtr UnifiedStorage::get_convertor() const noexcept
{
     return convertor_;
}

} // namespace _16nar::tools
