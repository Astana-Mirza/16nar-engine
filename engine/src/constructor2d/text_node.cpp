#include <16nar/constructor/text_node.h>

namespace _16nar
{

#ifdef USE_SFML

TextNode::TextNode( RenderSystem *render_system, const std::string& string, const Font& font, uint32_t char_size ):
     DrawableNode::DrawableNode( render_system ), text_( string, font, char_size ) {}


RenderData TextNode::get_render_data() const
{
     return RenderData{ get_blend(), get_global_transform_matr( false ), nullptr, get_shader() };
}


void TextNode::set_string( const std::string& string )
{
     text_.setString( string );
}


void TextNode::set_font( const Font& font )
{
     text_.setFont( font );
}


void TextNode::set_character_size( uint32_t char_size )
{
     text_.setCharacterSize( char_size );
}


void TextNode::set_line_spacing( float factor )
{
     text_.setLineSpacing( factor );
}


void TextNode::set_letter_spacing( float factor )
{
     text_.setLetterSpacing( factor );
}


void TextNode::set_style( uint32_t style )
{
     text_.setStyle( style );
}


void TextNode::set_color( const Color& color )
{
     text_.setFillColor( color );
}


void TextNode::set_outline_color( const Color& color )
{
     text_.setOutlineColor( color );
}


void TextNode::set_outline_thickness( float thickness )
{
     text_.setOutlineThickness( thickness );
}


std::string TextNode::get_string() const
{
     return text_.getString().toAnsiString();
}


const Font *TextNode::get_font() const
{
     return text_.getFont();
}


uint32_t TextNode::get_character_size() const
{
     return text_.getCharacterSize();
}


float TextNode::get_line_spacing() const
{
     return text_.getLineSpacing();
}


float TextNode::get_letter_spacing() const
{
     return text_.getLetterSpacing();
}


uint32_t TextNode::get_style() const
{
     return text_.getStyle();
}


const Color& TextNode::get_color() const
{
     return text_.getFillColor();
}


const Color& TextNode::get_outline_color() const
{
     return text_.getOutlineColor();
}


float TextNode::get_outline_thickness() const
{
     return text_.getOutlineThickness();
}


FloatRect TextNode::get_local_bounds() const
{
     return text_.getLocalBounds();
}


const Vector2f& TextNode::get_position() const
{
     return text_.getPosition();
}


float TextNode::get_rotation() const
{
     return text_.getRotation();
}


const Vector2f& TextNode::get_scale() const
{
     return text_.getScale();
}


const Vector2f& TextNode::get_origin() const
{
     return text_.getOrigin();
}


const TransformMatrix& TextNode::get_transform_matr() const
{
     return text_.getTransform();
}


const TransformMatrix& TextNode::get_inv_transform_matr() const
{
     return text_.getInverseTransform();
}


void TextNode::set_position( float x, float y )
{
     text_.setPosition( x, y );
     transformed_ = true;
}


void TextNode::set_position( const Vector2f& position )
{
     text_.setPosition( position );
     transformed_ = true;
}


void TextNode::set_rotation( float angle )
{
     text_.setRotation( angle );
     transformed_ = true;
}


void TextNode::set_scale( float factor_x, float factor_y )
{
     text_.setScale( factor_x, factor_y );
     transformed_ = true;
}


void TextNode::set_scale( const Vector2f& factors )
{
     text_.setScale( factors );
     transformed_ = true;
}


void TextNode::set_origin( float x, float y )
{
     text_.setOrigin( x, y );
     transformed_ = true;
}


void TextNode::set_origin( const Vector2f& origin )
{
     text_.setOrigin( origin );
     transformed_ = true;
}


void TextNode::move( float offset_x, float offset_y )
{
     text_.move( offset_x, offset_y );
     transformed_ = true;
}


void TextNode::move( const Vector2f& offset )
{
     text_.move( offset );
     transformed_ = true;
}


void TextNode::rotate( float angle )
{
     text_.rotate( angle );
     transformed_ = true;
}


void TextNode::scale( float factor_x, float factor_y )
{
     text_.scale( factor_x, factor_y );
     transformed_ = true;
}


void TextNode::scale( const Vector2f& factor )
{
     text_.scale( factor );
     transformed_ = true;
}


#endif // #ifdef USE_SFML

} // namespace _16nar
