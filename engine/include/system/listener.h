/// @file
/// @brief File with listener functions' declaration.
#ifndef _16NAR_LISTENER_H
#define _16NAR_LISTENER_H

#include <16nardefs.h>

namespace _16nar
{

namespace listener
{

#ifdef USE_SFML

/// @brief Sets global volume, from 0 to 100.
/// @param volume global volume.
void ENGINE_API set_global_volume( float volume );

/// @brief Sets position of the listener.
/// @param position listener's coordinate vector.
void ENGINE_API set_position( const Vector2f& position );

/// @brief Sets position of the listener.
/// @param x listener's x coordinate.
/// @param y listener's y coordinate.
void ENGINE_API set_position( float x, float y );

/// @brief Sets z coordinate for 3D sound.
/// @param value value of z coordinate.
void ENGINE_API set_z( float value );

/// @brief Gets global volume, from 0 to 100.
float ENGINE_API get_global_volume();

/// @brief Gets position of the listener.
Vector2f ENGINE_API get_position();

/// @brief Gets z coordinate for 3D sound.
float ENGINE_API get_z();

#endif // #ifdef USE_SFML

} // namespace listener

} // namespace _16nar

#endif // #ifndef _16NAR_LISTENER_H
