/// @file
/// @brief File with Game class definition.
#ifndef _16NAR_GAME_H
#define _16NAR_GAME_H

#include <memory>
#include <vector>
#include <string>
#include <chrono>

#include <render/quadrant.h>
#include <constructor/world_node.h>
#include <abstract/scene_reader.h>
#include <system/event_manager.h>

namespace _16nar
{

class Node;

/// @brief Singleton class for main game options management.
class ENGINE_API Game
{
public:
     using Time = std::chrono::duration< float >;

     /// @brief Method for getting single game object.
     static Game& get_game();

     /// @brief Method for getting current scene's world node.
     static WorldNode& get_world();

     Game( const Game& )               = delete;
     void operator=( const Game& )     = delete;

     /// @brief Runs the game loop within loaded scene.
     void run();

     /// @brief Ends current scene's game loop.
     void exit();

     /// @brief Loads scene with given name.
     /// @param name path to the scene.
     void load_scene( const std::string& name );

     /// @brief Sets window settings and opens it.
     /// @param title title of the window.
     /// @param width width of window in pixels.
     /// @param height height of window in pixels.
     /// @param flags flags for window settings.
     /// @param bits_per_pixel how many bits used to represent one pixel.
     void set_window( const std::string& title, unsigned width, unsigned height,
                      uint32_t flags = Style::Default,
                      unsigned bits_per_pixel = 32 );

     /// @brief Gets the event manager of the game.
     EventManager& get_event_manager();

private:
     /// @brief Constructor.
     Game();

     /// @brief Type of current game task.
     enum class TaskType
     {
          Running,       ///< Execution of a scene.
          Loading,       ///< Loading a scene.
          Exiting        ///< Exiting from game.
     };

     /// @brief Renders one frame on the window.
     void render();

     /// @brief Reads all input events for the window.
     void read_events();

     RenderWindow window_;                                      ///< main window of the game.
     WorldNode world_;                                          ///< node which manages scene states.
     std::unique_ptr< SceneReader > scene_reader_;              ///< pointer to reader of scenes.
     std::unique_ptr< EventManager > event_manager_;            ///< pointer to input event manager.
     Time time_per_frame_;                                      ///< minimal time of one rendering frame.
     TaskType current_task_;                                    ///< current game task.
};

} // namespace _16nar

#endif // #ifndef _16NAR_GAME_H
