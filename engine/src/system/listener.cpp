#include <16nar/system/listener.h>

namespace _16nar
{

namespace listener
{

void set_global_volume( float volume )
{
     sf::Listener::setGlobalVolume( volume );
}


void set_position( const Vector2f& position )
{
     // make y axis like in graphics.
     sf::Listener::setPosition( position.x, -position.y, sf::Listener::getPosition().z );
}


void set_position( float x, float y )
{
     // make y axis like in graphics.
     sf::Listener::setPosition( x, -y, sf::Listener::getPosition().z );
}


void set_z( float value )
{
     auto pos = sf::Listener::getPosition();
     pos.z = value;
     sf::Listener::setPosition( pos );
}


float get_global_volume()
{
     return sf::Listener::getGlobalVolume();
}


Vector2f get_position()
{
     auto pos = sf::Listener::getPosition();
     return Vector2f{ pos.x, -pos.y };       // make y axis like in graphics.
}


float get_z()
{
     return sf::Listener::getPosition().z;
}

} // namespace listener

} // namespace _16nar
