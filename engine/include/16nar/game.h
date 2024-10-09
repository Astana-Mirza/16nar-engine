/// @file
/// @brief File with Game class definition.
#ifndef _16NAR_GAME_H
#define _16NAR_GAME_H

#include <memory>
#include <vector>
#include <string>

#include <16nar/16nardefs.h>
#include <16nar/system/package_manager.h>

namespace _16nar
{

class IProfile;
class IRenderApi;
class ISceneReader;
class Window;

/// @brief Singleton class for main game options management.
class ENGINE_API Game
{
public:
     /// @brief Get singleton game object.
     /// @details @ref get_game() is shorter version of this function.
     /// @return Singleton game object.
     static Game& instance();

     /// @brief Initialize engine, must be called before any function in engine.
     /// @detail Does nothing if engine is already initialized. It is called in Game constructor.
     static void init();

     /// @brief Deinitialize engine, no engine functions must be called after this one.
     /// @detail Does nothing if engine is already initialized. It is called in Game destructor.
     static void deinit();

     /// @brief Destructor.
     ~Game();

     /// @brief Get render API of the game.
     /// @return render API of the game.
     IRenderApi& get_render_api();

     /// @brief Get window of the game.
     /// @return window of the game.
     Window& get_window();

     /// @brief Get reader of the game's scenes.
     /// @return reader of the game's scenes.
     ISceneReader& get_scene_reader();

     /// @brief Get package manager of the game.
     /// @return package manager of the game.
     PackageManager& get_pkg_manager();

private:
     Game( const Game& )               = delete;
     void operator=( const Game& )     = delete;

     /// @brief Constructor.
     Game();

private:
     PackageManager pkg_manager_;                      ///< package manager of the game.
     std::unique_ptr< IProfile > profile_;             ///< profile of selected architecture.
     std::unique_ptr< IRenderApi > render_api_;        ///< render API for drawing game objects.
     std::unique_ptr< ISceneReader > scene_reader_;    ///< reader of the game's scenes.
     std::unique_ptr< Window > window_;                ///< window of the game.
};


/// @copydoc Game::instance()
inline Game& get_game()
{
     return Game::instance();
}

} // namespace _16nar

#endif // #ifndef _16NAR_GAME_H
