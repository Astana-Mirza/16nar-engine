/// @file
/// @brief File with TextNode class definition.
#ifndef _16NAR_TEXT_NODE_H
#define _16NAR_TEXT_NODE_H

#include <16nar/constructor/abstract/drawable_node.h>

namespace _16nar
{

/// @brief Node that displays text with given font.
class ENGINE_API TextNode : public DrawableNode
{
public:
     using DrawableNode::DrawableNode;

     /// @brief Constructor.
     /// @param[in] render_system render system used to draw this object.
     /// @param[in] string string of a text.
     /// @param[in] font text font.
     /// @param[in] char_size size of a character.
     TextNode( RenderSystem *render_system, const std::string& string, const Font& font, uint32_t char_size = 30 );

     /// @brief Get render data of the object.
     RenderData get_render_data() const override;

     /// @brief Sets the string to be displayed.
     /// @param[in] string string of a text.
     void set_string( const std::string& string );

     /// @brief Sets font of a text.
     /// @param[in] font text font.
     void set_font( const Font& font );

     /// @brief Sets size of one character.
     /// @param[in] char_size size of a character.
     void set_character_size( uint32_t char_size );

     /// @brief Sets the line spacing.
     /// @param[in] factor factor of spacing between lines, default is 1.
     void set_line_spacing( float factor );

     /// @brief Sets the line spacing.
     /// @param[in] factor factor of spacing between letters, default is 1.
     void set_letter_spacing( float factor );

     /// @brief Sets text style.
     /// @param[in] style style of a text.
     void set_style( uint32_t style );

     /// @brief Sets fill color of a text.
     /// @param[in] color fill color.
     void set_color( const Color& color ) override;

     /// @brief Sets outline color of a text.
     /// @param[in] color outline color.
     void set_outline_color( const Color& color );

     /// @brief Sets outline thickness of a text.
     /// @param[in] thickness thickness of an outline.
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
     const Color& get_color() const override;

     /// @brief Gets outline color of the text.
     const Color& get_outline_color() const;

     /// @brief Gets outline thickness of the text.
     float get_outline_thickness() const;

     /// @brief Gets local bounds of an object (in its own coordinates).
     FloatRect get_local_bounds() const override;

     /// @brief Gets current object position.
     const Vector2f& get_position() const override;

     /// @brief Gets current object rotation.
     float get_rotation() const override;

     /// @brief Gets current object scale.
     const Vector2f& get_scale() const override;

     /// @brief Gets current object tarnsform origin point.
     const Vector2f& get_origin() const override;

     /// @brief Gets current object's transformation matrix.
     const TransformMatrix& get_transform_matr() const override;

     /// @brief Gets current object's inverse transformation matrix.
     const TransformMatrix& get_inv_transform_matr() const override;

     /// @brief Sets current object position.
     /// @param[in] x object's x coordinate.
     /// @param[in] y object's y coordinate.
     void set_position( float x, float y ) override;

     /// @brief Sets current object position.
     /// @param[in] position object's coordinate vector.
     void set_position( const Vector2f& position ) override;

     /// @brief Sets current object rotation.
     /// @param[in] angle object's rotation angle, in degrees.
     void set_rotation( float angle ) override;

     /// @brief Sets current object scale.
     /// @param[in] factor_x object's x scale factor.
     /// @param[in] factor_y object's y scale factor.
     void set_scale( float factor_x, float factor_y ) override;

     /// @brief Sets current object scale.
     /// @param[in] factors object's scale factor vector.
     void set_scale( const Vector2f& factors ) override;

     /// @brief Sets current object transform origin point.
     /// @param[in] x object's origin x coordinate.
     /// @param[in] y object's origin y coordinate.
     void set_origin( float x, float y ) override;

     /// @brief Sets current object transform origin point.
     /// @param[in] origin object's origin coordinate vector.
     void set_origin( const Vector2f& origin ) override;

     /// @brief Moves object by given offsets.
     /// @param[in] offset_x object's x moving offset.
     /// @param[in] offset_y object's y moving offset.
     void move( float offset_x, float offset_y ) override;

     /// @brief Moves object by given offset vector.
     /// @param[in] offset object's moving offset.
     void move( const Vector2f& offset ) override;

     /// @brief Rotates object by given angle.
     /// @param[in] angle object's rotation angle, in degrees.
     void rotate( float angle ) override;

     /// @brief Scales object by given factors.
     /// @param[in] factor_x object's x scale factor.
     /// @param[in] factor_y object's y scale factor.
     void scale( float factor_x, float factor_y ) override;

     /// @brief Scales current object by given factors.
     /// @param[in] factors object's scale factor vector.
     void scale( const Vector2f& factor ) override;

private:
     Text text_;         ///< text that is displayed on the screen.
};

} // namespace _16nar

#endif // #ifndef _16NAR_TEXT_NODE_H
