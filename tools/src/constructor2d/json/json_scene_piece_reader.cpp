#include <16nar/tools/constructor2d/json/json_scene_piece_reader.h>

#include <16nar/tools/constructor2d/json/json_utils.inl>

namespace _16nar::tools::constructor2d
{

JsonScenePieceReader::JsonScenePieceReader() = default;


JsonScenePieceReader::JsonScenePieceReader( const nlohmann::json& nodes ):
     node_reader_{}, begin_{ nodes.cbegin() }, end_{ nodes.cend() }, curr_node_{ nodes.cbegin() }
{
     if ( !is_empty() )
     {
          node_reader_ = JsonPropsReader{ curr_node_->at( "props" ), false };
     }
}


bool JsonScenePieceReader::is_empty()
{
     return begin_ == end_;
}


IPropsReader& JsonScenePieceReader::get_current_node_reader()
{
     return node_reader_;
}


NodeType JsonScenePieceReader::get_current_node_type()
{
     NodeType type = NodeType::Node2D;
     if ( curr_node_ != end_ )
     {
          type = curr_node_->at( "type" );
     }
     return type;
}


bool JsonScenePieceReader::next_node()
{
     if ( curr_node_ == end_ )
     {
          return false;
     }
     if ( ++curr_node_ == end_ )
     {
          return false;
     }
     node_reader_ = JsonPropsReader{ curr_node_->at( "props" ), false };
     return true;
}

} // namespace _16nar::tools::constructor2d
