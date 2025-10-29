#include <16nar/tools/assets/flatbuffers/flatbuffers_props_writer.h>

#include <16nar/tools/assets/flatbuffers/flatbuffers_props_reader.h>

#include <flatbuffers/flexbuffers.h>


#define _16NAR_SET_FB_VAL( FUNC )                                \
     const auto& item = schema_.items.at( name );                \
     if ( !indices_.empty() && item.index < indices_.back() )    \
     {                                                           \
          if ( !unordered_builder_ )                             \
          {                                                      \
               unordered_builder_ = new flexbuffers::Builder();  \
               map_key_ = unordered_builder_->StartMap();        \
          }                                                      \
          unordered_builder_->FUNC( name.c_str(), value );       \
          return;                                                \
     }                                                           \
     indices_.emplace_back( item.index );                        \
     builder_->FUNC( value );


#define _16NAR_SET_FB_FIXED_TYPED_VECTOR( VALUE, SIZE )          \
     const auto& item = schema_.items.at( name );                \
     if ( !indices_.empty() && item.index < indices_.back() )    \
     {                                                           \
          if ( !unordered_builder_ )                             \
          {                                                      \
               unordered_builder_ = new flexbuffers::Builder();  \
               map_key_ = unordered_builder_->StartMap();        \
          }                                                      \
          unordered_builder_->FixedTypedVector( name.c_str(), VALUE, SIZE ); \
          return;                                                \
     }                                                           \
     indices_.emplace_back( item.index );                        \
     builder_->FixedTypedVector( VALUE, SIZE );


#define _16NAR_SET_FB_VECTOR( FUNC )                                       \
     const auto& item = schema_.items.at( name );                          \
     if ( !indices_.empty() && item.index < indices_.back() )              \
     {                                                                     \
          if ( !unordered_builder_ )                                       \
          {                                                                \
               unordered_builder_ = new flexbuffers::Builder();            \
               map_key_ = unordered_builder_->StartMap();                  \
          }                                                                \
          auto start = unordered_builder_->StartVector( name.c_str() );    \
          for ( const auto val : value )                                   \
          {                                                                \
               unordered_builder_->FUNC( val );                            \
          }                                                                \
          unordered_builder_->EndVector( start, true, false );             \
          return;                                                          \
     }                                                                     \
     indices_.emplace_back( item.index );                                  \
     auto start = builder_->StartVector();                                 \
     for ( const auto val : value )                                        \
     {                                                                     \
          builder_->FUNC( val );                                           \
     }                                                                     \
     builder_->EndVector( start, true, false );


namespace _16nar::tools
{

FlatBuffersPropsWriter::FlatBuffersPropsWriter( const DataSchema& schema ):
     indices_{}, schema_{ schema }, builder_{ new flexbuffers::Builder() },
     unordered_builder_{}, vector_key_{}, map_key_{}
{
     vector_key_ = builder_->StartVector();
     indices_.reserve( schema_.items.size() );
}


FlatBuffersPropsWriter::~FlatBuffersPropsWriter()
{
     if ( unordered_builder_ )
     {
          delete unordered_builder_;
     }
     delete builder_;
}


std::shared_ptr< IPropsReader > FlatBuffersPropsWriter::conver_to_reader()
{
     std::shared_ptr< FlatBuffersPropsReader > reader{};
     {
          const auto& buffer = finish_and_get_result();
          reader = std::make_shared< FlatBuffersPropsReader >(
               reinterpret_cast< const std::byte * >( buffer.data() ), buffer.size(), true );
     }
     delete builder_;
     builder_ = new flexbuffers::Builder();

     vector_key_ = builder_->StartVector();
     indices_.clear();

     const auto *buffer = finish_and_get_result_unordered();
     if ( buffer )
     {
          reader->set_unordered_buffer(
               reinterpret_cast< const std::byte * >( buffer->data() ), buffer->size(), true );
          delete unordered_builder_;
          unordered_builder_ = nullptr;
          map_key_ = 0;
     }
     return reader;
}


const FlatBuffersPropsWriter::Buffer& FlatBuffersPropsWriter::finish_and_get_result()
{
     builder_->Vector( indices_.data(), indices_.size() );
     builder_->EndVector( vector_key_, false, false );
     builder_->Finish();
     return builder_->GetBuffer();
}


const FlatBuffersPropsWriter::Buffer *FlatBuffersPropsWriter::finish_and_get_result_unordered()
{
     if ( unordered_builder_ )
     {
          unordered_builder_->EndMap( map_key_ );
          unordered_builder_->Finish();
          return &unordered_builder_->GetBuffer();
     }
     return nullptr;
}


void FlatBuffersPropsWriter::set_uint64( const std::string& name, uint64_t value )
{
     _16NAR_SET_FB_VAL( UInt );
}


void FlatBuffersPropsWriter::set_uint32( const std::string& name, uint32_t value )
{
     _16NAR_SET_FB_VAL( UInt );
}


void FlatBuffersPropsWriter::set_uint16( const std::string& name, uint16_t value )
{
     _16NAR_SET_FB_VAL( UInt );
}


void FlatBuffersPropsWriter::set_uint8( const std::string& name, uint8_t value )
{
     _16NAR_SET_FB_VAL( UInt );
}


void FlatBuffersPropsWriter::set_int64( const std::string& name, int64_t value )
{
     _16NAR_SET_FB_VAL( Int );
}


void FlatBuffersPropsWriter::set_int32( const std::string& name, int32_t value )
{
     _16NAR_SET_FB_VAL( Int );
}


void FlatBuffersPropsWriter::set_int16( const std::string& name, int16_t value )
{
     _16NAR_SET_FB_VAL( Int );
}


void FlatBuffersPropsWriter::set_int8( const std::string& name, int8_t value )
{
     _16NAR_SET_FB_VAL( Int );
}


void FlatBuffersPropsWriter::set_bool( const std::string& name, bool value )
{
     _16NAR_SET_FB_VAL( Bool );
}


void FlatBuffersPropsWriter::set_float( const std::string& name, float value )
{
     // use IndirectFloat here, because it may save space if other props fit in smaller width
     _16NAR_SET_FB_VAL( IndirectFloat );
}


void FlatBuffersPropsWriter::set_double( const std::string& name, double value )
{
     // use IndirectDouble here, because it may save space if other props fit in smaller width
     _16NAR_SET_FB_VAL( IndirectDouble );
}


void FlatBuffersPropsWriter::set_string( const std::string& name, const std::string& value )
{
     _16NAR_SET_FB_VAL( String );
}


void FlatBuffersPropsWriter::set_uint64_arr( const std::string& name, const std::vector< uint64_t >& value )
{
     _16NAR_SET_FB_VECTOR( UInt );
}


void FlatBuffersPropsWriter::set_uint32_arr( const std::string& name, const std::vector< uint32_t >& value )
{
     _16NAR_SET_FB_VECTOR( UInt );
}


void FlatBuffersPropsWriter::set_uint16_arr( const std::string& name, const std::vector< uint16_t >& value )
{
     _16NAR_SET_FB_VECTOR( UInt );
}


void FlatBuffersPropsWriter::set_uint8_arr( const std::string& name, const std::vector< uint8_t >& value )
{
     _16NAR_SET_FB_VECTOR( UInt );
}


void FlatBuffersPropsWriter::set_int64_arr( const std::string& name, const std::vector< int64_t >& value )
{
     _16NAR_SET_FB_VECTOR( Int );
}


void FlatBuffersPropsWriter::set_int32_arr( const std::string& name, const std::vector< int32_t >& value )
{
     _16NAR_SET_FB_VECTOR( Int );
}


void FlatBuffersPropsWriter::set_int16_arr( const std::string& name, const std::vector< int16_t >& value )
{
     _16NAR_SET_FB_VECTOR( Int );
}


void FlatBuffersPropsWriter::set_int8_arr( const std::string& name, const std::vector< int8_t >& value )
{
     _16NAR_SET_FB_VECTOR( Int );
}


void FlatBuffersPropsWriter::set_bool_arr( const std::string& name, const std::vector< bool >& value )
{
     _16NAR_SET_FB_VECTOR( Bool );
}


void FlatBuffersPropsWriter::set_float_arr( const std::string& name, const std::vector< float >& value )
{
     // don't use IndirectFloat here, it makes no sense since we are storing array of doubles only
     _16NAR_SET_FB_VECTOR( Float );
}


void FlatBuffersPropsWriter::set_double_arr( const std::string& name, const std::vector< double >& value )
{
     // don't use IndirectDouble here, it makes no sense since we are storing array of doubles only
     _16NAR_SET_FB_VECTOR( Double );
}


void FlatBuffersPropsWriter::set_string_arr( const std::string& name, const std::vector< std::string >& value )
{
     _16NAR_SET_FB_VECTOR( Key );
}


void FlatBuffersPropsWriter::set_vec2f( const std::string& name, const Vec2f& value )
{
     _16NAR_SET_FB_FIXED_TYPED_VECTOR( value.data(), Vec2f::size );
}


void FlatBuffersPropsWriter::set_vec3f( const std::string& name, const Vec3f& value )
{
     _16NAR_SET_FB_FIXED_TYPED_VECTOR( value.data(), Vec3f::size );
}


void FlatBuffersPropsWriter::set_vec4f( const std::string& name, const Vec4f& value )
{
     _16NAR_SET_FB_FIXED_TYPED_VECTOR( value.data(), Vec4f::size );
}


void FlatBuffersPropsWriter::set_vec2i( const std::string& name, const Vec2i& value )
{
     _16NAR_SET_FB_FIXED_TYPED_VECTOR( value.data(), Vec2i::size );
}


void FlatBuffersPropsWriter::set_vec3i( const std::string& name, const Vec3i& value )
{
     _16NAR_SET_FB_FIXED_TYPED_VECTOR( value.data(), Vec3i::size );
}


void FlatBuffersPropsWriter::set_vec4i( const std::string& name, const Vec4i& value )
{
     _16NAR_SET_FB_FIXED_TYPED_VECTOR( value.data(), Vec4i::size );
}


void FlatBuffersPropsWriter::set_float_rect( const std::string& name, const FloatRect& value )
{
     typename Vec2f::type valarr[ 2 * Vec2f::size ] = { value.get_pos().x(), value.get_pos().y(),
          value.get_end().x(), value.get_end().y() };
     _16NAR_SET_FB_FIXED_TYPED_VECTOR( valarr, 2 * Vec2f::size );
}


void FlatBuffersPropsWriter::set_int_rect( const std::string& name, const IntRect& value )
{
     typename Vec2i::type valarr[ 2 * Vec2i::size ] = { value.get_pos().x(), value.get_pos().y(),
          value.get_end().x(), value.get_end().y() };
     _16NAR_SET_FB_FIXED_TYPED_VECTOR( valarr, 2 * Vec2i::size );
}


void FlatBuffersPropsWriter::set_resource_index( const std::string& name, ResourceIndex value )
{
     uint16_t valarr[ 2 ] = { value.package, value.resource };
     _16NAR_SET_FB_FIXED_TYPED_VECTOR( valarr, 2 );
}

} // namespace _16nar::tools
