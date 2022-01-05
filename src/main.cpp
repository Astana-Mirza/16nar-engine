#include <chrono>
#include "../inc/render/quadrant.h"
#include "../inc/constructor/sprite_node.h"

using namespace _16nar;

int main() {
	Quadrant* q1 = new Quadrant({0, 0, 400, 300});
	Quadrant* q2 = new Quadrant({400, 0, 400, 300});
	Quadrant* q3 = new Quadrant({0, 300, 400, 300});
	Quadrant* q4 = new Quadrant({400, 300, 400, 300});
	Quadrant world_quad({0, 0, 800, 600});
	world_quad.add_child(q1, 0);
	world_quad.add_child(q2, 1);
	world_quad.add_child(q3, 2);
	world_quad.add_child(q4, 3);

	Texture tex;
	tex.loadFromFile("res/Enterprise.png");

	SpriteNode ship(&world_quad, tex);
	ship.set_position(800, 600);

	sf::RenderWindow window(sf::VideoMode{1280, 720}, "Test");
	window.draw(world_quad);
	window.display();

	while (window.isOpen()) {
		sf::Event event;
	        while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
	        }
	}

	return 0;
}
