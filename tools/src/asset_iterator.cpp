#include <16nar/tools/asset_iterator.h>

namespace _16nar::tools
{

AssetIterator::AssetIterator( IAssetVisitor& visitor, IAssetReader& reader ):
     visitor_{ visitor }, reader_{ reader }
{}


IAssetReader& AssetIterator::get_reader()
{
     return reader_;
}


AssetIterator& AssetIterator::check_array( bool is_array )
{
     if ( reader_.is_array() != is_array )
     {
          throw std::runtime_error{ std::string{ "checked asset must " }
               + ( is_array ? "" : "not " ) + "be an array" };
     }
     return *this;
}


AssetIterator& AssetIterator::call( AssetActionId action_id )
{
     visitor_.visit( action_id, get_reader() );
     return *this;
}


AssetIterator& AssetIterator::go( std::string_view name )
{
     if ( !reader_.to_child( name ) )
     {
          throw std::runtime_error{ "cannot go to child asset: "
               + std::string( name.data(), name.size() ) };
     }
     return *this;
}


bool AssetIterator::go_opt( std::string_view name )
{
     return reader_.to_child( name );
}


AssetIterator& AssetIterator::up()
{
     if ( !reader_.to_parent() )
     {
          throw std::runtime_error{ "cannot go to parent asset" };
     }
     return *this;
}

} // namespace _16nar::tools
