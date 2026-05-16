#include <16nar/core/resources/resource_pkg_visitor.h>

#include <16nar/core/assets/asset_iterator.h>

namespace _16nar::resources
{

void ResourcePkgVisitor::visit( assets::AssetActionId, const assets::IAssetReader& )
{
     // do nothing by default, this function should be overriden in derived classes
}


void ResourcePkgVisitor::process_resource_package( assets::IAssetReader& reader )
{
     assets::AssetIterator iter{ *this, reader };
     iter
     .check_array( true )
     .call( Actions::PackageBegin )
     .for_each( [ &iter ]()
     {
          // optionally, asset metadata may be stored directly in this array,
          // but it can also be a regular resource within the package
          iter.call( Actions::ResourceEnter );
     } )
     .call( Actions::PackageEnd );
}


void ResourcePkgVisitor::process_resource( assets::IAssetReader& reader )
{
     assets::AssetIterator iter{ *this, reader };
     iter
     .call( Actions::ResourceBegin )
     .call( Actions::ResourceEnter )
     .call( Actions::ResourceEnd );
}

} // namespace _16nar::resources
