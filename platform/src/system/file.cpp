#include <16nar/platform/system/file.h>

namespace _16nar::system
{

File::File() noexcept:
     handle_{}
{}


File::File( File&& other ) noexcept:
     handle_{ other.handle_ }
{
     other.handle_ = nullptr;
}


File& File::operator=( File&& rhs ) noexcept
{
     if ( this != &rhs )
     {
          close();
          handle_ = rhs.handle_;
          rhs.handle_ = nullptr;
     }
     return *this;
}


File::~File()
{
     close();
}


bool File::open( const std::filesystem::path& path, bool write ) noexcept
{
     if ( is_open() )
     {
          close();
     }
     handle_ = std::fopen( path.string().c_str(), write ? "wb+" : "rb" );
     return is_open();
}


bool File::is_open() const noexcept
{
     return handle_ != nullptr;
}


void File::close() noexcept
{
     if ( is_open() )
     {
          std::fclose( handle_ );
     }
}


std::size_t File::tell() const noexcept
{
     return is_open() ? static_cast< std::size_t >( std::ftell( handle_ ) ) : 0;
}


bool File::seek( std::size_t offset, SeekOrigin origin ) noexcept
{
     return is_open() ? ( 0 == std::fseek( handle_, offset, origin ) ) : false;
}


std::size_t File::write( memory::ConstByteView buffer ) noexcept
{
     return is_open() ? std::fwrite( buffer.data, buffer.size, 1, handle_ ) * buffer.size : 0;
}


std::size_t File::read( memory::ByteView buffer ) const noexcept
{
     return is_open() ? std::fread( buffer.data, buffer.size, 1, handle_ ) * buffer.size : 0;
}


std::size_t File::get_size() const noexcept
{
     if ( !is_open() )
     {
          return 0;
     }
     const auto current = tell();
     if ( current == static_cast< std::size_t >( -1L )
          || 0 != std::fseek( handle_, 0, SEEK_END ) )
     {
          return 0;
     }
     const auto ret = tell();
     if ( 0 != std::fseek( handle_, static_cast< long >( current ), SEEK_SET ) )
     {
          return 0;
     }
     return ret;
}

} // namespace _16nar::system
