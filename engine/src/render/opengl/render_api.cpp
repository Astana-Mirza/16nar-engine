#include <16nar/render/opengl/render_api.h>

#include <16nar/render/opengl/st_resource_manager.h>
#include <16nar/render/opengl/mt_resource_manager.h>
#include <16nar/render/opengl/texture_loader.h>

namespace _16nar::opengl
{

RenderApi::RenderApi()
{
     managers_.emplace( ResourceType::Texture, std::make_unique< StResourceManager< TextureLoader > >() );
}


Resource RenderApi::load( ResourceType type, const std::any& params )
{
     const auto iter = managers_.find( type );
     if ( iter == managers_.cend() )
     {
          throw ResourceException{ "wrong resource type" };
     }
     return Resource{ type, iter->second->load( params ) };
}


void RenderApi::unload( const Resource& resource )
{
     const auto iter = managers_.find( resource.type );
     if ( iter == managers_.cend() )
     {
          throw ResourceException{ "wrong resource type" };
     }
     iter->second->unload( resource.id );
}


} // namespace _16nar::opengl
