#ifndef _16NAR_GAME_H
#define _16NAR_GAME_H

#include <memory>
#include <vector>
#include <string>

#include <render/quadrant.h>
#include <constructor/world_node.h>
#include <abstract/scene_reader.h>

namespace _16nar
{

class Node;

class Game
{
public:
     static Game& get_game();

     Game( const Game& )               = delete;
     void operator=( const Game& )     = delete;

     void run();
     void exit();

     void load_scene( const std::string& name );
     Node *get_node( const std::string& name ) const;
     void set_node_name( Node *node, const std::string& name );
     void delete_node_name( const std::string& name );

private:
     RenderWindow window_;
     std::string scene_name_;
     std::unordered_map< std::string, Node * > node_names_;
     WorldNode world_;
     std::unique_ptr< SceneReader > scene_reader_;
     Time time_per_frame_ = sf::seconds( 1.f / 60.f );
     SceneReader::SetupFuncPtr setup_func_ = nullptr;
     SceneReader::LoopFuncPtr loop_func_ = nullptr;

     Game() = default;
     void loop( float delta );
     void setup();
     void render();
     //void read_events();
};

} // namespace _16nar

#endif // #ifndef _16NAR_GAME_H
