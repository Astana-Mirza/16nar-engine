#include <16nar/system/profiles/single_thread_profile.h>

#include <16nar/game.h>

namespace _16nar
{

SingleThreadProfile::SingleThreadProfile( WorldNode& world, const std::shared_ptr< RenderWindow >& window ):
     Profile{ world, window } {}


void SingleThreadProfile::run()
{
     std::chrono::steady_clock clock;
     world_.setup();
     auto prev_time = clock.now();
     while ( !get_finish() )
     {
          Time delta = clock.now() - prev_time;
          while ( delta >= get_time_per_frame() )
          {
               Game::get_game().read_events();
               world_.loop( delta.count() );
               prev_time = clock.now();
               delta = clock.now() - prev_time;
          }
          render();
     }
}


void SingleThreadProfile::render()
{
     window_->clear();
     world_.render( *window_ );
     window_->display();
}

} // namespace _16nar
