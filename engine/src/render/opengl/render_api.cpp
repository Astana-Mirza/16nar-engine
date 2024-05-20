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

} // namespace _16nar::opengl
