#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_scene_reader.h>

#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_scene_state_reader.h>
#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_read_utils.h>

#include <16nar/gen/flatbuffers/constructor2d_scene_generated.h>

#include <type_traits>
#include <stdexcept>
#include <cassert>

namespace _16nar::tools::constructor2d
{

FlatBuffersSceneReader::FlatBuffersSceneReader( const std::byte *data, std::size_t size ):
     pieces_{}, state_reader_{}, scene_{}, states_{},
     begin_index_{}, end_index_{}, curr_index_{}
{
     if ( !data || !size )
     {
          throw std::runtime_error{ "passed empty buffer to flatbuffers scene reader" };
     }

     const auto ptr = reinterpret_cast< const std::uint8_t * >( data );
     flatbuffers::Verifier verifier{ ptr, size };
     if ( !_16nar::data::constructor2d::VerifyScene2DBuffer( verifier )
          || !( scene_ = _16nar::data::constructor2d::GetScene2D( ptr ) ) )
     {
          throw std::runtime_error{ "2d scene is broken" };
     }

     const std::uint32_t version = scene_->version();
     assert( ( version & NARENGINE_VERSION_COMPATIBLE_MASK ) == NARENGINE_VERSION_NO_PATCH_UINT32 );

     const auto *states = scene_->states();
     if ( !states )
     {
          throw std::runtime_error{ "2d scene has no states" };
     }
     states_ = reinterpret_cast< const void * >( states );
     end_index_ = states->size();
     if ( curr_index_ != end_index_ )
     {
          const auto *state = ( *states )[ curr_index_ ];
          state_reader_ = FlatBuffersSceneStateReader{ state };
     }

     const auto *pieces_ptr = scene_->internal_pieces();
     if ( !pieces_ptr )
     {
          return; // "internal_pieces" field is optional
     }

     const auto& pieces = *pieces_ptr;
     pieces_.reserve( pieces.size() );
     for ( const auto piece : pieces )
     {
          pieces_.emplace_back( piece );
     }
}


Dependencies FlatBuffersSceneReader::get_dependencies()
{
     return read_scene_deps( scene_->deps() );
}


std::optional< ResourceIndex > FlatBuffersSceneReader::get_setup_func()
{
     const auto *res = scene_->setup_func();
     if ( res )
     {
          return ResourceIndex{ res->package(), res->resource() };
     }
     return {};
}


std::optional< ResourceIndex > FlatBuffersSceneReader::get_loop_func()
{
     const auto *res = scene_->loop_func();
     if ( res )
     {
          return ResourceIndex{ res->package(), res->resource() };
     }
     return {};
}


bool FlatBuffersSceneReader::is_empty()
{
     return begin_index_ == end_index_;
}


ISceneStateReader& FlatBuffersSceneReader::get_current_state_reader()
{
     return state_reader_;
}


bool FlatBuffersSceneReader::next_state()
{
     if ( curr_index_ == end_index_ )
     {
          return false;
     }
     if ( ++curr_index_ == end_index_ )
     {
          return false;
     }

     using T = data::constructor2d::Scene2D;
     const auto fn = &T::states;
     const auto *states = reinterpret_cast< std::invoke_result_t< decltype( fn ), T > >( states_ );
     const auto *state = ( *states )[ curr_index_ ];
     state_reader_ = FlatBuffersSceneStateReader{ state };
     return true;
}


IScenePieceReader& FlatBuffersSceneReader::get_scene_piece_reader( std::size_t idx )
{
     return pieces_[ idx ];
}


std::size_t FlatBuffersSceneReader::get_scene_piece_count()
{
     return pieces_.size();
}

} // namespace _16nar::tools::constructor2d
