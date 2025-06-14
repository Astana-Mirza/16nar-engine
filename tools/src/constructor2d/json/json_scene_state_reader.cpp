#include <16nar/tools/constructor2d/json/json_scene_state_reader.h>

#include <16nar/tools/constructor2d/json/json_utils.inl>

#include <stdexcept>

namespace _16nar::tools::constructor2d
{

JsonSceneStateReader::JsonSceneStateReader( const nlohmann::json& json ):
     json_{ &json }, render_system_reader_{ json.at( "render_system" ), false },
     node_reader_{}, end_{}, curr_node_{}
{
     const auto& nodes = json.at( "nodes" );
     curr_node_ = nodes.cbegin();
     end_ = nodes.cend();
     if ( curr_node_ != end_ )
     {
          node_reader_ = JsonPropsReader{ *curr_node_, false };
     }
}


bool JsonSceneStateReader::get_rendering()
{
     return json_ && json_->at( "rendering" );
}


bool JsonSceneStateReader::get_updating()
{
     return json_ && json_->at( "updating" );
}


IPropsReader& JsonSceneStateReader::get_render_system_reader()
{
     return render_system_reader_;
}


RenderSystemType JsonSceneStateReader::get_render_system_type()
{
     RenderSystemType ret{};
     if ( json_ )
     {
          json_->at( "render_system" ).at( "type" );
     }
     return ret;
}


IPropsReader& JsonSceneStateReader::get_current_node_reader()
{
     return node_reader_;
}


NodeType JsonSceneStateReader::get_current_node_type()
{
     NodeType type = NodeType::Node2D;
     if ( curr_node_ != end_ )
     {
          type = curr_node_->at( "type" );
     }
     return type;
}


bool JsonSceneStateReader::next_node()
{
     if ( curr_node_ == end_ )
     {
          return false;
     }
     if ( ++curr_node_ == end_ )
     {
          return false;
     }
     node_reader_ = JsonPropsReader{ *curr_node_, false };
     return true;
}

} // namespace _16nar::tools::constructor2d
