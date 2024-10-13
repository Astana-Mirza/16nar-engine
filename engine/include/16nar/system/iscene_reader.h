/// @file
/// @brief Header file with ISceneReader interface definition.
#ifndef _16NAR_ISCENE_READER_H
#define _16NAR_ISCENE_READER_H

#include <string_view>
#include <memory>

namespace _16nar
{

class IRenderSystem;

/// @brief Interface for reading scene from file.
class ISceneReader
{
public:
     /// @brief Virtual default destructor.
     virtual ~ISceneReader() = default;

     /// @brief Set file for reading a scene.
     /// @param[in] filename path to file of the scene.
     virtual void set_file( std::string_view filename ) = 0;

     /// @brief Create scene from file.
     virtual void create_scene() = 0;

     /// @brief Set current scene as active.
     virtual void activate_scene() = 0;
};

} // namespace _16nar

#endif // #ifndef _16NAR_ISCENE_READER_H
