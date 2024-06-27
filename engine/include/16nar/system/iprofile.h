/// @file
/// @brief Header file with IProfile interface definition.
#ifndef _16NAR_IPROFILE_H
#define _16NAR_IPROFILE_H

#include <16nar/16nardefs.h>

namespace _16nar
{

/// @brief Interface for application execution profile.
/// @details A profile specifies how the application is updating and rendering.
/// It determines threading model and performs synchronization of different threads.
class ENGINE_API IProfile
{
public:
     /// @brief Virtual default destructor.
     virtual ~IProfile() = default;

     /// @brief Run updating and rendering loops.
     virtual void run() = 0;

     /// @brief Finish execution of current scene.
     virtual void finish_scene() = 0;

     /// @brief Finish execution of current application.
     virtual void exit() = 0;

     /// @brief Set time per frame.
     /// @param[in] time_per_frame time per frame, in seconds.
     virtual void set_time_per_frame( float time_per_frame ) = 0;
};

} // namespace _16nar

#endif // #ifndef _16NAR_IPROFILE_H
