#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_node_package_reader.h>

#include <16nar/tools/constructor2d/flatbuffers/flatbuffers_read_utils.h>

#include <16nar/gen/flatbuffers/constructor2d_node_pkg_generated.h>

#include <cassert>
#include <stdexcept>

namespace _16nar::tools::constructor2d
{

FlatBuffersNodePackageReader::FlatBuffersNodePackageReader( const std::byte *data, std::size_t size ):
     pieces_{}, package_{}
{
     if ( !data || !size )
     {
          throw std::runtime_error{ "passed empty buffer to flatbuffers node package reader" };
     }

     const auto ptr = reinterpret_cast< const std::uint8_t * >( data );
     flatbuffers::Verifier verifier{ ptr, size };
     if ( !_16nar::data::constructor2d::VerifyNodePackage2DBuffer( verifier )
          || !( package_ = _16nar::data::constructor2d::GetNodePackage2D( ptr ) ) )
     {
          throw std::runtime_error{ "2d node package is broken" };
     }

     const std::uint32_t version = package_->version();
     assert( ( version & NARENGINE_VERSION_COMPATIBLE_MASK ) == NARENGINE_VERSION_NO_PATCH_UINT32 );

     const auto *pieces_ptr = package_->pieces();
     if ( !pieces_ptr )
     {
          throw std::runtime_error{ "2d node package pieces are broken" };
     }

     const auto& pieces = *pieces_ptr;
     pieces_.reserve( pieces.size() );
     for ( const auto *piece : pieces )
     {
          if ( !piece || !piece->name() || !piece->name()->c_str()
               || !piece->name()->size() || !piece->piece() )
          {
               throw std::runtime_error{ "2d node package named piece is broken" };
          }
          pieces_.emplace( piece->name()->c_str(), piece->piece() );
     }
}


Dependencies FlatBuffersNodePackageReader::get_dependencies()
{
     return read_scene_deps( package_->deps() );
}


IScenePieceReader& FlatBuffersNodePackageReader::get_scene_piece_reader( const std::string& name )
{
     return pieces_.at( name );
}


std::size_t FlatBuffersNodePackageReader::get_scene_piece_count()
{
     return pieces_.size();
}


std::vector< std::string > FlatBuffersNodePackageReader::get_scene_piece_names()
{
     std::vector< std::string > names;
     names.reserve( pieces_.size() );
     for ( const auto& iter : pieces_ )
     {
          names.emplace_back( iter.first );
     }
     return names;
}

} // namespace _16nar::tools::constructor2d
