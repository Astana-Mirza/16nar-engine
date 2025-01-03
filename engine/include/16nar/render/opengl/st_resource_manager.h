/// @file
/// @brief File with StResourceManager class definition.
#ifndef _16NAR_OPENGL_ST_RESOURCE_MANAGER_H
#define _16NAR_OPENGL_ST_RESOURCE_MANAGER_H

#include <16nar/render/render_defs.h>
#include <16nar/render/iresource_manager.h>

#include <unordered_map>

namespace _16nar::opengl
{

/// @brief Class for tracking resources for OpenGL, singlethread profile.
/// @tparam T type of resource loader.
template < typename T >
class StResourceManager : public IResourceManager
{
public:
     /// @brief Constructor.
     /// @param[in] managers resource managers for access to related resources.
     /// @throws std::bad_alloc.
     StResourceManager( const ResourceManagerMap& managers );

     // need to define these because of user-defined destructor

     /// @brief Move constructor.
     /// @param[in] other object to be moved from.
     StResourceManager( StResourceManager&& other );

     /// @brief Move assignment operator.
     /// @param[in] rhs object to be moved from.
     /// @return reference to current object.
     StResourceManager& operator=( StResourceManager&& rhs );

     /// @brief Destructor, requests unload of all loaded resources.
     ~StResourceManager();

     /// @copydoc IResourceManager::load(const std::any&)
     virtual ResID load( const std::any& params ) override;

     /// @copydoc IResourceManager::unload(ResId)
     virtual void unload( ResID id ) override;

     /// @copydoc IResourceManager::clear()
     virtual void clear() override;

     /// @copydoc IResourceManager::get_handler(ResID) const
     virtual std::any get_handler( ResID id ) const override;

private:
     /// @brief Handler of the resource.
     using HandlerType = typename T::HandlerType;

     /// @brief Loading parameters of the resource.
     using LoadParamsType = typename T::LoadParamsType;

     std::unordered_map< ResID, HandlerType > resources_;   ///< resources loaded in previous frames.
     const ResourceManagerMap& managers_;                   ///< resource managers for access to related resources.
     ResID next_ = 1;                                       ///< next ID to be assigned.
};

} // namespace _16nar::opengl

#include <16nar/render/opengl/st_resource_manager.inl>

#endif // #ifndef _16NAR_OPENGL_ST_RESOURCE_MANAGER_H
