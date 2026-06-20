#include <16nar/core/assets/json/json_to_flatbuffers_convertor.h>

#include <flatbuffers/util.h>

namespace _16nar::assets
{

JsonToFlatBuffersConvertor::JsonToFlatBuffersConvertor( strings::NameTable& type_names ):
     parser_{}, type_names_{ type_names }, json_out_{}, error_{}
{
     // use quotes in field names
     parser_.opts.strict_json = true;

     // print one-line JSON
     parser_.opts.indent_step = -1;
}


bool JsonToFlatBuffersConvertor::add_schema( const std::string& schema_data,
     const std::vector< std::string >& include_paths,
     const std::string& schema_path )
{
     std::vector< std::string > posix_paths;
     std::vector< const char * > c_paths;
     std::string posix_schema_path;

     if ( !include_paths.empty() )
     {
          posix_paths.reserve( include_paths.size() );
          c_paths.reserve( include_paths.size() + 1 );
          for ( const auto& path : include_paths )
          {
               posix_paths.emplace_back( flatbuffers::PosixPath( path ) );
               c_paths.emplace_back( posix_paths.back().c_str() );
          }
          c_paths.emplace_back( nullptr );
     }

     if ( !schema_path.empty() )
     {
          posix_schema_path = flatbuffers::PosixPath( schema_path );
     }

     return parser_.Parse( schema_data.c_str(),
          c_paths.empty() ? nullptr : c_paths.data(),
          posix_schema_path.empty() ? nullptr : posix_schema_path.c_str() );
}


AssetData JsonToFlatBuffersConvertor::convert_forward( AssetData content )
{
     if( !content.data )
     {
          error_ = "empty asset data";
          return AssetData{};
     }

     if ( !set_root_type( content.type_id ) )
     {
          return AssetData{};
     }

     error_.clear();
     if ( !parser_.ParseJson( reinterpret_cast< const char * >( content.data.data ) ) )
     {
          error_ = std::move( parser_.error_ );
          return AssetData{};
     }

     const std::uint8_t *buf = parser_.builder_.GetBufferPointer();
     const std::size_t size = parser_.builder_.GetSize();
     AssetData result{};
     result.type_id = content.type_id;
     result.data = memory::ConstByteView{ reinterpret_cast< const std::byte * >( buf ), size };
     return result;
}


AssetData JsonToFlatBuffersConvertor::convert_backward( AssetData content )
{
     if( !content.data )
     {
          error_ = "empty asset data";
          return AssetData{};
     }

     if ( !set_root_type( content.type_id ) )
     {
          return AssetData{};
     }

     error_.clear();
     json_out_.clear();
     const char *error_str = flatbuffers::GenText( parser_, content.data.data, &json_out_ );
     if ( error_str )
     {
          error_ = error_str;
          return AssetData{};
     }

     AssetData result{};
     result.type_id = content.type_id;
     result.data = memory::ConstByteView{
          reinterpret_cast< const std::byte * >( json_out_.c_str() ), json_out_.size() };
     return result;
}


std::string_view JsonToFlatBuffersConvertor::get_error_description() const
{
     return error_;
}


bool JsonToFlatBuffersConvertor::set_root_type( strings::StaticName type_id )
{
     const auto name = type_names_.get_name( type_id );
     if ( !name.empty() )
     {
          if ( !parser_.SetRootType( name.data() ) )
          {
               error_ = "unknown type name: ";
               error_ += name;
               return false;
          }
     }
     return true;
}

} // namespace _16nar::assets
