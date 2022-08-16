#include <constructor/sound_node.h>

namespace _16nar
{

#ifdef USE_SFML

SoundNode::SoundNode( const SoundBuffer& buffer ) : sound_{ buffer } {}


void SoundNode::set_position( float x, float y )
{
     Node2D::set_position( x, y );
     auto global = get_global_transform_matr().transformPoint( { 0, 0 } );
     sound_.setPosition( global.x, -global.y, get_z() );          // make y axis like in graphics.
}


void SoundNode::set_position( const Vector2f& position )
{
     Node2D::set_position( position );
     auto global = get_global_transform_matr().transformPoint( { 0, 0 } );
     sound_.setPosition( global.x, -global.y, get_z() );          // make y axis like in graphics.
}


void SoundNode::move( float offset_x, float offset_y )
{
     Node2D::move( offset_x, offset_y );
     auto global = get_global_transform_matr().transformPoint( { 0, 0 } );
     sound_.setPosition( global.x, -global.y, get_z() );          // make y axis like in graphics.
}


void SoundNode::move( const Vector2f& offset )
{
     Node2D::move( offset );
     auto global = get_global_transform_matr().transformPoint( { 0, 0 } );
     sound_.setPosition( global.x, -global.y, get_z() );          // make y axis like in graphics.
}


void SoundNode::set_z( float value )
{
     auto pos = sound_.getPosition();
     pos.z = value;
     sound_.setPosition( pos );
}


void SoundNode::set_buffer( const SoundBuffer& buffer )
{
     sound_.setBuffer( buffer );
}


void SoundNode::set_loop( bool loop )
{
     sound_.setLoop( loop );
}


void SoundNode::set_offset( float offset )
{
     sound_.setPlayingOffset( sf::seconds( offset ) );
}


void SoundNode::set_volume( float volume )
{
     sound_.setVolume( volume );
}


void SoundNode::set_pitch( float pitch )
{
     sound_.setPitch( pitch );
}


void SoundNode::set_relative_to_listener( bool relative )
{
     sound_.setRelativeToListener( relative );
}


void SoundNode::set_min_distance( float distance )
{
     sound_.setMinDistance( distance );
}


void SoundNode::set_attenuation( float attenuation )
{
     sound_.setAttenuation( attenuation );
}


void SoundNode::play()
{
     sound_.play();
}


void SoundNode::pause()
{
     sound_.pause();
}


void SoundNode::stop()
{
     sound_.stop();
}


float SoundNode::get_z() const
{
     auto pos = sound_.getPosition();
     return pos.z;
}


const SoundBuffer *SoundNode::get_buffer() const
{
     return sound_.getBuffer();
}


bool SoundNode::get_loop() const
{
     return sound_.getLoop();
}


float SoundNode::get_offset() const
{
     return sound_.getPlayingOffset().asSeconds();
}


float SoundNode::get_volume() const
{
     return sound_.getVolume();
}


float SoundNode::get_pitch() const
{
     return sound_.getPitch();
}


bool SoundNode::get_relative_to_listener() const
{
     return sound_.isRelativeToListener();
}


float SoundNode::get_min_distance() const
{
     return sound_.getMinDistance();
}


float SoundNode::get_attenuation() const
{
     return sound_.getAttenuation();
}

#endif // #ifdef USE_SFML

} // namespace _16nar