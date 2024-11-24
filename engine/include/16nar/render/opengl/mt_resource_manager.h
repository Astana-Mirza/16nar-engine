/// @file
/// @brief File with MtResourceManager template class definition.
#ifndef _16NAR_OPENGL_MT_RESOURCE_MANAGER_H
#define _16NAR_OPENGL_MT_RESOURCE_MANAGER_H

#include <16nar/render/render_defs.h>
#include <16nar/render/iresource_manager.h>

#include <unordered_map>
#include <array>
#include <queue>
#include <atomic>

namespace _16nar::opengl
{

/// @brief Class tracks resources for OpenGL, multithread profile.
/// @details For OpenGL, all render functions must be called from
/// one thread, so a resource will be fully initialized after
/// RenderDevice render call. Before that moment, the resource
/// manager will keep its data without loading it into GPU.
///
/// MtResourceManager keeps IDs of resources and some
/// information about them.
///
/// All functions, except load and unload must be called from
/// render thread.
/// @tparam T type of resource.
template < typename T >
class MtResourceManager : public IResourceManager
{
public:
     /// @brief Constructor.
     /// @param[in] managers resource managers for access to related resources.
     MtResourceManager( const ResourceManagerMap& managers );

     /// @brief Destructor, requests unload of all loaded resources.
     ~MtResourceManager();

     /// @brief Implementation of @ref IResourceManager::load(const std::any&)
     /// @details The resource will be loaded during render processing, so
     /// due to game loop the load is not made immediately,
     /// but request is saved and its ID is given.
     /// @param[in] params resource load params.
     /// @throws ResourceException, ExceededIdException.
     /// @return resource identifier.
     virtual ResID load( const std::any& params ) override;

     /// @brief Implementation of @ref IResourceManager::unload(ResId)
     /// @details The resource will be loaded during render processing, so
     /// due to game loop the load is not made immediately, but request is saved.
     virtual void unload( ResID id ) override;

     /// @copydoc IResourceManager::clear()
     virtual void clear() override;

     /// @copydoc IResourceManager::get_handler(ResID) const
     virtual std::any get_handler( ResID id ) const override;

     /// @brief Process all requests in load queue.
     /// @throws ResourceException, ExceededIdException.
     virtual void process_load_queue() override;

     /// @brief Process all requests in unload queue.
     /// @throws ResourceException.
     virtual void process_unload_queue() override;

     /// @copydoc IResourceManager::end_frame()
     virtual void end_frame() override;

private:
     /// @brief Handler of the resource.
     using HandlerType = typename T::HandlerType;

     /// @brief Loading parameters of the resource.
     using LoadParamsType = typename T::LoadParamsType;

     /// @brief Element of load queue.
     using Request = std::pair< ResID, LoadParamsType >;

     std::unordered_map< ResID, HandlerType > resources_;                  ///< resources loaded in previous frames.
     std::array< std::queue< Request >, _16nar_saved_frames > load_queue_; ///< requests to load resources.
     std::array< std::queue< ResID >, _16nar_saved_frames > unload_queue_; ///< requests to unload resources.
     const ResourceManagerMap& managers_;                                  ///< resource managers for access to related resources.
     ResID next_;                                                          ///< next ID to be assigned.
     std::atomic_size_t frame_index_;                                      ///< index of the frame being rendered.
};

} // namespace _16nar::opengl

#include <16nar/render/opengl/mt_resource_manager.inl>

#endif // #ifndef _16NAR_OPENGL_MT_RESOURCE_MANAGER_H
