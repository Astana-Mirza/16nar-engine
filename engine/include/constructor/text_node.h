/// @file
/// File with TextNode class definition.
#ifndef _16NAR_TEXT_NODE_H
#define _16NAR_TEXT_NODE_H

#include <constructor/abstract/drawable_node.h>

namespace _16nar
{

/// Node that displays text with given font.
class ENGINE_API TextNode : public DrawableNode
{
public:
     /// Constructor.
     /// @param quad base quadrant of this node.
     /// @param string string of a text.
     /// @param font text font.
     /// @param char_size size of a character.
     TextNode( Quadrant* quad, const std::string& string, const Font& font, uint32_t char_size = 30 );

     /// Draw this object on the target.
     /// @param target target where object should be rendered.
     /// @param states options for rendering.
     void draw( RenderTarget& target, RenderStates states ) const;

     /// Sets the sstring to be displayed.
     /// @param string string of a text.
     void set_string( const std::string& string );

     /// Sets font of a text.
     /// @param font text font.
     void set_font( const Font& font );

     /// Sets size of one character.
     /// @param char_size size of a character.
     void set_character_size( uint32_t char_size );

     /// Sets the line spacing.
     /// @param factor factor of spacing between lines, default is 1.
     void set_line_spacing( float factor );

     /// Sets the line spacing.
     /// @param factor factor of spacing between letters, default is 1.
     void set_letter_spacing( float factor );

     /// Sets text style.
     /// @param style style of a text.
     void set_style( uint32_t style );

     /// Sets fill color of a text.
     /// @param color fill color.
     void set_color( const Color& color );

     /// Sets outline color of a text.
     /// @param color outline color.
     void set_outline_color( const Color& color );

     /// Sets outline thickness of a text.
     /// @param thickness thickness of an outline.
     void set_outline_thickness( float thickness );

     /// Gets text string.
     std::string get_string() const;

     /// Gets text font.
     const Font *get_font() const;

     /// Gets character size.
     uint32_t get_character_size() const;

     /// Gets line spacing factor.
     float get_line_spacing() const;

     /// Gets letter spacing factor.
     float get_letter_spacing() const;

     /// Gets text style.
     uint32_t get_style() const;

     /// Gets color of an object.
     const Color& get_color() const;

     /// Gets outline color of the text.
     const Color& get_outline_color() const;

     /// Gets outline thickness of the text.
     float get_outline_thickness() const;

     /// Gets local bounds of an object (in its own coordinates).
     FloatRect get_local_bounds() const;

     /// Gets current object position.
     const Vector2f& get_position() const;

     /// Gets current object rotation.
     float get_rotation() const;

     /// Gets current object scale.
     const Vector2f& get_scale() const;

     /// Gets current object tarnsform origin point.
     const Vector2f& get_origin() const;

     /// Gets current object's transformation matrix.
     const TransformMatrix& get_transform_matr() const;

     /// Gets current object's inverse transformation matrix.
     const TransformMatrix& get_inv_transform_matr() const;

     /// Sets current object position.
     /// @param x object's x coordinate.
     /// @param y object's y coordinate.
     void set_position( float x, float y );

     /// Sets current object position.
     /// @param position object's coordinate vector.
     void set_position( const Vector2f& position );

     /// Sets current object rotation.
     /// @param angle object's rotation angle, in degrees.
     void set_rotation( float angle );

     /// Sets current object scale.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     void set_scale( float factor_x, float factor_y );

     /// Sets current object scale.
     /// @param factors object's scale factor vector.
     void set_scale( const Vector2f& factors );

     /// Sets current object transform origin point.
     /// @param x object's origin x coordinate.
     /// @param y object's origin y coordinate.
     void set_origin( float x, float y );

     /// Sets current object transform origin point.
     /// @param origin object's origin coordinate vector.
     void set_origin( const Vector2f& origin );

     /// Moves object by given offsets.
     /// @param offset_x object's x moving offset.
     /// @param offset_y object's y moving offset.
     void move( float offset_x, float offset_y );

     /// Moves object by given offset vector.
     /// @param offset object's moving offset.
     void move( const Vector2f& offset );

     /// Rotates object by given angle.
     /// @param angle object's rotation angle, in degrees.
     void rotate( float angle );

     /// Scales object by given factors.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     void scale( float factor_x, float factor_y );

     /// Scales current object by given factors.
     /// @param factors object's scale factor vector.
     void scale( const Vector2f& factor );

private:
     Text text_;         ///< text that is displayed on the screen.
};

} // namespace _16nar

#endif // #ifndef _16NAR_TEXT_NODE_H