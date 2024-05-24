/// @file
/// @brief File with StResourceManager class definition.
#ifndef _16NAR_OPENGL_ST_RESOURCE_MANAGER_H
#define _16NAR_OPENGL_ST_RESOURCE_MANAGER_H

#include <16nar/render/render_defs.h>
#include <16nar/abstract/resources/iresource_manager.h>

#include <unordered_map>

namespace _16nar::opengl
{

/// @brief Class for tracking resources for OpenGL, singlethread profile.
/// @tparam T type of resource loader.
template < typename T >
class StResourceManager : public IResourceManager
{
public:
     /// @brief Destructor, requests unload of all loaded resources.
     ~StResourceManager();

     /// @copydoc IResourceManager::load(const std::any&)
     /// @details Throws ResourceException if unable to load a resource.
     virtual ResID load( const std::any& params ) override;

     /// @copydoc IResourceManager::unload(ResId)
     /// @details Throws ResourceException if unable to unload a resource.
     virtual void unload( ResID id ) override;

     /// @copydoc IResourceManager::clear()
     virtual void clear() override;

private:
     std::unordered_map< ResID, typename T::Handler > resources_;    ///< resources loaded in previous frames.
     ResID next_ = 1;                                       ///< next ID to be assigned.
};

} // namespace _16nar::opengl

#include <16nar/render/opengl/st_resource_manager.inl>

#endif // #ifndef _16NAR_OPENGL_ST_RESOURCE_MANAGER_H