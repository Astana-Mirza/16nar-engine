#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_scene_piece_reader.h>

#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_read_utils.h>

#include <16nar/gen/flatbuffers/constructor2d_nodes_generated.h>

#include <stdexcept>

namespace _16nar::tools::constructor2d
{

FlatBuffersScenePieceReader::FlatBuffersScenePieceReader() = default;


FlatBuffersScenePieceReader::FlatBuffersScenePieceReader( const data::constructor2d::ScenePiece2D *piece ):
     node_reader_{}, nodes_{}, begin_index_{}, end_index_{}, curr_index_{}
{
     if ( !piece )
     {
          throw std::runtime_error{ "2d scene piece is broken" };
     }

     const auto *nodes = piece->nodes();
     if ( !nodes )
     {
          throw std::runtime_error{ "2d scene piece has broken nodes" };
     }

     end_index_ = nodes->size();
     nodes_ = reinterpret_cast< const void * >( nodes );
     if ( !is_empty() )
     {
          init_current_node_reader();
     }
}


bool FlatBuffersScenePieceReader::is_empty()
{
     return begin_index_ == end_index_;
}


IPropsReader& FlatBuffersScenePieceReader::get_current_node_reader()
{
     return node_reader_;
}


NodeType FlatBuffersScenePieceReader::get_current_node_type()
{
     return curr_node_type_;
}


bool FlatBuffersScenePieceReader::next_node()
{
     if ( curr_index_ == end_index_ )
     {
          return false;
     }
     if ( ++curr_index_ == end_index_ )
     {
          return false;
     }
     init_current_node_reader();
     return true;
}


void FlatBuffersScenePieceReader::init_current_node_reader()
{
     const auto *nodes = reinterpret_cast< const ::flatbuffers::Vector<
          ::flatbuffers::Offset< data::constructor2d::NodeData > > * >( nodes_ );
     const auto *node = ( *nodes )[ curr_index_ ];
     node_reader_ = make_node_reader( node, curr_node_type_ );
}

} // namespace _16nar::tools::constructor2d
