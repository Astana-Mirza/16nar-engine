/// @file
/// @brief Header file with CubemapLoader class definition.
#ifndef _16NAR_OPENGL_CUBEMAP_LOADER_H
#define _16NAR_OPENGL_CUBEMAP_LOADER_H

#include <16nar/render/opengl/utils.h>

#include <vector>

namespace _16nar::opengl
{

/// @brief Class for loading the cubemap.
class CubemapLoader
{
public:
     /// @brief Parameters of cubemap loading.
     using LoadParamsType = LoadParams< ResourceType::Cubemap >;

     /// @brief Handler of loaded cubemap and its attachments.
     using HandlerType = Handler< ResourceType::Cubemap >;

     /// @brief Load cubemap to OpenGL and fill its handler.
     /// @param[in] params parameters of cubemap loading.
     /// @param[out] handler handler of the cubemap.
     /// @return true on success, false otherwise.
     static bool load( const ResourceManagerMap&, const LoadParamsType& params, HandlerType& handler );

     /// @brief Unload cubemap from OpenGL.
     /// @param[in] handler handler of the cubemap.
     /// @return true on success, false otherwise.
     static bool unload( const HandlerType& handler );

};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_CUBEMAP_LOADER_H
