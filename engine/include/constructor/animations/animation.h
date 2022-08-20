/// @file
/// @brief File with Animation class definition.
#ifndef _16NAR_ANIMATION_H
#define _16NAR_ANIMATION_H

#include <16nardefs.h>

#include <map>
#include <functional>

namespace _16nar
{

class Node;

/// @brief Class for controlling game animations.
class ENGINE_API Animation
{
public:
     using AnimationCallback = std::function< void( Node * ) >;
     using CallbackMap = std::map< float, AnimationCallback >;

     /// @brief Constructor.
     /// @param node node controlled by the animation.
     /// @param duration duration of the animation, in seconds.
     Animation( Node *node, float duration );
    
     Animation( Animation&& other ) = delete;
     Animation( const Animation& )  = delete;
     virtual ~Animation()           = default;

     /// @brief Advances animation execution, returns true if animation continued, false otherwise.
     /// @param delta time elapsed since last update, in seconds.
     virtual bool advance( float delta );

     /// @brief Adds callback function for specified time point.
     /// @param point time point for calling the function, in seconds since start of animation.
     /// @param callback function to be called at the time point.
     void add_callback( float point, AnimationCallback&& callback );

     /// @brief Starts the animation.
     void start();

     /// @brief Pauses or continues animation.
     /// @param paused will the animation be paused or not.
     void set_paused( bool paused );

     /// @brief Sets or disables replaying of the animation after it finishes.
     /// @param replay will the animation be replayed or not.
     void set_replay( bool replay );

     /// @brief Will the animation be replayed.
     bool get_replay() const;

     /// @brief Is the animation paused.
     bool get_paused() const;

     /// @brief Is the animation playing now.
     bool is_playing() const;

     /// @brief Gets duration of the animation.
     float get_duration() const;

     /// @brief Gets node contolled by this animation.
     Node *get_node() const;

private:
     CallbackMap callbacks_;                 ///< all callbacks with their time points.
     CallbackMap::const_iterator iter_;      ///< iterator in callbacks.
     Node *node_;                            ///< node controlled by the animation.
     float duration_;                        ///< duration of the animation, in seconds.
     float current_point_;                   ///< current time point, in seconds since start of animation.
     bool paused_;                           ///< is the animation paused.
     bool replay_;                           ///< will the animation be replayed.
};

} // namespace _16nar

#endif // #ifndef _16NAR_ANIMATION_H