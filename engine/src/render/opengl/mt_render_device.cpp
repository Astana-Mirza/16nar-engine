#include <16nar/render/opengl/mt_render_device.h>

namespace _16nar::opengl
{

MtRenderDevice::MtRenderDevice( const ResourceManagerMap& managers ):
     StRenderDevice::StRenderDevice( managers ), render_queue_{}, frame_index_{ 0 }
{}


void MtRenderDevice::render( const RenderParams& params )
{
     size_t next_index = ( frame_index_ + 1 ) % _16nar_saved_frames;
     render_queue_[ next_index ].push( params );
}


void MtRenderDevice::process_render_queue()
{
     auto& queue = render_queue_[ frame_index_ ];
     while ( !queue.empty() )
     {
          auto& request = queue.front();
          do_render( request );
          queue.pop();
     }
}


void MtRenderDevice::end_frame()
{
     std::queue< RenderParams >{}.swap( render_queue_[ frame_index_ ] );
     frame_index_ = ( frame_index_ + 1 ) % _16nar_saved_frames;
}

} // namespace _16nar::opengl
