/// @file
/// @brief Header file with IPropsWriterFactory interface definition.
#ifndef _16NAR_TOOLS_IPROPS_WRITER_FACTORY_H
#define _16NAR_TOOLS_IPROPS_WRITER_FACTORY_H

#include <16nar/16nardefs.h>
#include <16nar/tools/data_schema.h>
#include <16nar/tools/assets/iprops_writer.h>

#include <string>

namespace _16nar::tools
{

/// @brief Interface of factory for making properties writers.
class ENGINE_API IPropsWriterFactory
{
public:
     /// @brief Virtual default destructor.
     virtual ~IPropsWriterFactory() = default;

     /// @brief Create properties writer.
     /// @param[in] schema schema of data to be written with created writer.
     /// @return properties writer.
     virtual std::shared_ptr< IPropsWriter > create_writer( const DataSchema& schema ) = 0;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IPROPS_WRITER_FACTORY_H
