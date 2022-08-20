/// @file
/// @brief File with EventManager class definition.
#ifndef _16NAR_EVENT_MANAGER_H
#define _16NAR_EVENT_MANAGER_H

#include <16nardefs.h>
#include <unordered_set>
#include <unordered_map>
#include <list>

namespace _16nar
{

class ENGINE_API EventManager
{
public:
     /// @brief Handles incoming event.
     /// @param event event to be handled.
     void handle_event( const Event& event );

     /// @brief Clears all saved events.
     void clear_events();

     /// @brief Sets the manager to listen to key press and release.
     /// @param key key triggering press and relese.
     /// @param action action referenced to this event.
     void add_pressable_key( Keyboard::Key key, int action );

     /// @brief Sets the manager to listen to joystick button press and release.
     /// @param button number of button of the joystick.
     /// @param action action referenced to this event.
     void add_pressable_joystick_button( unsigned int button, int action );

     /// @brief Sets the manager to listen to mouse button press and release.
     /// @param button number of button of the mouse.
     /// @param action action referenced to this event.
     void add_pressable_mouse_button( Mouse::Button button, int action );

     /// @brief Deletes a pressable key.
     /// @param key pressable key to be deleted.
     void delete_pressable_key( Keyboard::Key key );

     /// @brief Deletes a pressable joystick button.
     /// @param button pressable button to be deleted.
     void delete_pressable_joystick_button( unsigned int button );

     /// @brief Deletes a pressable mouse button.
     /// @param button pressable button to be deleted.
     void delete_pressable_mouse_button( Mouse::Button button );

     /// @brief Gets lsit of events of given type.
     /// @param type type of event.
     const std::list< Event >& get_events( Event::EventType type ) const;

     /// @brief Checks if the action is pressed.
     /// @param action checked action.
     /// @param joystick number of joystick (starts from 1, 0 is keyboard + mouse, -1 is any device).
     bool is_pressed( int action, int joystick = -1 ) const;

private:
     std::unordered_map< int, std::unordered_set< int > > pressed_actions_;      ///< actions pressed on all joysticks in current moment.
     std::unordered_map< Event::EventType, std::list< Event > > events_; ///< all events stored with their types.
     std::unordered_map< Keyboard::Key, int > key_actions_;              ///< mappings of keys to actions.
     std::unordered_map< unsigned int, int > joystick_actions_;          ///< mappings of joystick buttons to actions.
     std::unordered_map< Mouse::Button, int > mouse_actions_;            ///< mappings of mouse buttons to actions.
};

} // namespace _16nar

#endif // #ifndef _16NAR_EVENT_MANAGER_H