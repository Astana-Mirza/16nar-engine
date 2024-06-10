#include <16nar/game.h>
#include <16nar/system/window.h>
#include <16nar/system/monitor.h>

#include <cstdlib>

int main()
{
     using namespace _16nar;
     //auto& game = Game::get_game();

     Game::init();
     Window win( Vec2i{ 800, 600 }, "" );

     /*game.set_window("Example Game", 800, 600);
     game.load_scene( "example-level.nar" );
     game.run();*/
     std::getchar();
     win.close();
     Game::deinit();

     return EXIT_SUCCESS;
}