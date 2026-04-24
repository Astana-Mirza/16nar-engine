/// @file
/// @brief File with IResourceLoader interface definition.
#ifndef _16NAR_TOOLS_IRESOURCE_LOADER_H
#define _16NAR_TOOLS_IRESOURCE_LOADER_H

#include <16nar/tools/defs.h>

namespace _16nar::tools
{

class ENGINE_API IResourceLoader
{
public:
     /// @brief Load resource and pass it to consumer.
     /// @param[in] asset asset with resource description.
     virtual void load(  ) = 0;


};

} // namespace _16nar::tools

#endif // #ifndef _16NAR_TOOLS_IRESOURCE_LOADER_H
