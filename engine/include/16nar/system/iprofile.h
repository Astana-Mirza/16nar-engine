/// @file
/// @brief Header file with IProfile interface definition.
#ifndef _16NAR_IPROFILE_H
#define _16NAR_IPROFILE_H

#include <16nar/16nardefs.h>

#include <string_view>

namespace _16nar
{

/// @brief Interface for application execution profile.
/// @details A profile specifies how the application is updating and rendering.
/// It determines threading model and performs synchronization of different threads.
class ENGINE_API IProfile
{
public:
     /// @brief Default constructor.
     IProfile() = default;

     /// @brief Virtual default destructor.
     virtual ~IProfile() = default;

     /// @brief Run updating and rendering loops of selected scene.
     /// @details Scene will be loaded if it is not yet loaded.
     virtual void run() = 0;

     /// @brief Finish execution of current scene and run next one.
     /// @param[in] name name of the next scene to be executed.
     virtual void switch_scene( std::string_view name ) = 0;

     /// @brief Finish execution of current application.
     virtual void exit() = 0;

     /// @brief Set time per frame.
     /// @param[in] time_per_frame time per frame, in seconds.
     virtual void set_time_per_frame( float time_per_frame ) = 0;

private:
     IProfile( const IProfile& )                          = delete;
     IProfile& operator=( const IProfile& )               = delete;
};

} // namespace _16nar

#endif // #ifndef _16NAR_IPROFILE_H
