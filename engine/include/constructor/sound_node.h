/// @file
/// File with SoundNode class definition.
#ifndef _16NAR_SOUND_NODE_H
#define _16NAR_SOUND_NODE_H

#include <16nardefs.h>
#include <constructor/node_2d.h>

namespace _16nar
{

/// Node that can play sound from given buffer.
class ENGINE_API SoundNode : public Node2D
{
public:
     using Node2D::Node2D;

     /// Constructor.
     /// @param buffer sound buffer to be played with this sound.
     SoundNode( const SoundBuffer& buffer );

     /// Sets current object position.
     /// @param x object's x coordinate.
     /// @param y object's y coordinate.
     void set_position( float x, float y );

     /// Sets current object position.
     /// @param position object's coordinate vector.
     void set_position( const Vector2f& position );

     /// Moves object by given offsets.
     /// @param offset_x object's x moving offset.
     /// @param offset_y object's y moving offset.
     void move( float offset_x, float offset_y );

     /// Moves object by given offset vector.
     /// @param offset object's moving offset.
     void move( const Vector2f& offset );

     /// Sets z coordinate for 3D sound.
     /// @param value value of z coordinate.
     void set_z( float value );

     /// Sets sound buffer to be played.
     /// @param buffer sound buffer to be played with this sound.
     void set_buffer( const SoundBuffer& buffer );

     /// Should the sound be played in a loop.
     /// @param loop play sound in a loop or not.
     void set_loop( bool loop );

     /// Sets playing offset of a sound.
     /// @param offset playing offset.
     void set_offset( float offset );

     /// Sets playing volume.
     /// @param volume playing volume.
     void set_volume( float volume );

     /// Sets sound pitch.
     /// @param pitch sound pitch.
     void set_pitch( float pitch );

     /// Should the sound be relative to listener or absolute.
     /// @param relative should the sound be relative or not.
     void set_relative_to_listener( bool relative );

     /// Distance on which the sound will not fade out.
     /// @param distance minimal distance of a sound.
     void set_min_distance( float distance );

     /// Sets attenuation factor of the sound.
     /// @param attenuation attenuation factor of the sound.
     void set_attenuation( float attenuation );

     /// Plays the sound.
     void play();

     /// Pause the sound.
     void pause();

     /// Stop the sound.
     void stop();

     /// Gets the z coordinate.
     float get_z() const;

     /// Gets the sound buffer of the sound.
     const SoundBuffer *get_buffer() const;

     /// Gets the loop option.
     bool get_loop() const;

     /// Gets playing offset.
     float get_offset() const;

     /// Gets sound volume.
     float get_volume() const;

     /// Gets sound pitch.
     float get_pitch() const;

     /// Checks if sound is relative to listener.
     bool get_relative_to_listener() const;

     /// Gets minimal distance of the sound.
     float get_min_distance() const;

     /// Gets attenuation of the sound.
     float get_attenuation() const;

private:
     Sound sound_;       ///< sound which is played.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SOUND_NODE_H