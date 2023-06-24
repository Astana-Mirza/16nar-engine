#include <16nar/render/frame_render_device.h>

namespace _16nar
{

FrameRenderDevice::FrameRenderDevice( const std::shared_ptr< RenderWindow >& window ):
     frames_{}, window_{ window } {}


void FrameRenderDevice::add_data( const DeviceData& data )
{
     if ( frames_.empty() )
     {
          frames_.push( std::vector< DeviceData >{ data } );
     }
     else
     {
          frames_.back().push_back( data );
     }
}


void FrameRenderDevice::end_frame()
{
     if ( frames_.empty() || frames_.front().empty() )
     {
          return;
     }
     frames_.push( std::vector< DeviceData >{} );
}


void FrameRenderDevice::flush()
{
     window_->clear();
     for ( const auto& data : frames_.front() )
     {
          switch ( data.type )
          {
               case DeviceDataType::View:
                    window_->setView( data.view );
               break;
               case DeviceDataType::RenderData:
                    //window_->draw( data.render_data );
               break;
               default:
               break;
          }
     }
     window_->display();
     frames_.pop();
}

} // namespace _16nar
