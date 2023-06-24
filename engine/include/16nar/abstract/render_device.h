/// @file
/// @brief Header file with RenderDevice interface definition.
#ifndef _16NAR_RENDER_DEVICE_H
#define _16NAR_RENDER_DEVICE_H

#include <16nar/16nardefs.h>
#include <16nar/render/view.h>

namespace _16nar
{


/// @brief Interface for low-level rendering. It deals only with raw vertex data.
/// @detail The device has buffer in which it stores objects to be rendered. When
/// the frame has to be drawn, RenderDevice flushes its buffer, i.e. renders saved
/// data on the target.
class ENGINE_API RenderDevice
{
public:
     /// @brief Type of device data, defining meaning of render command.
     enum class DeviceDataType
     {
          RenderData,     ///< data for rendering.
          View            ///< change of a view.
     };

     /// @brief Structure for generic render commands.
     struct DeviceData
     {
          DeviceDataType type;     ///< type of data.
          union
          {
               RenderData render_data;    ///< data for rendering.
               View view;                 ///< view to be set.
          };
     };

     virtual ~RenderDevice() = default;

     /// @brief Add data for rendering.
     /// @detail Data will be rendered in order of adding,
     /// it may be significant if drawing with multiple layers.
     /// @param[in] data render data.
     virtual void add_data( const DeviceData& data ) = 0;

     /// @brief End one render frame, not neccessarily render it (for render, use flush).
     virtual void end_frame() = 0;

     /// @brief Flush all saved render data, which means that data will be rendered.
     virtual void flush() = 0;
};

} // namespace _16nar

#endif  // #ifndef _16NAR_RENDER_DEVICE_H