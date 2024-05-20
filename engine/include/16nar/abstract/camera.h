/// @file
/// @brief Header file with Camera abstact class definition.
#ifndef _16NAR_CAMERA_H
#define _16NAR_CAMERA_H

#include <16nar/16nardefs.h>

namespace _16nar
{

class TransformMatrix;

/// @brief Camera defining a view which looks into the scene.
class ENGINE_API Camera
{
public:
     virtual ~Camera()                                           = default;
     virtual const TransformMatrix& get_transform_matr() const         = 0;
     virtual const TransformMatrix& get_inverse_transform_matr() const = 0;
};

} // namespace _16nar

#endif // #ifndef _16NAR_CAMERA_H
