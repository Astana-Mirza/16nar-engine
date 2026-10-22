#include <16nar/core/profile/general/pipeline.h>

#include <16nar/platform/memory/memory_domain.h>
#include <16nar/platform/memory/proxy_resource.h>
#include <16nar/platform/memory/stats_resource_wrapper.h>
#include <16nar/platform/logger/logger.h>
#include <16nar/platform/strings/name_manager.h>

#include <16nar/core/resources/unified_storage.h>
#include <16nar/core/ecs/ecs_registry.h>
#include <16nar/core/ecs/ecs_storage.h>
#include <16nar/core/loader/service_locator.h>
#include <16nar/core/config/config_manager.h>

#include <utility>
#include <stdexcept>
#include <cassert>

namespace _16nar::profile
{
namespace
{

template < typename T, typename... Args >
T *allocate_object( std::pmr::memory_resource& resource, Args&&... args )
{
     std::pmr::polymorphic_allocator< T > alloc{ &resource };
     T *obj = alloc.allocate( 1 );
     try
     {
          alloc.construct( obj, std::forward< Args >( args )... );
     }
     catch( ... )
     {
          alloc.deallocate( obj, 1 );
          throw;
     }
     return obj;
}


template < typename T >
void deallocate_object( std::pmr::memory_resource& resource, T *obj )
{
     if ( !obj )
     {
          return;
     }

     std::pmr::polymorphic_allocator< T > alloc{ &resource };
     alloc.destroy( obj );
     alloc.deallocate( obj, 1 );
}

} // anonymous


struct Pipeline::Impl
{
     /// @brief Constructor.
     /// @param[in] memory_manager memory domain manager. 
     Impl( memory::MemoryManager& memory_manager );

     /// @brief Destructor.
     ~Impl();

     /// @brief Initialize the pipeline using custom parameters.
     /// @param[in] data data for pipeline initialization.
     void init( Pipeline::InitData&& data );

     Impl( const Impl& ) = delete;
     Impl& operator=( const Impl& ) = delete;

     /// @brief Map of resource managers.
     using ResourceManagers = std::pmr::unordered_map<
          strings::StaticName, resources::IResourceManagerPtr >;

     memory::MemoryManager& memory;          ///< memory domain manager.
     ResourceManagers resources;             ///< resource managers.
     strings::NameManager *names;            ///< name table manager.
     resources::UnifiedStorage *storage;     ///< storage of application data.
     ecs::EcsRegistry *ecs_registry;         ///< registry of ECS types.
     ecs::EcsStorage *ecs_storage;           ///< storage of ECS data.
     loader::ServiceLocator *locator;        ///< service locator for plugins' private interfaces.
     config::ConfigManager *config;          ///< configuration manager.
     bool initialized;                       ///< true if pipeline was initialized, false otherwise.
};


Pipeline::Pipeline( memory::IMemoryDomain& root_domain )
     : memory_{ root_domain }
     , impl_{}
{
     impl_ = allocate_object< Impl >( memory_.get_root_domain()->get_resource(), memory_ );
}


Pipeline::~Pipeline()
{
     deallocate_object( memory_.get_root_domain()->get_resource(), impl_ );
}


void Pipeline::init( Pipeline::InitData&& data )
{
     impl_->init( std::move( data ) );
}


memory::MemoryManager& Pipeline::get_memory_manager() noexcept
{
     return memory_;
}


bool Pipeline::is_initialized() const noexcept
{
     return impl_->initialized;
}


strings::NameManager& Pipeline::get_name_manager() noexcept
{
     assert( is_initialized() );
     return *impl_->names;
}


resources::IResourceManager *Pipeline::get_resource_manager( strings::StaticName type ) noexcept
{
     assert( is_initialized() );
     const auto iter = impl_->resources.find( type );
     return ( iter == impl_->resources.end() ) ? nullptr : iter->second.get();
}


bool Pipeline::add_resource_manager( std::string_view type, resources::IResourceManagerPtr manager )
{
     assert( is_initialized() );
     if ( type.empty() )
     {
          LOG_16NAR_ERROR( "Cannot add resource with empty type name" );
          return false;
     }
     if ( !manager )
     {
          LOG_16NAR_ERROR( "Cannot add resource type '%.*s' with null manager",
               static_cast< int >( type.size() ), type.data() );
          return false;
     }

     auto *name_table = impl_->names->get_table( "resource_type" );
     assert( name_table );

     const auto type_name = name_table->add_name( type );
     if ( type_name.empty() )
     {
          LOG_16NAR_ERROR( "Cannot add resource type name '%.*s'",
               static_cast< int >( type.size() ), type.data() );
          return false;
     }
     return impl_->resources.try_emplace( type_name, std::move( manager ) ).second;
}


resources::UnifiedStorage& Pipeline::get_unified_storage() noexcept
{
     assert( is_initialized() );
     return *impl_->storage;
}


ecs::EcsRegistry& Pipeline::get_ecs_registry() noexcept
{
     assert( is_initialized() );
     return *impl_->ecs_registry;
}


ecs::EcsStorage& Pipeline::get_current_ecs_storage() noexcept
{
     assert( is_initialized() );
     return *impl_->ecs_storage;
}


loader::ServiceLocator& Pipeline::get_service_locator() noexcept
{
     assert( is_initialized() );
     return *impl_->locator;
}


config::ConfigManager& Pipeline::get_config_manager() noexcept
{
     assert( is_initialized() );
     return *impl_->config;
}


Pipeline::Impl::Impl( memory::MemoryManager& memory_manager )
     : memory{ memory_manager }
     , resources{}
     , names{}
     , storage{}
     , ecs_registry{}
     , ecs_storage{}
     , locator{}
     , config{}
{
     // ensure that the logger is created
     ( void ) logger::Logger::instance();
     LOG_16NAR_INFO( "Creating execution pipeline" );

     auto check_domain = []( memory::IMemoryDomain *domain, std::string_view name ) -> void
     {
          if ( !domain )
          {
               throw std::runtime_error{ "cannot create memory domain '" + std::string{ name } + "'" };
          }
     };

     auto *strings_domain = memory.add_domain<
          memory::MemoryDomain< memory::StatsResourceWrapper<
                    std::pmr::unsynchronized_pool_resource, std::uint64_t > > >(
                         "strings", strings::StaticName{} );
     check_domain( strings_domain, "strings" );

     auto *utility_domain = memory.add_domain<
          memory::MemoryDomain< memory::StatsResourceWrapper<
                    std::pmr::unsynchronized_pool_resource, std::uint64_t > > >(
                         "utility", strings::StaticName{} );
     check_domain( utility_domain, "utility" );

     // de facto use root domain, but have own counters
     auto *bulk_domain = memory.add_domain<
          memory::MemoryDomain< memory::ProxyResource > >( "bulk", strings::StaticName{} );
     check_domain( bulk_domain, "bulk" );

     auto *ecs_utility_domain = memory.add_domain<
          memory::MemoryDomain< memory::StatsResourceWrapper<
                    std::pmr::unsynchronized_pool_resource, std::uint64_t > > >(
                         "ecs_utility", strings::StaticName{} );
     check_domain( ecs_utility_domain, "ecs_utility" );

     auto *ecs_bulk_domain = memory.add_domain<
          memory::MemoryDomain< memory::StatsResourceWrapper<
                    std::pmr::unsynchronized_pool_resource, std::uint64_t > > >(
                         "ecs_bulk", strings::StaticName{} );
     check_domain( ecs_bulk_domain, "ecs_bulk" );

     auto *config_domain = memory.add_domain<
          memory::MemoryDomain< memory::StatsResourceWrapper<
                    std::pmr::unsynchronized_pool_resource, std::uint64_t > > >(
                         "config", strings::StaticName{} );
     check_domain( config_domain, "config" );

     LOG_16NAR_INFO( "Execution pipeline was created successfully" );
}


Pipeline::Impl::~Impl()
{
     auto& resource = memory.get_root_domain()->get_resource();
     deallocate_object( resource, names );
     deallocate_object( resource, storage );
     deallocate_object( resource, ecs_registry );
     deallocate_object( resource, ecs_storage );
     deallocate_object( resource, locator );
     deallocate_object( resource, config );
}


void Pipeline::Impl::init( Pipeline::InitData&& data )
{
     assert( !initialized );

     auto check_table = []( strings::NameTable *table, std::string_view name ) -> void
     {
          if ( !table )
          {
               throw std::runtime_error{ "cannot create name table '" + std::string{ name } + "'" };
          }
     };

     auto *root_domain        = memory.get_root_domain();
     auto *strings_domain     = memory.get_domain( strings::StaticName{ "strings" }, true );
     auto *utility_domain     = memory.get_domain( strings::StaticName{ "utility" }, true );
     auto *bulk_domain        = memory.get_domain( strings::StaticName{ "bulk" }, true );
     auto *ecs_utility_domain = memory.get_domain( strings::StaticName{ "ecs_utility" }, true );
     auto *ecs_bulk_domain    = memory.get_domain( strings::StaticName{ "ecs_bulk" }, true );
     auto *config_domain      = memory.get_domain( strings::StaticName{ "config" }, true );

     assert( root_domain );
     assert( strings_domain );
     assert( utility_domain );
     assert( bulk_domain );
     assert( ecs_utility_domain );
     assert( ecs_bulk_domain );
     assert( config_domain );

     names = allocate_object< strings::NameManager >( root_domain->get_resource(),
          strings_domain->get_resource() );

     auto *path = names->add_table( "path" );
     check_table( path, "path" );
     auto *ecs_type = names->add_table( "ecs_type" );
     check_table( ecs_type, "ecs_type" );
     auto *resource_type = names->add_table( "resource_type" );
     check_table( resource_type, "resource_type" );
     auto *config_name = names->add_table( "config_name" );
     check_table( config_name, "config_name" );

     resources = ResourceManagers( &utility_domain->get_resource() );
     storage = allocate_object< resources::UnifiedStorage >( root_domain->get_resource(),
          utility_domain->get_resource(), bulk_domain->get_resource(),
          *path, std::move( data.file_processor ) );
     ecs_registry = allocate_object< ecs::EcsRegistry >( root_domain->get_resource(),
          utility_domain->get_resource(), *ecs_type );
     ecs_storage = allocate_object< ecs::EcsStorage >( root_domain->get_resource(),
          ecs_utility_domain->get_resource(), ecs_bulk_domain->get_resource(),
          *ecs_type, *ecs_registry );
     locator = allocate_object< loader::ServiceLocator >( root_domain->get_resource(),
          utility_domain->get_resource() );
     config = allocate_object< config::ConfigManager >( root_domain->get_resource(),
          config_domain->get_resource(), *config_name );

     initialized = true;
}

} // namespace _16nar::profile
