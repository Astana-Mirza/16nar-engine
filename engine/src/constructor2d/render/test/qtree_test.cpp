#include <catch2/catch_test_macros.hpp>

#include <16nar/constructor2d/render/drawable_2d.h>
#include <16nar/constructor2d/render/quadrant.h>
#include <16nar/constructor2d/render/qtree_render_system.h>

namespace
{

class MockDrawable2D : public _16nar::constructor2d::Drawable2D
{
public:
     MockDrawable2D( const _16nar::FloatRect& rect,
          const _16nar::Shader& shader,
          _16nar::constructor2d::IRenderSystem2D& render_system ):
          _16nar::constructor2d::Drawable2D( shader, render_system ),
          rect_{ rect } {}

     virtual _16nar::DrawInfo get_draw_info() const override
     {
          return _16nar::DrawInfo{};
     }

     virtual _16nar::FloatRect get_local_bounds() const override
     {
          return rect_;
     }

     virtual _16nar::FloatRect get_global_bounds() const override
     {
          return _16nar::FloatRect{ rect_.get_pos() * 2.0f, 10.0f, 10.0f };
     }

private:
     _16nar::FloatRect rect_;
};


TEST_CASE( "Quadrant object detection", "[qtree_render_system]" )
{
     _16nar::constructor2d::QTreeRenderSystem render_system{};
     MockDrawable2D obj1{
          _16nar::FloatRect{ { 50.0f, 50.0f }, 10.0f, 10.0f },
          _16nar::Shader{}, render_system
     };
     _16nar::constructor2d::Quadrant quad{ _16nar::FloatRect{ { 0.0f, 0.0f }, 100.0f, 100.0f } };
}

} // anonymous namespace
