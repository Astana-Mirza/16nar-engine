/// @file
/// @brief Header file with TextureLoader class definition.
#ifndef _16NAR_OPENGL_TEXTURE_LOADER_H
#define _16NAR_OPENGL_TEXTURE_LOADER_H

#include <16nar/render/opengl/utils.h>

namespace _16nar::opengl
{

/// @brief Class for loading the texture.
class TextureLoader
{
public:
     /// @brief Parameters of texture loading.
     using LoadParamsType = LoadParams< ResourceType::Texture >;

     /// @brief Handler of texture.
    using HandlerType = Handler< ResourceType::Texture >;

     /// @brief Load texture to OpenGL and fill its handler.
     /// @param[in] params parameters of texture loading.
     /// @param[out] handler handler of the texture.
     /// @return true on success, false otherwise.
     static bool load( const ResourceManagerMap&, const LoadParamsType& params, HandlerType& handler );

     /// @brief Unload texture from OpenGL.
     /// @param[in] handler handler of the texture.
     /// @return true on success, false otherwise.
     static bool unload( const HandlerType& handler );
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_TEXTURE_LOADER_H
