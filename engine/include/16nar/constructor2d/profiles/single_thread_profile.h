/// @file
/// @brief Header file with SingleThreadProfile class definition.
#ifndef _16NAR_CONSTRUCTOR_2D_SINGLE_THREAD_PROFILE_H
#define _16NAR_CONSTRUCTOR_2D_SINGLE_THREAD_PROFILE_H

#include <16nar/system/iprofile.h>

#include <memory>
#include <string>

namespace _16nar
{

class IRenderApi;
class IRenderSystem;
class Window;

} // namespace _16nar


namespace _16nar::constructor2d
{

/// @brief Profile which runs the application update and render in single (main) thread.
class ENGINE_API SingleThreadProfile : public IProfile
{
public:
     /// @brief Constructor.
     SingleThreadProfile();

     /// @copydoc IProfile::run()
     virtual void run() override;

     /// @copydoc IProfile::switch_scene(std::string_view)
     virtual void switch_scene( std::string_view name ) override;

     /// @copydoc IProfile::exit()
     virtual void exit() override;

     /// @copydoc IProfile::set_time_per_frame(float)
     virtual void set_time_per_frame( float time_per_frame ) override;

private:
     /// @brief Type of current execution task.
     enum class TaskType
     {
          Running,       ///< Execution of a scene.
          Loading,       ///< Loading a scene.
          Exiting        ///< Exiting from application.
     };

     /// @brief Renders scene on the window.
     void render();

     /// @brief Run scene with the profile.
     void run_scene();

private:
     std::string scene_name_;                          ///< name of current scene.
     TaskType current_task_;                           ///< current task executed by profile.
     float time_per_frame_;                            ///< minimal time of one rendering frame.
     bool finish_;                                     ///< should the profile finish scene.
};

} // namespace _16nar::constructor2d

#endif // #ifndef _16NAR_CONSTRUCTOR_2D_SINGLE_THREAD_PROFILE_H
