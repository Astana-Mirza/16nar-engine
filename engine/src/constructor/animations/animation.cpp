#include <constructor/animations/animation.h>

#include <constructor/abstract/node.h>

namespace _16nar
{

Animation::Animation( Node *node, float duration ):
     iter_{ callbacks_.cend() }, node_{ node }, duration_{ duration },
     current_point_{ 0 }, paused_{ false }, replay_{ false } {}


bool Animation::advance( float delta )
{
     if ( paused_ )
     {
          return false;
     }
     if ( !is_playing() )
     {
          if ( replay_ )
          {
               start();
          }
          else
          {
               return false;
          }
     }
     current_point_ += delta;
     if ( iter_ != callbacks_.cend() && iter_->first <= current_point_ )
     {
          iter_->second( node_ );
          ++iter_;
     }
     return true;
}


void Animation::add_callback( float point, AnimationCallback&& callback )
{
     callbacks_[ point ] = std::move( callback );
}


void Animation::start()
{
     iter_ = callbacks_.begin();
     current_point_ = 0;
}


void Animation::set_paused( bool paused )
{
     paused_ = paused;
}


void Animation::set_replay( bool replay )
{
     replay_ = replay;
}


bool Animation::get_replay() const
{
     return replay_;
}


bool Animation::get_paused() const
{
     return paused_;
}


bool Animation::is_playing() const
{
     return current_point_ < duration_;
}


float Animation::get_duration() const
{
     return duration_;
}


Node *Animation::get_node() const
{
     return node_;
}

} // namespace _16nar