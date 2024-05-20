/// @file
/// @brief File with SoundNode class definition.
#ifndef _16NAR_SOUND_NODE_H
#define _16NAR_SOUND_NODE_H

#include <16nar/16nardefs.h>
#include <16nar/constructor/node_2d.h>

namespace _16nar
{

/// @brief Node that can play sound from given buffer.
class ENGINE_API SoundNode : public Node2D
{
public:
     using Node2D::Node2D;

     /// @brief Constructor.
     /// @param[in] buffer sound buffer to be played with this sound.
     SoundNode( const SoundBuffer& buffer );

     /// @brief Sets current object position.
     /// @param[in] x object's x coordinate.
     /// @param[in] y object's y coordinate.
     void set_position( float x, float y ) override;

     /// @brief Sets current object position.
     /// @param[in] position object's coordinate vector.
     void set_position( const Vector2f& position ) override;

     /// @brief Moves object by given offsets.
     /// @param[in] offset_x object's x moving offset.
     /// @param[in] offset_y object's y moving offset.
     void move( float offset_x, float offset_y ) override;

     /// @brief Moves object by given offset vector.
     /// @param[in] offset object's moving offset.
     void move( const Vector2f& offset ) override;

     /// @brief Sets z coordinate for 3D sound.
     /// @param[in] value value of z coordinate.
     void set_z( float value );

     /// @brief Sets sound buffer to be played.
     /// @param[in] buffer sound buffer to be played with this sound.
     void set_buffer( const SoundBuffer& buffer );

     /// @brief Should the sound be played in a loop.
     /// @param[in] loop play sound in a loop or not.
     void set_loop( bool loop );

     /// @brief Sets playing offset of a sound.
     /// @param[in] offset playing offset.
     void set_offset( float offset );

     /// @brief Sets playing volume.
     /// @param[in] volume playing volume.
     void set_volume( float volume );

     /// @brief Sets sound pitch.
     /// @param[in] pitch sound pitch.
     void set_pitch( float pitch );

     /// @brief Should the sound be relative to listener or absolute.
     /// @param[in] relative should the sound be relative or not.
     void set_relative_to_listener( bool relative );

     /// @brief Distance on which the sound will not fade out.
     /// @param[in] distance minimal distance of a sound.
     void set_min_distance( float distance );

     /// @brief Sets attenuation factor of the sound.
     /// @param[in] attenuation attenuation factor of the sound.
     void set_attenuation( float attenuation );

     /// @brief Plays the sound.
     void play();

     /// @brief Pause the sound.
     void pause();

     /// @brief Stop the sound.
     void stop();

     /// @brief Gets the z coordinate.
     float get_z() const;

     /// @brief Gets the sound buffer of the sound.
     const SoundBuffer *get_buffer() const;

     /// @brief Gets the loop option.
     bool get_loop() const;

     /// @brief Gets playing offset.
     float get_offset() const;

     /// @brief Gets sound volume.
     float get_volume() const;

     /// @brief Gets sound pitch.
     float get_pitch() const;

     /// @brief Checks if sound is relative to listener.
     bool get_relative_to_listener() const;

     /// @brief Gets minimal distance of the sound.
     float get_min_distance() const;

     /// @brief Gets attenuation of the sound.
     float get_attenuation() const;

private:
     Sound sound_;       ///< sound which is played.
};

} // namespace _16nar

#endif // #ifndef _16NAR_SOUND_NODE_H
