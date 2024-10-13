/// @file
/// @brief Header file with Tile class definition.
#ifndef _16NAR_TILE_H
#define _16NAR_TILE_H

#include <16nar/16nardefs.h>
#include <16nar/abstract/drawable.h>

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
     /// @param[in] render_system render system used to draw this object.
     /// @param[in] tilemap tilemap node keeping this tile.
     /// @param[in] vertex_count count of vertices of the tile.
     Tile( RenderSystem *render_system, TilemapNode& tilemap, size_t vertex_count );

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
     bool is_visible() const override;

     /// @brief Sets visibility of the object.
     /// @param[in] visible visibility.
     void set_visible( bool visible ) override;

     /// @brief Gets the scene layer of this object.
     int get_layer() const override;

     /// @brief Sets scene layer of this object.
     /// @param[in] layer scene layer.
     void set_layer( int layer ) override;

     /// @brief Sets shader of this object.
     /// @param[in] shader pointer to shader.
     void set_shader( Shader *shader ) override;

     /// @brief Gets shader of this object.
     Shader *get_shader() const override;

     /// @brief Sets the blend mode of this object.
     /// @param[in] blend blend mode of this object.
     void set_blend( const BlendMode& blend ) override;

     /// @brief Gets the blend mode of this object.
     const BlendMode& get_blend() const override;

     /// @brief Gets local bounds of an object (in its own coordinates).
     FloatRect get_local_bounds() const override;

     /// @brief Gets global bounds of an object (in world coordinates).
     FloatRect get_global_bounds() const override;

protected:
     /// @brief Get render data of the object.
     RenderData get_render_data() const override;

     /// @brief Sets color of an object.
     /// @param[in] color color of an object.
     void set_color( const Color& color ) override;

     /// @brief Gets color of an object.
     const Color& get_color() const override;

private:
     TilemapNode& tilemap_;   ///< tilemap node keeping this tile.
     VertexArray vertices_;   ///< vertices of this tile.
};

} // namespace _16nar

#endif // #ifndef _16NAR_TILE_H
