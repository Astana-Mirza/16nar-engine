/// @file
/// @brief File with Game class definition.
#ifndef _16NAR_GAME_H
#define _16NAR_GAME_H

#include <memory>
#include <vector>
#include <string>

#include <16nar/render/quadrant.h>
#include <16nar/constructor/world_node.h>
#include <16nar/abstract/scene_reader.h>
#include <16nar/system/event_manager.h>
#include <16nar/abstract/profile.h>

namespace _16nar
{

class Node;

/// @brief Singleton class for main game options management.
class ENGINE_API Game
{
public:
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

     /// @brief Reads all input events for the window.
     void read_events();

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

     /// @brief Gets texture with given id, throws runtime_error if no such texture.
     /// @param id id of a texture.
     Texture& get_texture( ResourceID id );

     /// @brief Gets sound buffer with given id, throws runtime_error if no such sound buffer.
     /// @param id id of a sound buffer.
     SoundBuffer& get_sound( ResourceID id );

     /// @brief Gets font with given id, throws runtime_error if no such font.
     /// @param id id of a font.
     Font& get_font( ResourceID id );

     /// @brief Gets shader with given id, throws runtime_error if no such shader.
     /// @param id id of a shader.
     Shader& get_shader( ResourceID id );

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

     WorldNode world_;                                          ///< node which manages scene states.
     std::shared_ptr< RenderWindow > window_;                   ///< pointer to main window of the game.
     std::unique_ptr< Profile > profile_;                       ///< current running profile.
     std::unique_ptr< SceneReader > scene_reader_;              ///< pointer to reader of scenes.
     std::unique_ptr< EventManager > event_manager_;            ///< pointer to input event manager.
     TaskType current_task_;                                    ///< current game task.
};

} // namespace _16nar

#endif // #ifndef _16NAR_GAME_H
