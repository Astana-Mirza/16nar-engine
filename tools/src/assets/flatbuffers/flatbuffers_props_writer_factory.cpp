#include <16nar/tools/assets/flatbuffers/flatbuffers_props_writer_factory.h>

#include <16nar/tools/assets/flatbuffers/flatbuffers_props_writer.h>

namespace _16nar::tools
{

std::shared_ptr< IPropsWriter > FlatBuffersPropsWriterFactory::create_writer( const DataSchema& schema )
{
     return std::make_shared< FlatBuffersPropsWriter >( schema );
}

} // namespace _16nar::tools
