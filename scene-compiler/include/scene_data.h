/// @file
/// File with SceneData template class definition and its specialization for QStrings.
#ifndef _16NAR_SCENE_DATA_H
#define _16NAR_SCENE_DATA_H

#include <base_data.h>
#include <QString>

/// Class for representing any data, that can be stored in the scene.
/// @tparam T type of stored data structure.
template < typename T >
class SceneData : public BaseData
{
public:
     /// Constructor.
     /// @param data data to be stored.
     SceneData( const T& data ) : data_{ data } {}


     /// Gets the size of stored data.
     uint32_t size() const
     {
          return sizeof( data_ );
     }


     /// Write stored binary data to file.
     /// @param out_file file where data will be written.
     void write_binary( QFile& out_file ) const
     {
          out_file.write( reinterpret_cast< char * >( &data_ ), size() );
     }

private:
     T data_;            ///< stored data structure.
};


/// Specialization of SceneData template class for QString, needed because strings are not plain data.
template <>
class SceneData< QString > : public BaseData
{
public:
     /// Constructor.
     /// @param data data to be stored.
     SceneData( const QString& data ) : data_{ data + '\n' } {}


     /// Gets the size of stored data.
     uint32_t size() const
     {
          return data_.size() + 1;      // +1 for '\0'
     }


     /// Write stored binary data to file.
     /// @param out_file file where data will be written.
     void write_binary( QFile& out_file ) const
     {
          out_file.write( data_.toStdString().data(), data_.size() );
          out_file.putChar( '\0' );
     }

private:
     QString data_;      ///< stored string.
};

#endif // #ifndef _16NAR_SCENE_DATA_H
