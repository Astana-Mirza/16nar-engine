/// @file
/// @brief File with TextNode class definition.
#ifndef _16NAR_TEXT_NODE_H
#define _16NAR_TEXT_NODE_H

#include <constructor/abstract/drawable_node.h>

namespace _16nar
{

/// @brief Node that displays text with given font.
class ENGINE_API TextNode : public DrawableNode
{
public:
     using DrawableNode::DrawableNode;

     /// @brief Constructor.
     /// @param quad base quadrant of this node.
     /// @param string string of a text.
     /// @param font text font.
     /// @param char_size size of a character.
     TextNode( Quadrant* quad, const std::string& string, const Font& font, uint32_t char_size = 30 );

     /// @brief Draw this object on the target.
     /// @param target target where object should be rendered.
     void draw( RenderTarget& target ) const;

     /// @brief Sets the sstring to be displayed.
     /// @param string string of a text.
     void set_string( const std::string& string );

     /// @brief Sets font of a text.
     /// @param font text font.
     void set_font( const Font& font );

     /// @brief Sets size of one character.
     /// @param char_size size of a character.
     void set_character_size( uint32_t char_size );

     /// @brief Sets the line spacing.
     /// @param factor factor of spacing between lines, default is 1.
     void set_line_spacing( float factor );

     /// @brief Sets the line spacing.
     /// @param factor factor of spacing between letters, default is 1.
     void set_letter_spacing( float factor );

     /// @brief Sets text style.
     /// @param style style of a text.
     void set_style( uint32_t style );

     /// @brief Sets fill color of a text.
     /// @param color fill color.
     void set_color( const Color& color );

     /// @brief Sets outline color of a text.
     /// @param color outline color.
     void set_outline_color( const Color& color );

     /// @brief Sets outline thickness of a text.
     /// @param thickness thickness of an outline.
     void set_outline_thickness( float thickness );

     /// @brief Gets text string.
     std::string get_string() const;

     /// @brief Gets text font.
     const Font *get_font() const;

     /// @brief Gets character size.
     uint32_t get_character_size() const;

     /// @brief Gets line spacing factor.
     float get_line_spacing() const;

     /// @brief Gets letter spacing factor.
     float get_letter_spacing() const;

     /// @brief Gets text style.
     uint32_t get_style() const;

     /// @brief Gets color of an object.
     const Color& get_color() const;

     /// @brief Gets outline color of the text.
     const Color& get_outline_color() const;

     /// @brief Gets outline thickness of the text.
     float get_outline_thickness() const;

     /// @brief Gets local bounds of an object (in its own coordinates).
     FloatRect get_local_bounds() const;

     /// @brief Gets current object position.
     const Vector2f& get_position() const;

     /// @brief Gets current object rotation.
     float get_rotation() const;

     /// @brief Gets current object scale.
     const Vector2f& get_scale() const;

     /// @brief Gets current object tarnsform origin point.
     const Vector2f& get_origin() const;

     /// @brief Gets current object's transformation matrix.
     const TransformMatrix& get_transform_matr() const;

     /// @brief Gets current object's inverse transformation matrix.
     const TransformMatrix& get_inv_transform_matr() const;

     /// @brief Sets current object position.
     /// @param x object's x coordinate.
     /// @param y object's y coordinate.
     void set_position( float x, float y );

     /// @brief Sets current object position.
     /// @param position object's coordinate vector.
     void set_position( const Vector2f& position );

     /// @brief Sets current object rotation.
     /// @param angle object's rotation angle, in degrees.
     void set_rotation( float angle );

     /// @brief Sets current object scale.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     void set_scale( float factor_x, float factor_y );

     /// @brief Sets current object scale.
     /// @param factors object's scale factor vector.
     void set_scale( const Vector2f& factors );

     /// @brief Sets current object transform origin point.
     /// @param x object's origin x coordinate.
     /// @param y object's origin y coordinate.
     void set_origin( float x, float y );

     /// @brief Sets current object transform origin point.
     /// @param origin object's origin coordinate vector.
     void set_origin( const Vector2f& origin );

     /// @brief Moves object by given offsets.
     /// @param offset_x object's x moving offset.
     /// @param offset_y object's y moving offset.
     void move( float offset_x, float offset_y );

     /// @brief Moves object by given offset vector.
     /// @param offset object's moving offset.
     void move( const Vector2f& offset );

     /// @brief Rotates object by given angle.
     /// @param angle object's rotation angle, in degrees.
     void rotate( float angle );

     /// @brief Scales object by given factors.
     /// @param factor_x object's x scale factor.
     /// @param factor_y object's y scale factor.
     void scale( float factor_x, float factor_y );

     /// @brief Scales current object by given factors.
     /// @param factors object's scale factor vector.
     void scale( const Vector2f& factor );

private:
     Text text_;         ///< text that is displayed on the screen.
};

} // namespace _16nar

#endif // #ifndef _16NAR_TEXT_NODE_H