/// @file
/// @brief File with RenderApi class definition.
#ifndef _16NAR_OPENGL_RENDER_API_H
#define _16NAR_OPENGL_RENDER_API_H

#include <16nar/16nardefs.h>

#include <16nar/abstract/resources/iresource_manager.h>
#include <16nar/render/render_defs.h>
#include <16nar/system/exceptions.h>

#include <unordered_map>
#include <memory>

namespace _16nar::opengl
{

/// @brief Class for rendering primitives with OpenGL.
class ENGINE_API RenderApi
{
public:
     /// @brief Constructor.
     RenderApi();

     /// @brief Load a resource with given parameters.
     /// @tparam T type of a resource.
     /// @param[in] params parameters for loading a resource.
     /// @return typed identifier of a resource.
     template < ResourceType T >
     TypedResource< T > load( const LoadParams< T >& params )
     {
          const auto iter = managers_.find( T );
          if ( iter == managers_.cend() )
          {
               throw ResourceException{ "wrong resource type" };
          }
          return iter->second->load( params );
     }

     /// @brief Unload a resource.
     /// @tparam T type of a resource.
     /// @param[in] resource typed identifier of a resource.
     template < ResourceType T >
     void unload( const TypedResource< T >& resource )
     {
          const auto iter = managers_.find( T );
          if ( iter == managers_.cend() )
          {
               throw ResourceException{ "wrong resource type" };
          }
          iter->second->unload( resource.id );
     }

private:
     using ManagerMap = std::unordered_map< ResourceType, std::unique_ptr< IResourceManager > >;

     RenderApi( const RenderApi& ) = delete;
     RenderApi& operator=( const RenderApi& ) = delete;

private:
     ManagerMap managers_;    ///< all resource managers.
};

} // namespace _16nar::opengl

#endif // _16NAR_OPENGL_RENDER_API_H
