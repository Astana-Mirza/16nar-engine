#include <16nar/tools/resource_pkg_visitor.h>

#include <16nar/tools/asset_iterator.h>

namespace _16nar::tools
{

void ResourcePkgVisitor::visit( AssetActionId, const IAssetReader& )
{
     // do nothing by default, this function should be overriden in derived classes
}


void ResourcePkgVisitor::process_resource_package( IAssetReader& reader )
{
     AssetIterator iter{ *this, reader };
     iter
     .check_array( true )
     .call( Actions::PackageBegin )
     .for_each( [ &iter ]()
     {
          // optionally, asset metadata may be stored directly in this array,
          // but it can also be a regular resource within the package
          // also, some formats (JSON) are able to store it only here
          iter.call( Actions::ResourceEnter );
     } )
     .call( Actions::PackageEnd );
}


void ResourcePkgVisitor::process_resource( IAssetReader& reader )
{
     AssetIterator iter{ *this, reader };
     iter
     .call( Actions::ResourceBegin )
     .call( Actions::ResourceEnter )
     .call( Actions::ResourceEnd );
}

} // namespace _16nar::tools
