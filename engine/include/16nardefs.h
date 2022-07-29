/// @file
/// File with basic definitions, needed by engine.
#ifndef _16NAR_DEFS_H
#define _16NAR_DEFS_H

#include <16narconfig.h>

#ifdef USE_SFML
#    define SFML_DEFINE_DISCRETE_GPU_PREFERENCE
#    include <SFML/Graphics.hpp>
#endif


#ifdef WIN_EXPORT
#    ifdef BUILDING_DLL
#         define ENGINE_API __declspec( dllexport )
#    else
#         define ENGINE_API __declspec( dllimport )
#    endif
#else
#    define ENGINE_API
#endif


namespace _16nar
{

#ifdef USE_SFML

namespace Style = sf::Style;

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
using sf::Keyboard;
using sf::Mouse;
using sf::Joystick;
using sf::Event;

#endif // #ifdef USE_SFML

} // namespace _16nar

#endif // #ifndef _16NAR_DEFS_H
