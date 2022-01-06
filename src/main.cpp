#include <chrono>
#include "../inc/render/quadrant.h"
#include "../inc/constructor/sprite_node.h"
#include "../inc/constructor/node_2d.h"

#include <iostream>

using namespace _16nar;


struct LandingSignal : public Signal {
	LandingSignal(const Vector2f& p) : pos{p} {}
	Vector2f pos;
};

int main() {
	/*Quadrant* q1 = new Quadrant({0, 0, 400, 300});
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

	sf::RenderWindow window(sf::VideoMode{800, 600}, "Test");
	window.draw(world_quad);
	window.display();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}*/

	Node2D planet;
	Node2D ship;
	ship.set_position(100, 100);

	planet.connect<LandingSignal>(&ship, [=](const LandingSignal& sig){
		std::cout << "Landed on (" << sig.pos.x
			<< ", " << sig.pos.y << ")" << std::endl;
	});

	ship.emit<LandingSignal>({ship.get_position()});

	return 0;
}
