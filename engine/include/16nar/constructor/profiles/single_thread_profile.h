/// @file
/// @brief Header file with SingleThreadProfile2D class definition.
#ifndef _16NAR_SINGLE_THREAD_PROFILE_H
#define _16NAR_SINGLE_THREAD_PROFILE_H

#include <16nar/system/iprofile.h>

#include <memory>

namespace _16nar
{

class IRenderApi;
class IRenderSystem;
class Window;

}


namespace _16nar::constructor
{

/// @brief Profile which runs the application update and render in single (main) thread.
class ENGINE_API SingleThreadProfile : public IProfile
{
public:
     /// @brief Constructor.
     /// @param[in] api API for rendering selected objects.
     /// @param[in] window main window of the application.
     SingleThreadProfile( IRenderApi& api, Window& window );

     /// @copydoc IProfile::run()
     virtual void run() override;

     /// @copydoc IProfile::finish_scene()
     virtual void finish_scene() override;

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
     Window& window_;                                  ///< window of the application.
     std::unique_ptr< IRenderSystem > render_system_;  ///< render system for drawing scene.
     TaskType current_task_;                           ///< current task executed by profile.
     float time_per_frame_;                            ///< minimal time of one rendering frame.
     bool finish_;                                     ///< should the profile finish scene.
};

} // namespace _16nar::constructor

#endif // #ifndef _16NAR_SINGLE_THREAD_PROFILE_H
