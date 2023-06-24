#include <16nar/system/profiles/single_thread_profile.h>

#include <16nar/game.h>
#include <16nar/abstract/render_device.h>
#include <16nar/render/frame_render_device.h>

namespace _16nar
{

SingleThreadProfile::SingleThreadProfile( WorldNode& world, const std::shared_ptr< RenderWindow >& window ):
     Profile{ world }, render_device_{ std::make_unique< FrameRenderDevice >( window ) } {}


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
     world_.start_render( *render_device_ );   // single thread, so wait for all to be drawn
     world_.finish_render( *render_device_ );
}

} // namespace _16nar
