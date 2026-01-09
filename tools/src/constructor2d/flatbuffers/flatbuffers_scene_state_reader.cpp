#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_scene_state_reader.h>

#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_read_utils.h>

#include <16nar/gen/flatbuffers/constructor2d_scene_generated.h>

#include <stdexcept>

namespace _16nar::tools::constructor2d
{

FlatBuffersSceneStateReader::FlatBuffersSceneStateReader() = default;


FlatBuffersSceneStateReader::FlatBuffersSceneStateReader( const data::constructor2d::SceneState2D *state ):
     render_system_reader_{},
     node_reader_{},
     nodes_{},
     begin_index_{},
     end_index_{},
     curr_index_{},
     curr_node_type_{ NodeType::Node2D },
     render_system_type_{ RenderSystemType::QTreeRenderSystem },
     rendering_{},
     updating_{}
{
     if ( !state )
     {
          throw std::runtime_error{ "2d scene state is broken" };
     }

     const auto *nodes = state->nodes();
     if ( !nodes )
     {
          throw std::runtime_error{ "2d scene state has broken nodes" };
     }

     end_index_ = nodes->size();
     nodes_ = reinterpret_cast< const void * >( nodes );
     if ( !is_empty() )
     {
          init_current_node_reader();
     }

     render_system_reader_ = make_render_system_reader( state->render_system(), render_system_type_ );
}


bool FlatBuffersSceneStateReader::get_rendering()
{
     return rendering_;
}


bool FlatBuffersSceneStateReader::get_updating()
{
     return updating_;
}


IPropsReader& FlatBuffersSceneStateReader::get_render_system_reader()
{
     return render_system_reader_;
}


RenderSystemType FlatBuffersSceneStateReader::get_render_system_type()
{
     return render_system_type_;
}


bool FlatBuffersSceneStateReader::is_empty()
{
     return begin_index_ == end_index_;
}


IPropsReader& FlatBuffersSceneStateReader::get_current_node_reader()
{
     return node_reader_;
}


NodeType FlatBuffersSceneStateReader::get_current_node_type()
{
     return curr_node_type_;
}


bool FlatBuffersSceneStateReader::next_node()
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


void FlatBuffersSceneStateReader::init_current_node_reader()
{
     const auto *nodes = reinterpret_cast< const ::flatbuffers::Vector<
          ::flatbuffers::Offset< data::constructor2d::NodeData > > * >( nodes_ );
     const auto *node = ( *nodes )[ curr_index_ ];
     node_reader_ = make_node_reader( node, curr_node_type_ );
}

} // namespace _16nar::tools::constructor2d
