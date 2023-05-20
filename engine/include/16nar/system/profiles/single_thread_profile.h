/// @file
/// @brief Header file with SingleThreadProfile class definition.
#ifndef _16NAR_SINGLE_THREAD_PROFILE_H
#define _16NAR_SINGLE_THREAD_PROFILE_H

#include <16nar/abstract/profile.h>

namespace _16nar
{

/// @brief Profile which runs game update and render in single (main) thread.
class ENGINE_API SingleThreadProfile : public Profile
{
public:
     /// @brief Constructor.
     /// @param[in] world world node for which the profile will work.
     /// @param[in] window pointer to main window of the game.
     SingleThreadProfile( WorldNode& world, const std::shared_ptr< RenderWindow >& window );

     /// @brief Run updating and rendering loop.
     virtual void run() override;

     /// @brief Renders one frame on the window.
     void render();
};

} // namespace _16nar

#endif // #ifndef _16NAR_PROFILE_H
