/// @file
/// @brief File with SceneData template class definition and its specializations.
#ifndef _16NAR_SCENE_DATA_H
#define _16NAR_SCENE_DATA_H

#include <base_data.h>

#include <string>
#include <memory>

/// @brief Class for representing any data, that can be stored in the scene.
/// @tparam T type of stored data structure.
template < typename T >
class SceneData : public BaseData
{
public:
     /// @brief Constructor.
     /// @param data data to be stored.
     SceneData( const T& data ) : data_{ data } {}


     /// @brief Gets the size of stored data.
     uint32_t size() const
     {
          return sizeof( data_ );
     }


     /// @brief Write stored binary data to stream.
     /// @param os output stream where data will be written.
     void write_binary( std::ostream& os ) const
     {
          os.write( reinterpret_cast< const char * >( &data_ ), size() );
     }

private:
     T data_;            ///< stored data structure.
};


/// @brief Specialization of SceneData template class for std::string, needed because strings are not plain data.
template <>
class SceneData< std::string > : public BaseData
{
public:
     /// @brief Constructor.
     /// @param data data to be stored.
     SceneData( const std::string& data ) : data_{ data } {}


     /// @brief Gets the size of stored data.
     uint32_t size() const
     {
          return static_cast< uint32_t >( data_.size() ) + 1;      // +1 for '\0'
     }


     /// @brief Write stored binary data to stream.
     /// @param os output stream where data will be written.
     void write_binary( std::ostream& os ) const
     {
          os.write( data_.data(), data_.size() );
          os.put( '\0' );
     }

private:
     std::string data_;      ///< stored string.
};


/// @brief Specialization of SceneData template class for arrays.
template < typename T >
class SceneData< T[] > : public BaseData
{
public:
     /// @brief Constructor.
     /// @param count count of elements of the array.
     SceneData( uint32_t count ):
          data_{ std::make_shared_for_overwrite< T[] >( count ) },
          size_{ count * sizeof( T ) } {}


     /// @brief Gets stored array.
     const std::shared_ptr< T[] >& get_data() const
     {
          return data_;
     }

     /// @brief Gets the size of stored data.
     uint32_t size() const
     {
          return size_;
     }


     /// @brief Write stored binary data to stream.
     /// @param os output stream where data will be written.
     void write_binary( std::ostream& os ) const
     {
          os.write( reinterpret_cast< const char * >( data_.get() ), size_ );
     }

private:
     std::shared_ptr< T[] > data_;      ///< pointer to array of elements.
     uint32_t size_;                    ///< size of entire array in bytes.
};

#endif // #ifndef _16NAR_SCENE_DATA_H
