/// @file
/// @brief Header file with Profile interface definition.
#ifndef _16NAR_PROFILE_H
#define _16NAR_PROFILE_H

#include <16nar/16nardefs.h>

#include <chrono>
#include <memory>

namespace _16nar
{

class WorldNode;
class RenderWindow;

/// @brief Interface for game running profile.
/// @details A profile specifies how the game is updating and rendering.
class ENGINE_API Profile
{
public:
     using Time = std::chrono::duration< float >;

     /// @brief Constructor.
     /// @param[in] world world node for which the profile will work.
     Profile( WorldNode& world ) : world_{ world } {}

     virtual ~Profile() = default;

     /// @brief Run updating and rendering loop.
     virtual void run() = 0;

     /// @brief Set finish flag.
     /// @param[in] finish finish flag value.
     inline void set_finish( bool finish ) { finish_ = finish; }

     /// @brief Get finish flag.
     /// @return finish flag value.
     inline bool get_finish() { return finish_; }

     /// @brief Set time per frame.
     /// @param[in] time_per_frame time per frame.
     inline void set_time_per_frame( Time time_per_frame ) { time_per_frame_ = time_per_frame; }

     /// @brief Get time per frame.
     /// @return time per frame.
     inline Time get_time_per_frame() { return time_per_frame_; }

protected:
     WorldNode& world_;                          ///< world node for which the profile works.

private:
     Time time_per_frame_ = Time{ 1.f / 60.f };  ///< minimal time of one rendering frame.
     bool finish_ = false;                       ///< should the profile finish its job.
};

} // namespace _16nar

#endif // #ifndef _16NAR_PROFILE_H
