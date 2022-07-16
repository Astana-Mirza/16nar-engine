/// @file
/// File with basic definitions, needed by engine.
#ifndef _16NAR_DEFS_H
#define _16NAR_DEFS_H

#ifdef RENDER_SFML
#include <SFML/Graphics.hpp>
#endif

namespace _16nar {

#ifdef RENDER_SFML

using Transform = sf::Transformable;
using DrawableBase = sf::Drawable;
using TransformMatrix = sf::Transform;

using sf::Vector2f;
using sf::IntRect;
using sf::FloatRect;
using sf::RenderTarget;
using sf::RenderStates;
using sf::RenderWindow;
using sf::Sprite;
using sf::Texture;
using sf::Clock;
using sf::Time;
using sf::Keyboard;
using sf::Mouse;
using sf::Joystick;
using sf::Event;

#endif // #ifdef RENDER_SFML

} // namespace _16nar

#endif // #ifndef _16NAR_DEFS_H
