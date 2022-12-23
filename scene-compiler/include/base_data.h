/// @file
/// @brief File with BaseData abstract class definition.
#ifndef _16NAR_BASE_DATA_H
#define _16NAR_BASE_DATA_H

#include <ostream>

/// @brief Abstract class for representing different data in scene compiler.
class BaseData
{
public:
     virtual ~BaseData() = default;

     /// @brief Gets the size of stored data.
     virtual uint32_t size() const = 0;

     /// @brief Write stored binary data to stream.
     /// @param os output stream where data will be written.
     virtual void write_binary( std::ostream& os ) const = 0;
};

#endif // #ifndef _16NAR_BASE_DATA_H