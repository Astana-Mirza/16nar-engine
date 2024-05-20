/// @file
/// @brief Header file with FrameBufferLoader class definition.
#ifndef _16NAR_OPENGL_FRAMEBUFFER_LOADER_H
#define _16NAR_OPENGL_FRAMEBUFFER_LOADER_H

#include <16nar/render/render_defs.h>
#include <16nar/abstract/resources/resource_loader.h>

#include <vector>

namespace _16nar
{

namespace opengl
{
class FrameBufferLoader;
}


/// @brief Handler of loaded framebuffer and its attachments.
template <>
struct Handler< opengl::FrameBufferLoader >
{
     std::vector< unsigned int > attachments;     ///< attachments descriptors.
     unsigned int descriptor = 0;                 ///< framebuffer descriptor.
};


namespace opengl
{

/// @brief Class for loading the framebuffer.
class FrameBufferLoader : public ResourceLoader< FrameBufferLoader >
{
public:
     friend class ResourceLoader< FrameBufferLoader >;
     using LoadParams = LoadParams< ResourceType::FrameBuffer >;
     using Handler = Handler< FrameBufferLoader >;

     /// @brief Unload framebuffer from OpenGL.
     /// @param[in] handler handler of the framebuffer.
     /// @return true on success, false otherwise.
     static bool unload( const Handler& handler );

     /// @brief Constructor.
     /// @param[in] params parameters of loading the framebuffer.
     FrameBufferLoader( const LoadParams& params );

private:
     /// @copydoc ResourceLoader::load()
     bool load_impl() const;

     /// @copydoc ResourceLoader::get_handler()
     inline Handler get_handler_impl() const { return handler_; }

private:
     LoadParams params_; ///< parameters of loading the framebuffer.
     Handler handler_;   ///< handler of the framebuffer.
};

} // namespace opengl
} // namespace _16nar

#endif // #ifndef _16NAR_OPENGL_FRAMEBUFFER_LOADER_H
