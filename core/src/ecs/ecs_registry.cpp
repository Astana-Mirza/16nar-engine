#include <16nar/core/ecs/ecs_registry.h>

#include <16nar/platform/logger/logger.h>

namespace _16nar::ecs
{

EcsRegistry::EcsRegistry( std::pmr::memory_resource& resource, strings::NameTable& type_names )
     : type_names_{ type_names }
     , types_( &resource )
{}


bool EcsRegistry::register_component_type(
     strings::StaticName type, const TypeDescription& desc )
{
     if ( type.empty() )
     {
          LOG_16NAR_ERROR( "Cannot register ECS component type with empty name" );
          return false;
     }

     const auto type_name = type_names_.get_name( type, true );
     const auto result = types_.try_emplace( type, desc );
     if ( !result.second )
     {
          LOG_16NAR_WARNING( "Cannot register ECS component type '%.*s', already registered",
               static_cast< int >( type_name.size() ), type_name.data() );
          return false;
     }

     auto& added = result.first->second;
     if ( !added.size && ( added.construct || added.destruct ) )
     {
          LOG_16NAR_WARNING( "Lifetime functions for ECS flag component '%.*s' will be ignored",
               static_cast< int >( type_name.size() ), type_name.data() );
          added.construct = nullptr;
          added.destruct = nullptr;
     }

     return true;
}


TypeDescription *EcsRegistry::get_component_type( strings::StaticName type )
{
     auto iter = types_.find( type );
     return iter == ( types_.end() ) ? nullptr : &iter->second;
}

} // namespace _16nar::ecs
