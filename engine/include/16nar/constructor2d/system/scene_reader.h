/// @file
/// @brief Definition of scene reader for constructor architecture.
#ifndef _16NAR_CONSTRUCTOR_2D_SCENE_READER_H
#define _16NAR_CONSTRUCTOR_2D_SCENE_READER_H

#include <16nar/system/iscene_reader.h>
#include <16nar/constructor2d/system/scene.h>

namespace _16nar::constructor2d
{

/// @brief Scene reader for constructor architecture.
class ENGINE_API SceneReader : public ISceneReader
{
public:
     /// @brief Constructor.
     SceneReader();

     /// @copydoc ISceneReader::set_file(const std::string&)
     void set_file( const std::string& filename ) override;

     /// @copydoc ISceneReader::create_scene()
     void create_scene() override;

     /// @copydoc ISceneReader::activate_scene()
     void activate_scene() override;

private:
     std::string scene_name_;                          ///< name of scene being loaded.
     std::unique_ptr< Scene > new_scene_;              ///< new scene being loaded.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_SCENE_READER_H
