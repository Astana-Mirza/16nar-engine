#include <catch2/catch_test_macros.hpp>

#include <16nar/render/camera_2d.h>
#include <16nar/constructor2d/render/drawable_2d.h>
#include <16nar/constructor2d/render/quadrant.h>
#include <16nar/constructor2d/render/qtree_render_system.h>

namespace
{

class MockDrawable2D : public _16nar::constructor2d::Drawable2D
{
public:
     MockDrawable2D( const _16nar::FloatRect& rect,
          const _16nar::Shader& shader ):
          _16nar::constructor2d::Drawable2D( shader ),
          rect_{ rect } {}

     virtual _16nar::DrawInfo get_draw_info() const noexcept override
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

     _16nar::FloatRect rect_;
};


TEST_CASE( "Quadrant object detection", "[qtree_render_system]" )
{
     _16nar::constructor2d::QTreeRenderSystem render_system{};
     _16nar::Camera2D camera{ { 55.0f, 55.0f }, 20.0f, 20.0f };
     auto root = std::make_unique< _16nar::constructor2d::Quadrant >( _16nar::FloatRect{ { 0.0f, 0.0f }, 100.0f, 100.0f } );
     root->add_child( std::make_unique< _16nar::constructor2d::Quadrant >( _16nar::FloatRect{ { 0.0f, 0.0f }, 50.0f, 50.0f } ), 0 );
     root->add_child( std::make_unique< _16nar::constructor2d::Quadrant >( _16nar::FloatRect{ { 50.0f, 0.0f }, 50.0f, 50.0f } ), 1 );
     root->add_child( std::make_unique< _16nar::constructor2d::Quadrant >( _16nar::FloatRect{ { 0.0f, 50.0f }, 50.0f, 50.0f } ), 2 );
     root->add_child( std::make_unique< _16nar::constructor2d::Quadrant >( _16nar::FloatRect{ { 50.0f, 50.0f }, 50.0f, 50.0f } ), 3 );

     render_system.set_root_quadrant( std::move( root ) );
     render_system.set_camera( &camera );

     // scope because Drawable2D must be destroyed before render system
     {
          MockDrawable2D obj1{
               _16nar::FloatRect{ { 30.0f, 30.0f }, 10.0f, 10.0f },
               _16nar::Shader{}
          };
          obj1.set_render_system( &render_system );

          const auto& quad3_drawables = render_system.get_root_quadrant()->get_children()[ 3 ]->get_draw_children();
          REQUIRE( quad3_drawables.find( &obj1 ) != quad3_drawables.cend() );

          _16nar::constructor2d::Quadrant::LayerMap layers{};
          render_system.get_root_quadrant()->find_objects( camera.get_global_bounds(), layers );
          REQUIRE( !layers.empty() );
          REQUIRE( layers[ 0 ].find( &obj1 ) != layers[ 0 ].cend() );

          obj1.rect_ = _16nar::FloatRect{ { 5.0f, 10.0f }, 10.0f, 10.0f };
          render_system.handle_change( &obj1 );
          const auto& quad0_drawables = render_system.get_root_quadrant()->get_children()[ 0 ]->get_draw_children();
          REQUIRE( quad0_drawables.find( &obj1 ) != quad0_drawables.cend() );

          obj1.rect_ = _16nar::FloatRect{ { 22.0f, 23.0f }, 10.0f, 10.0f };
          render_system.handle_change( &obj1 );
          const auto& quad_drawables = render_system.get_root_quadrant()->get_draw_children();
          REQUIRE( quad_drawables.find( &obj1 ) != quad_drawables.cend() );

          obj1.rect_ = _16nar::FloatRect{ { 30.0f, 10.0f }, 10.0f, 10.0f };
          render_system.handle_change( &obj1 );
          const auto& quad1_drawables = render_system.get_root_quadrant()->get_children()[ 1 ]->get_draw_children();
          REQUIRE( quad1_drawables.find( &obj1 ) != quad1_drawables.cend() );
     }
}

} // anonymous namespace
