/// @file
/// File with BaseData abstract class definition.
#ifndef _16NAR_BASE_DATA_H
#define _16NAR_BASE_DATA_H

#include <QFile>

/// Abstract class for representing different data in scene compiler.
class BaseData
{
public:
     virtual ~BaseData() = default;

     /// Gets the size of stored data.
     virtual uint32_t size() const = 0;

     /// Write stored binary data to file.
     /// @param out_file file where data will be written.
     virtual void write_binary( QFile& out_file ) const = 0;
};

#endif // #ifndef _16NAR_BASE_DATA_H