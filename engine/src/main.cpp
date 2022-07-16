#include <game.h>

#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE

int main()
{
     _16nar::Game::get_game().load_scene( "main_menu.nar" );
     _16nar::Game::get_game().run();
     return 0;
}
