#include <16nar/render/opengl/mt_render_device.h>


namespace _16nar::opengl
{

#define _16NAR_ENQUEUE_COMMAND( _cmd, ... )  \
     size_t next_index = ( frame_index_ + 1 ) % _16nar_saved_frames; \
     render_queue_[ next_index ].push( std::bind( &StRenderDevice::_cmd, this, __VA_ARGS__ ) );


MtRenderDevice::MtRenderDevice( const ResourceManagerMap& managers ):
     StRenderDevice::StRenderDevice( managers ), render_queue_{}, frame_index_{ 0 }
{}


void MtRenderDevice::render( const RenderParams& params )
{
     _16NAR_ENQUEUE_COMMAND( render, params );
}


void MtRenderDevice::set_viewport( const IntRect& rect )
{
     _16NAR_ENQUEUE_COMMAND( set_viewport, rect );
}


void MtRenderDevice::set_depth_test_state( bool enable )
{
     _16NAR_ENQUEUE_COMMAND( set_depth_test_state, enable );
}


void MtRenderDevice::bind_shader( const Shader& shader )
{
     _16NAR_ENQUEUE_COMMAND( bind_shader, shader );
}


void MtRenderDevice::set_shader_params( const ShaderSetupFunction& setup )
{
     _16NAR_ENQUEUE_COMMAND( set_shader_params, setup );
}


void MtRenderDevice::bind_framebuffer( const FrameBuffer& framebuffer )
{
     _16NAR_ENQUEUE_COMMAND( bind_framebuffer, framebuffer );
}


void MtRenderDevice::clear( bool color, bool depth, bool stencil )
{
     _16NAR_ENQUEUE_COMMAND( clear, color, depth, stencil );
}


void MtRenderDevice::process_render_queue()
{
     auto& queue = render_queue_[ frame_index_ ];
     while ( !queue.empty() )
     {
          queue.front()();
          queue.pop();
     }
}


void MtRenderDevice::end_frame()
{
     std::queue< Request >{}.swap( render_queue_[ frame_index_ ] );
     frame_index_ = ( frame_index_ + 1 ) % _16nar_saved_frames;
}

} // namespace _16nar::opengl
