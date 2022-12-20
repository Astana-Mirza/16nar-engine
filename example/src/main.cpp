#include <16nar/game.h>

#include <cstdlib>

int main()
{
	using namespace _16nar;
	auto& game = Game::get_game();

	game.set_window( "Example Game", 800, 600 );
	game.load_scene( "example-level.nar" );
	game.run();

	return EXIT_SUCCESS;
}