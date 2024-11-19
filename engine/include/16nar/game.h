/// @file
/// @brief File with Game class definition.
#ifndef _16NAR_GAME_H
#define _16NAR_GAME_H

#include <memory>
#include <vector>
#include <string>

#include <16nar/16nardefs.h>
#include <16nar/system/package_manager.h>
#include <16nar/system/game_config.h>

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
     /// Terminates the application with error if game initialization fails during
     /// execution of this function. So it is recommended to initialize the game
     /// before creation of instance if one needs to check errors.
     /// @return Singleton game object.
     static Game& instance() noexcept;

     /// @brief Initialize engine, must be called before any function in engine.
     /// @details Does nothing if engine is already initialized. It is called in Game constructor.
     /// @param[in] config config used to initialize the engine.
     /// @throws std::bad_alloc, std::runtime_error.
     static void init( const GameConfig& config = {} );

     /// @brief Deinitialize engine, no engine functions must be called after this one.
     /// @details Does nothing if engine is already deinitialized. It is called in Game destructor.
     static void deinit() noexcept;

     /// @brief Destructor.
     ~Game();

     /// @brief Release all game resources.
     /// @details No game logic must be executed after this function.
     /// Not intended to be called by user.
     void finalize() noexcept;

     /// @brief Set render API of the game.
     /// @param[in] render_api render API object.
     void set_render_api( std::unique_ptr< IRenderApi >&& render_api ) noexcept;

     /// @brief Set window of the game.
     /// @param[in] window window object.
     void set_window( std::unique_ptr< Window >&& window ) noexcept;

     /// @brief Set reader of game scenes.
     /// @param[in] scene_reader Reader of game scenes.
     void set_scene_reader( std::unique_ptr< ISceneReader >&& scene_reader ) noexcept;

     /// @brief Set profile of game execution.
     /// @param[in] profile profile of game execution.
     void set_profile( std::unique_ptr< IProfile >&& profile ) noexcept;

     /// @brief Get common application configuration.
     /// @return common application configuration.
     const GameConfig& get_config() const noexcept;

     /// @brief Get render API of the game.
     /// @return render API of the game.
     IRenderApi& get_render_api() noexcept;

     /// @brief Get window of the game.
     /// @return window of the game.
     Window& get_window() noexcept;

     /// @brief Get reader of the game's scenes.
     /// @return reader of the game's scenes.
     ISceneReader& get_scene_reader() noexcept;

     /// @brief Get package manager of the game.
     /// @return package manager of the game.
     PackageManager& get_pkg_manager() noexcept;

private:
     Game( const Game& )               = delete;
     void operator=( const Game& )     = delete;

     /// @brief Constructor.
     Game() noexcept;

private:
     static bool initialized_;                         ///< engine initialization status.
     static GameConfig config_;                        ///< common application configuration.

     std::unique_ptr< PackageManager > pkg_manager_;   ///< package manager of the game.
     std::unique_ptr< IProfile > profile_;             ///< profile of selected architecture.
     std::unique_ptr< IRenderApi > render_api_;        ///< render API for drawing game objects.
     std::unique_ptr< ISceneReader > scene_reader_;    ///< reader of the game's scenes.
     std::unique_ptr< Window > window_;                ///< window of the game.
};


/// @brief Equivalent of @ref Game::instance()
inline Game& get_game() noexcept
{
     return Game::instance();
}

} // namespace _16nar

#endif // #ifndef _16NAR_GAME_H
