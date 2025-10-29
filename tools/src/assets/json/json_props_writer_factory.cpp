#include <16nar/tools/assets/json/json_props_writer_factory.h>

#include <16nar/tools/assets/json/json_props_writer.h>

namespace _16nar::tools
{

std::shared_ptr< IPropsWriter > JsonPropsWriterFactory::create_writer( const DataSchema& )
{
     return std::make_shared< JsonPropsWriter >();
}

} // namespace _16nar::tools
