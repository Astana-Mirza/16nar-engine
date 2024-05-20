#include <16nar/system/exceptions.h>

#include <string>

namespace _16nar
{

ResourceException::ResourceException( ResID id ):
     std::runtime_error{ "resource operation failed for id " + std::to_string( id ) }
{}


 ResourceException::ResourceException( const std::string& msg, ResID id ):
     std::runtime_error{ msg + ( id ? std::to_string( id ) : "" ) }
{}


 ExceededIdException::ExceededIdException():
      ResourceException::ResourceException( "resource IDs exceeded" )
{}

} // namespace _16nar
