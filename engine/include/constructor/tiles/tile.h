/// @file
/// @brief Header file with Tile class definition.
#ifndef _16NAR_TILE_H
#define _16NAR_TILE_H

#include <16nardefs.h>
#include <abstract/drawable.h>

namespace _16nar
{

class TilemapNode;

/// @brief Class that represents one tile in a tilemap.
/// @detail Tile is flyweight, all drawing information is stored in its tilemap's settings,
/// this class only stores tile's position and texture area. Editing functions apply to all
/// tiles of this tile's tilemap.
class ENGINE_API Tile : public Drawable
{
public:
     /// @brief Constructor.
     /// @param[in] quad pointer to quadrant of this object.
     /// @param[in] tilemap tilemap node keeping this tile.
     /// @param[in] vertex_count count of vertices of the tile.
     Tile( Quadrant *quad, TilemapNode& tilemap, size_t vertex_count );

     /// @brief Copy constructor.
     /// @param[in] other tile, which will be copied.
     Tile( const Tile& other );

     /// @brief Gets this tile's vertex count.
     size_t get_vertex_count() const;

     /// @brief Gets vertex at given index.
     /// @param[in] index index of a vertex.
     Vertex& get_vertex( size_t index );

     /// @brief Sets vertex array's primitive type.
     /// @param[in] type primitive type.
     void set_primitive_type( PrimitiveType type );

     /// @brief Gets vertex array's primitive type.
     PrimitiveType get_primitive_type() const;

     /// @brief Checks if the object is visible.
     bool is_visible() const;

     /// @brief Sets visibility of the object.
     /// @param[in] visible visibility.
     void set_visible( bool visible );

     /// @brief Gets the scene layer of this object.
     int get_layer() const;

     /// @brief Sets scene layer of this object.
     /// @param[in] layer scene layer.
     void set_layer( int layer );

     /// @brief Sets shader of this object.
     /// @param[in] shader pointer to shader.
     void set_shader( Shader *shader );

     /// @brief Gets shader of this object.
     Shader *get_shader() const;

     /// @brief Sets the blend mode of this object.
     /// @param[in] blend blend mode of this object.
     void set_blend( const BlendMode& blend );

     /// @brief Gets the blend mode of this object.
     const BlendMode& get_blend() const;

     /// @brief Gets local bounds of an object (in its own coordinates).
     FloatRect get_local_bounds() const;

protected:
     /// @brief Draws an object on a given target.
     /// @param[in] target render target where the object should be drawn.
     void draw( RenderTarget& target ) const;

     /// @brief Sets color of an object.
     /// @param[in] color color of an object.
     void set_color( const Color& color );

     /// @brief Gets color of an object.
     const Color& get_color() const;

     /// @brief Check if this object fits in specified quadrant.
     /// @param[in] quad pointer to quadrant to be checked.
     bool check_quadrant( const Quadrant *quad ) const;

private:
     TilemapNode& tilemap_;   ///< tilemap node keeping this tile.
     VertexArray vertices_;   ///< vertices of this tile.
};

} // namespace _16nar

#endif // #ifndef _16NAR_TILE_H
