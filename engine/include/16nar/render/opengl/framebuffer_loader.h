/// @file
/// @brief Header file with FrameBufferLoader class definition.
#ifndef _16NAR_OPENGL_FRAMEBUFFER_LOADER_H
#define _16NAR_OPENGL_FRAMEBUFFER_LOADER_H

#include <16nar/render/render_defs.h>

#include <vector>

namespace _16nar::opengl
{

/// @brief Class for loading the framebuffer.
class FrameBufferLoader
{
public:
     /// @brief Parameters of framebuffer loading.
     using LoadParams = LoadParams< ResourceType::FrameBuffer >;

     /// @brief Handler of loaded framebuffer and its attachments.
     struct Handler
     {
          std::vector< unsigned int > attachments;     ///< attachments descriptors.
          unsigned int descriptor = 0;                 ///< framebuffer descriptor.
     };

     /// @brief Load framebuffer to OpenGL and fill its handler.
     /// @param[in] params parameters of framebuffer loading.
     /// @param[out] handler handler of the framebuffer.
     /// @return true on success, false otherwise.
     static bool load( const LoadParams& params, Handler& handler );

     /// @brief Unload framebuffer from OpenGL.
     /// @param[in] handler handler of the framebuffer.
     /// @return true on success, false otherwise.
     static bool unload( const Handler& handler );

};

} // namespace _16nar::opengl

#endif // #ifndef _16NAR_OPENGL_FRAMEBUFFER_LOADER_H
