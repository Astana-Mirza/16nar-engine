/// @file
/// @brief Header file with JsonPropsWriterFactory class definition.
#ifndef _16NAR_TOOLS_JSON_PROPS_WRITER_FACTORY_H
#define _16NAR_TOOLS_JSON_PROPS_WRITER_FACTORY_H

#include <16nar/tools/assets/iprops_writer_factory.h>

namespace _16nar::tools
{

/// @brief Factory for making properties writers in JSON format.
class ENGINE_API JsonPropsWriterFactory : public IPropsWriterFactory
{
public:
    /// @copydoc IPropsWriterFactory::create_writer(const DataSchema&)
    std::shared_ptr< IPropsWriter > create_writer( const DataSchema& schema ) override;
};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_JSON_PROPS_WRITER_FACTORY_H
