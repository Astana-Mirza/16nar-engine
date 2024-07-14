#include <16nar/constructor/profiles/single_thread_profile.h>

#include <16nar/system/window.h>
#include <16nar/render/irender_system.h>

#include <chrono>

namespace _16nar::constructor
{

SingleThreadProfile::SingleThreadProfile( IRenderApi& api, Window& window ):
     window_{ window }, render_system_{}, current_task_{ TaskType::Loading },
     time_per_frame_{ 1.0f / 60.0f }, finish_{ false }
{}


void SingleThreadProfile::run()
{
     if ( current_task_ == TaskType::Loading )
     {
          // load scene
     }
     current_task_ = TaskType::Running;
     finish_ = false;
     while ( current_task_ != TaskType::Exiting )
     {
          if ( current_task_ == TaskType::Loading )
          {
               // load scene
               current_task_ = TaskType::Running;
               finish_ = false;
          }
          run_scene();
          if ( current_task_ == TaskType::Exiting )
          {
               render_system_.reset();
               window_.close();
          }
     }
}


void SingleThreadProfile::finish_scene()
{
     finish_ = true;
}


void SingleThreadProfile::exit()
{
     current_task_ = TaskType::Exiting;
     finish_scene();
}


void SingleThreadProfile::set_time_per_frame( float time_per_frame )
{
     time_per_frame_ = time_per_frame;
}


void SingleThreadProfile::render()
{
     // use render_system_ to render all world states
}


void SingleThreadProfile::run_scene()
{
     std::chrono::steady_clock clock;
     // setup scene
     auto prev_time = clock.now();
     while ( !finish_ )
     {
          std::chrono::duration< float > delta = clock.now() - prev_time;
          while ( delta.count() >= time_per_frame_ )
          {
               // read_events
               // update scene
               prev_time = clock.now();
               delta = clock.now() - prev_time;
          }
          render();
     }
}

} // namespace _16nar::constructor
