#include <catch2/catch_test_macros.hpp>

#include <16nar/platform/memory/memory_manager.h>
#include <16nar/platform/memory/proxy_resource.h>
#include <16nar/platform/memory/stats_resource_wrapper.h>
#include <16nar/platform/memory/memory_domain.h>

#include <atomic>

TEST_CASE( "Memory manager functions", "[memory_memory_manager]" )
{
     _16nar::memory::MemoryDomain< _16nar::memory::ProxyResource > root_domain{
          std::pmr::new_delete_resource()
     };

     _16nar::memory::MemoryUsage usage{};
     usage = root_domain.get_usage();

     CHECK( usage.bytes_total == 0 );
     CHECK( usage.bytes_current == 0 );
     CHECK( usage.alloc_total == 0 );
     CHECK( usage.alloc_current == 0 );

     _16nar::memory::MemoryManager manager{ root_domain };

     CHECK( manager.get_domain( _16nar::strings::StaticName{ "not_exists" } ) == &root_domain );
     CHECK( manager.get_domain( _16nar::strings::StaticName{ "not_exists" }, true ) == nullptr );

     std::byte buffer[ 4096 ];
     auto *domain = manager.add_domain<
          _16nar::memory::MemoryDomain<
               _16nar::memory::StatsResourceWrapper<
                    std::pmr::monotonic_buffer_resource, std::uint64_t
               >
          >
     >( "monotonic", _16nar::strings::StaticName{}, buffer, sizeof( buffer ) );
     REQUIRE( domain != nullptr );
     CHECK( manager.get_domain( _16nar::strings::StaticName{ "monotonic" } ) == domain );

     usage = root_domain.get_usage();
     CHECK( usage.bytes_total != 0 );
     CHECK( usage.bytes_current != 0 );
     CHECK( usage.alloc_total != 0 );
     CHECK( usage.alloc_current != 0 );

     usage = domain->get_usage();
     CHECK( usage.bytes_total == 0 );
     CHECK( usage.bytes_current == 0 );
     CHECK( usage.alloc_total == 0 );
     CHECK( usage.alloc_current == 0 );

     auto *child_domain = manager.add_domain<
          _16nar::memory::MemoryDomain<
               _16nar::memory::StatsResourceWrapper<
                    std::pmr::unsynchronized_pool_resource, std::uint64_t
               >
          >
     >( "pool", _16nar::strings::StaticName{ "monotonic" } );
     REQUIRE( child_domain != nullptr );

     auto *ptr = child_domain->get_resource().allocate( 64 );
     REQUIRE( ptr != nullptr );
     usage = child_domain->get_usage();
     CHECK( usage.bytes_total == 64 );
     CHECK( usage.bytes_current == 64 );
     CHECK( usage.alloc_total == 1 );
     CHECK( usage.alloc_current == 1 );
     child_domain->get_resource().deallocate( ptr, 64 );
     usage = child_domain->get_usage();
     CHECK( usage.bytes_total == 64 );
     CHECK( usage.bytes_current == 0 );
     CHECK( usage.alloc_total == 1 );
     CHECK( usage.alloc_current == 0 );

     manager.reset_domain( _16nar::strings::StaticName{ "monotonic" } );
     usage = child_domain->get_usage();
     CHECK( usage.bytes_total == 0 );
     CHECK( usage.bytes_current == 0 );
     CHECK( usage.alloc_total == 0 );
     CHECK( usage.alloc_current == 0 );
     usage = domain->get_usage();
     CHECK( usage.bytes_total == 0 );
     CHECK( usage.bytes_current == 0 );
     CHECK( usage.alloc_total == 0 );
     CHECK( usage.alloc_current == 0 );

     auto *ptr2 = child_domain->get_resource().allocate( 64 );
     REQUIRE( ptr2 != nullptr );
     usage = child_domain->get_usage();
     CHECK( usage.bytes_total == 64 );
     CHECK( usage.bytes_current == 64 );
     CHECK( usage.alloc_total == 1 );
     CHECK( usage.alloc_current == 1 );
     child_domain->get_resource().deallocate( ptr2, 64 );
     usage = child_domain->get_usage();
     CHECK( usage.bytes_total == 64 );
     CHECK( usage.bytes_current == 0 );
     CHECK( usage.alloc_total == 1 );
     CHECK( usage.alloc_current == 0 );

     auto *again_domain = manager.add_domain<
          _16nar::memory::MemoryDomain<
               _16nar::memory::StatsResourceWrapper<
                    std::pmr::synchronized_pool_resource, std::atomic_uint64_t
               >
          >
     >( "pool", _16nar::strings::StaticName{} );
     CHECK( again_domain == nullptr );
}
