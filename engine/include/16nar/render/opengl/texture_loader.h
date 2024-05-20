/// @file
/// @brief Header file with TextureLoader class definition.
#ifndef _16NAR_OPENGL_TEXTURE_LOADER_H
#define _16NAR_OPENGL_TEXTURE_LOADER_H

#include <16nar/render/render_defs.h>
#include <16nar/abstract/resources/resource_loader.h>

#include <string_view>
#include <memory>

namespace _16nar
{

namespace opengl
{
class TextureLoader;
}


/// @brief Handler of texture.
template <>
struct Handler< opengl::TextureLoader >
{
     unsigned int descriptor; ///< texture descriptor.
};


namespace opengl
{

/// @brief Class for loading the texture.
class TextureLoader : public ResourceLoader< TextureLoader >
{
public:
     friend class ResourceLoader< TextureLoader >;
     using LoadParams = LoadParams< ResourceType::Texture >;
     using Handler = Handler< TextureLoader >;

     /// @brief Unload texture from OpenGL.
     /// @param[in] handler handler of the texture.
     /// @return true on success, false otherwise.
     static bool unload( Handler handler );

     /// @brief Constructor.
     /// @param[in] params parameters of loading the texture.
     TextureLoader( const LoadParams& params );

private:
     /// @copydoc ResourceLoader::load()
     bool load_impl() const;

     /// @copydoc ResourceLoader::get_handler()
     inline Handler get_handler_impl() const { return handler_; }

private:
     LoadParams params_;      ///< parameters of loading the texture.
     Handler handler_;        ///< texture handler.
};


/// @brief Load texture from file.
/// @details Data will always be freed after load.
/// @param[in] params parameters of loading the texture.
/// @param[in] file path to texture file.
TextureLoader load_texture( const TextureLoader::LoadParams& params, std::string_view file );

} // namespace opengl
} // namespace _16nar

#endif // #ifndef _16NAR_OPENGL_TEXTURE_LOADER_H
