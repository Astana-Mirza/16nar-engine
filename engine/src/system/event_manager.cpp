#include <16nar/system/event_manager.h>

namespace _16nar
{

void EventManager::handle_event( const Event& event )
{
     switch ( event.type )
     {
          case Event::KeyPressed:
               if ( key_actions_.find( event.key.code ) != key_actions_.cend() )
               {
                   pressed_actions_[ 0 ].insert( key_actions_[ event.key.code ] );
               }
               break;
          case Event::KeyReleased:
               if ( key_actions_.find( event.key.code ) != key_actions_.cend() )
               {
                    pressed_actions_[ 0 ].erase( key_actions_[ event.key.code ] );
               }
               break;
          case Event::JoystickButtonPressed:
               if ( joystick_actions_.find( event.joystickButton.button ) != joystick_actions_.cend() )
               {
                    pressed_actions_[ event.joystickButton.joystickId + 1 ].insert(
                         joystick_actions_[ event.joystickButton.button ] );
               }
               break;
          case Event::JoystickButtonReleased:
               if ( mouse_actions_.find( event.mouseButton.button ) != mouse_actions_.cend() )
               {
                    pressed_actions_[ event.joystickButton.joystickId + 1 ].erase(
                         joystick_actions_[ event.joystickButton.button ] );
               }
               break;
          case Event::MouseButtonPressed:
               if ( mouse_actions_.find( event.mouseButton.button ) != mouse_actions_.cend() )
               {
                    pressed_actions_[ 0 ].insert( mouse_actions_[ event.mouseButton.button ] );
               }
               events_[ event.type ].push_back( event );
               break;
          case Event::MouseButtonReleased:
               if ( mouse_actions_.find( event.mouseButton.button ) != mouse_actions_.cend() )
               {
                    pressed_actions_[ 0 ].erase( mouse_actions_[ event.mouseButton.button ] );
               }
               events_[ event.type ].push_back( event );
               break;
          default:
               events_[ event.type ].push_back( event );
               break;
     }
}


void EventManager::clear_events()
{
     for ( auto& [ type, list ] : events_ )
     {
          list.clear();
     }
}


void EventManager::add_pressable_key( Keyboard::Key key, int action )
{
     key_actions_[ key ] = action;
}


void EventManager::add_pressable_joystick_button( unsigned int button, int action )
{
     joystick_actions_[ button ] = action;
}


void EventManager::add_pressable_mouse_button( Mouse::Button button, int action )
{
     mouse_actions_[ button ] = action;
}


void EventManager::delete_pressable_key( Keyboard::Key key )
{
     key_actions_.erase( key );
}


void EventManager::delete_pressable_joystick_button( unsigned int button )
{
     joystick_actions_.erase( button );
}


void EventManager::delete_pressable_mouse_button( Mouse::Button button )
{
     mouse_actions_.erase( button );
}


const std::list< Event >& EventManager::get_events( Event::EventType type ) const
{
     return events_.at( type );
}


bool EventManager::is_pressed( int action, int joystick ) const
{
     if ( joystick < 1 )
     {
          bool keyboard = false;
          if ( pressed_actions_.find( 0 ) == pressed_actions_.cend() )
          {
               return false;
          }
          keyboard = pressed_actions_.at( 0 ).find( action ) != pressed_actions_.at( 0 ).cend();
          if ( joystick == 0 )
          {
               return keyboard;
          }
          for ( auto& [ j_num, pressed ] : pressed_actions_ )
          {
               if ( pressed.find( action ) != pressed.cend() )
               {
                    return true;
               }
          }
          return false;
     }
     if ( pressed_actions_.find( joystick ) == pressed_actions_.cend() )
     {
          return false;
     }
     const auto& pressed = pressed_actions_.at( joystick );
     return pressed.find( action ) != pressed.cend();
}

} // namespace _16nar
