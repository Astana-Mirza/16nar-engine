/// @file
/// @brief File with TilemapNode class definition.
#ifndef _16NAR_TILEMAP_NODE_H
#define _16NAR_TILEMAP_NODE_H

#include <16nar/constructor/node_2d.h>
#include <16nar/constructor/tiles/tile.h>

#include <forward_list>

namespace _16nar
{

/// @brief Container for tiles and their rendering information.
class TilemapNode final: public Node2D
{
public:
     /// @brief Data structure with rendering settings for all tiles of this tilemap.
     struct TileDrawSettings
     {
          const Texture *texture = nullptr; ///< texture, containing full tilemap.
          BlendMode blend = BlendAlpha;     ///< mode, defining how pixels of the tiles will be blended.
          Shader *shader = nullptr;         ///< shader to be drawn with the object.
          Color color = Color{};            ///< color, unused, because tiles have texture.
          int layer = 0;                    ///< scene layer.
          bool visible = true;              ///< visibility.
     };

     using Node2D::Node2D;

     /// @brief Make a tile with given vertices and return reference to it.
     /// @param[in] render_system render system used to draw the tile.
     /// @param[in] vertex_count number of vertices of a tile.
     Tile& make_tile( RenderSystem *render_system, size_t vertex_count );

     /// @brief Gets tile rendering settings.
     TileDrawSettings& get_settings();

protected:
     /// @brief Function that launches update loop recursively.
     /// @detail If transformed, launches render system updates for all tiles, which may be expensive operation.
     /// @param[in] upd value, telling if this branch of scene tree has to be updated.
     /// @param[in] delta time since last update, in seconds.
     void loop_call( bool update, float delta );

private:
     std::forward_list< Tile > tiles_;       ///< tiles of this tilemap.
     TileDrawSettings settings_;             ///< settings for rendering all tiles.
};

} // namespace _16nar

#endif // #ifndef _16NAR_TILEMAP_NODE_H
