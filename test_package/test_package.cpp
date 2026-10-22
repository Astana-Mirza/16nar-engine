#include <16nar/platform/logger/logger.h>
#include <16nar/platform/logger/terminal_log_writer.h>

#include <16nar/platform/memory/memory_manager.h>
#include <16nar/platform/memory/proxy_resource.h>
#include <16nar/platform/memory/memory_domain.h>

#include <16nar/core/profile/general/pipeline.h>
#include <16nar/core/assets/flatbuffers/flatbuffers_asset_file_processor.h>

#include <cstdlib>
#include <stdexcept>
#include <cassert>

using namespace _16nar;

namespace
{

void init_pipeline( profile::Pipeline& pipeline )
{
     auto& memory = pipeline.get_memory_manager();
     auto *utility_domain = memory.get_domain( strings::StaticName{ "utility" } );
     auto *bulk_domain = memory.get_domain( strings::StaticName{ "bulk" } );
     assert( utility_domain );
     assert( bulk_domain );

     /// @note user-provided memory domains may also be created here

     profile::Pipeline::InitData init_data{};
     init_data.file_processor = std::allocate_shared< assets::FlatBuffersAssetFileProcessor >(
          std::pmr::polymorphic_allocator< assets::FlatBuffersAssetFileProcessor >(
               &memory.get_root_domain()->get_resource() ),
          utility_domain->get_resource(), bulk_domain->get_resource() );

     pipeline.init( std::move( init_data ) );
}

} // anonymous namespace


int main( int argc, char *argv[] )
{
     auto& logger = logger::Logger::instance();
     logger.set_log_level( logger::Logger::Info );
     logger.add_writer( std::make_unique< logger::TerminalLogWriter >() );

     try
     {
          memory::MemoryDomain< memory::ProxyResource > root_domain{ std::pmr::new_delete_resource() };
          profile::Pipeline pipeline{ root_domain };

          init_pipeline( pipeline );

          LOG_16NAR_INFO( "Hello, 16nar!" );
     }
     catch ( const std::exception& ex )
     {
          LOG_16NAR_ERROR( "Exception: %s", ex.what() );
          return EXIT_FAILURE;
     }

     return EXIT_SUCCESS;
}
