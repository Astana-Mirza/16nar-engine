#include <16nar/core/resources/unified_storage.h>

#include <16nar/platform/logger/logger.h>
#include <16nar/platform/memory/memory_manager.h>
#include <16nar/platform/memory/imemory_domain.h>

#include <16nar/core/assets/iasset_file_processor.h>
#include <16nar/core/assets/iasset_data_convertor.h>
#include <16nar/core/resources/resource_pkg_visitor.h>

#include <16nar/gen/flatbuffers/resource_generated.h>

#include <stdexcept>

namespace _16nar::resources
{
namespace
{

constexpr char pkg_asset_name[] = "_16nar.data.ResourcePackage";

} // anonymous namespace


class ResourceParser : public ResourcePkgVisitor
{
public:
     ResourceParser( UnifiedStorage& storage, strings::StaticName package_name ):
          storage_{ storage }, package_name_{ package_name }
     {}

     void visit( assets::AssetActionId action_id, const assets::IAssetReader& reader ) override
     {
          if ( action_id != ResourcePkgVisitor::PackageBegin )
          {
               return;
          }

          auto content = reader.get_content();
          if ( !content.data || content.type_id != strings::StaticName( pkg_asset_name ) )
          {
               throw std::runtime_error{ "resource package buffer is corrupted" };
          }
          if ( storage_.convertor_ )
          {
               content = storage_.convertor_->convert_forward( content );
               if ( !content.data )
               {
                    const auto desc = storage_.convertor_->get_error_description();
                    throw std::runtime_error{ std::string{ desc.begin(), desc.end() } };
               }
          }
          const auto data = reinterpret_cast< const std::uint8_t * >( content.data.data );
          flatbuffers::Verifier verifier{ data, content.data.size };
          if ( !_16nar::data::VerifyResourcePackageBuffer( verifier ) )
          {
               throw std::runtime_error{ "resource package flat buffer is corrupted" };
          }
          const auto *package_buf = _16nar::data::GetResourcePackage( data );
          UnifiedStorage::ResourcePackage package{};
          package.chunk_size = package_buf->chunk_size();
          if ( !package_buf->database() || !package.chunk_size
               || !package_buf->database()->c_str() || !package_buf->resources() )
          {
               throw std::runtime_error{ "resource package is corrupted" };
          }

          const auto database_path = UnifiedStorage::correct_path(
               storage_.base_dir_, package_buf->database()->c_str() ).string();
          if ( !package.database.open( database_path.c_str() ) )
          {
               throw std::runtime_error{ "cannot open resource package database " + database_path };
          }
          storage_.packages_.emplace( package_name_, std::move( package ) );

          for ( const auto res_ptr : *package_buf->resources() )
          {
               UnifiedStorage::ResourceDesc desc{};
               desc.type_id.hash = res_ptr->hash().type_id();
               desc.chunk_id = res_ptr->chunk_id();
               desc.size = res_ptr->size();
               desc.orig_size = res_ptr->orig_size();
               desc.package = package_name_;

               strings::StaticName res_name{};
               res_name.hash = res_ptr->hash().name();
               if ( res_name.empty() || !desc.size || !desc.orig_size )
               {
                    throw std::runtime_error{ "resource description is corrupted" };
               }

               const auto result = storage_.resources_.emplace( res_name, desc );
               if ( result.second )
               {
                    continue;
               }
               if ( result.first->second.type_id == desc.type_id )
               {
                    LOG_16NAR_DEBUG( "Patching resource '%s'",
                         storage_.name_table_->get_name( res_name, true ).data() );
                    result.first->second = desc;
               }
               else
               {
                    throw std::runtime_error{ "cannot add resource "
                         + std::string{ storage_.name_table_->get_name( res_name, true ) }
                         + " - possible type mismatch or hash collision" };
               }
          }
     }

private:
     UnifiedStorage& storage_;
     strings::StaticName package_name_;
};


std::filesystem::path UnifiedStorage::correct_path(
     const std::filesystem::path& base_dir,
     const std::filesystem::path& path )
{
     if ( !base_dir.empty() && path.is_relative() )
     {
          return base_dir / path;
     }
     return path;
}


UnifiedStorage::UnifiedStorage(
     const std::filesystem::path& base_dir,
     memory::MemoryManager& memory_manager,
     strings::NameManager& name_manager,
     assets::IAssetFileProcessorPtr file_processor ):
     resources_{}, packages_{}, base_dir_{ base_dir },
     name_table_{}, file_processor_{ file_processor },
     asset_reader_{}, convertor_{}, memory_{}, unpacked_{}
{
     if ( !file_processor_ )
     {
          throw std::runtime_error{ "file processor is not present" };
     }

     name_table_ = name_manager.get_table( "asset" );
     if ( !name_table_ )
     {
          throw std::runtime_error{ "asset name table is not present" };
     }

     auto *memory_domain = memory_manager.get_domain( strings::StaticName{ "asset" } );
     if ( !memory_domain )
     {
          throw std::runtime_error{ "asset memory domain is not present" };
     }
     memory_ = &memory_domain->get_resource();
     asset_reader_ = file_processor_->make_asset_reader();
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


std::filesystem::path UnifiedStorage::get_path( strings::StaticName name ) const
{
     std::filesystem::path result{};
     const auto data = name_table_->get_name( name );
     if ( !data.empty() )
     {
          return correct_path( base_dir_, data );
     }
     return result;
}


memory::SharedBufferPtr UnifiedStorage::load( strings::StaticName name )
{
     if ( unpacked_ )
     {
          system::File file;
          const auto path = get_path( name ).string();
          if ( path.empty() || !file.open( path ) )
          {
               LOG_16NAR_ERROR( "Cannot open unpacked resource '%s'", path.c_str() );
               return memory::SharedBufferPtr{};
          }
          const std::size_t size = file.get_size();
          if ( !size )
          {
               LOG_16NAR_ERROR( "Cannot get size of unpacked resource '%s'", path.c_str() );
               return memory::SharedBufferPtr{};
          }
          auto buffer = memory::SharedBufferPtr::allocate( *memory_, size );
          if ( size != file.read( buffer.get_view() ) )
          {
               LOG_16NAR_ERROR( "Cannot read data of unpacked resource '%s'", path.c_str() );
               return memory::SharedBufferPtr{};
          }
          return buffer;
     }

     const auto iter = resources_.find( name );
     if ( iter == resources_.cend() )
     {
          LOG_16NAR_ERROR( "Cannot find resource '%s'",
               name_table_->get_name( name, true ).data() );
          return memory::SharedBufferPtr{};
     }
     const auto& desc = iter->second;
     const auto pkg_iter = packages_.find( desc.package );
     if ( pkg_iter == packages_.end() )
     {
          LOG_16NAR_ERROR( "Cannot find package '%s' while loading resource '%s'",
               name_table_->get_name( desc.package, true ).data(),
               name_table_->get_name( name, true ).data() );
          return memory::SharedBufferPtr{};
     }
     auto& package = pkg_iter->second;
     if ( !package.database.seek( desc.chunk_id * package.chunk_size, system::File::SeekOrigin::Set ) )
     {
          LOG_16NAR_ERROR( "Cannot seek chunk %u in package '%s' while loading resource '%s'",
               desc.chunk_id,
               name_table_->get_name( desc.package, true ).data(),
               name_table_->get_name( name, true ).data() );
          return memory::SharedBufferPtr{};
     }
     auto buffer = memory::SharedBufferPtr::allocate( *memory_, desc.orig_size );
     if ( desc.orig_size != package.database.read( buffer.get_view() ) )
     {
          LOG_16NAR_ERROR( "Cannot read resource '%s' from package '%s'",
               name_table_->get_name( name, true ).data(),
               name_table_->get_name( desc.package, true ).data() );
          return memory::SharedBufferPtr{};
     }
     return buffer;
}


bool UnifiedStorage::mount( const std::filesystem::path& path )
{
     const auto full_path = correct_path( base_dir_, path ).string();
     if ( unpacked_ )
     {
          LOG_16NAR_INFO( "Mount package '%s' (disabled in unpacked mode)", full_path.c_str() );
          return true;
     }

     strings::StaticName name{ path.string().c_str() };
     if ( packages_.count( name ) )
     {
          LOG_16NAR_INFO( "Package '%s' is already mounted", full_path.c_str() );
          return true;
     }

     LOG_16NAR_DEBUG( "Mounting package '%s'...", full_path.c_str() );
     system::File asset_file{};
     if ( !asset_file.open( full_path ) )
     {
          LOG_16NAR_ERROR( "Cannot open package file '%s'", full_path.c_str() );
          return false;
     }

     auto data = file_processor_->read_asset_data( asset_file );
     if ( !data )
     {
          LOG_16NAR_ERROR( "Cannot read data of package '%s'", full_path.c_str() );
          return false;
     }

     try
     {
          asset_reader_->reset( data.get_const_view() );
          ResourceParser parser{ *this, name };
          parser.process_resource_package( *asset_reader_ );
     }
     catch ( const std::exception& ex )
     {
          LOG_16NAR_ERROR( "Cannot mount package '%s': %s", full_path.c_str(), ex.what() );
          return false;
     }

     LOG_16NAR_INFO( "Mounted package '%s'", full_path.c_str() );
     return true;
}


void UnifiedStorage::set_convertor( assets::IAssetDataConvertorPtr convertor ) noexcept
{
     convertor_ = convertor;
}


assets::IAssetFileProcessorPtr UnifiedStorage::get_file_processor() const noexcept
{
     return file_processor_;
}


assets::IAssetDataConvertorPtr UnifiedStorage::get_convertor() const noexcept
{
     return convertor_;
}

} // namespace _16nar::resources
