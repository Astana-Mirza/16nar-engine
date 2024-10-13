/// @file
/// @brief Header file with ShaderLoader class definition.
#ifndef _16NAR_OPENGL_SHADER_LOADER_H
#define _16NAR_OPENGL_SHADER_LOADER_H

#include <16nar/render/opengl/utils.h>

namespace _16nar::opengl
{

/// @brief Class for loading the SPIR-V shader program.
class ShaderLoader
{
public:
     /// @brief Parameters of shader loading.
     using LoadParamsType = LoadParams< ResourceType::Shader >;

     /// @brief Handler of shader program.
     using HandlerType = Handler< ResourceType::Shader >;

     /// @brief Load shader to OpenGL and fill its handler.
     /// @param[in] params parameters of shader loading.
     /// @param[out] handler handler of the shader.
     /// @return true on success, false otherwise.
     static bool load( const ResourceManagerMap&, const LoadParamsType& params, HandlerType& handler );

     /// @brief Unload shader from OpenGL.
     /// @param[in] handler handler of the shader.
     /// @return true on success, false otherwise.
     static bool unload( const HandlerType& handler );
};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_SHADER_LOADER_H
