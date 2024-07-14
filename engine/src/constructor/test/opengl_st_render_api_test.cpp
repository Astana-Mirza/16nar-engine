#include <16nar/render/opengl/render_api.h>

#include <16nar/system/window.h>
#include <16nar/logger/logger.h>
#include <16nar/logger/terminal_log_writer.h>
#include <16nar/render/camera_2d.h>
#include <16nar/render/ishader_program.h>
#include <16nar/math/math_functions.h>
#include <16nar/game.h>

#include <16nar/constructor/render/drawable_2d.h>
#include <16nar/constructor/render/qtree_render_system.h>
#include <16nar/constructor/render/quadrant.h>

#include <thread>
#include <chrono>

using namespace _16nar;

class TestDrawable : public constructor::Drawable2D
{
public:
     TestDrawable( const VertexBuffer& vertices, const Shader& shader,
          constructor::IRenderSystem2D *render_system ):
          Drawable2D( shader, render_system ), draw_info_{}, layer_{ 0 }
     {
          draw_info_.render_params.primitive = PrimitiveType::Triangles;
          draw_info_.render_params.vertex_buffer = vertices;
          draw_info_.render_params.vertex_count = 3;
          draw_info_.shader = shader;
     }

     virtual DrawInfo get_draw_info() const override
     {
          draw_info_.shader_setup = [ matr = model_matr_ ]( const IShaderProgram& shader )
          {
               shader.set_uniform( "model_matr", matr );
          };
          return draw_info_;
     }

     virtual FloatRect get_local_bounds() const override
     {
          return FloatRect{ Vec2f{ -10, -10 }, 10, 10 };
     }

     virtual FloatRect get_global_bounds() const override
     {
          return model_matr_ * get_local_bounds();
     }

     TransformMatrix& get_model()
     {
          return model_matr_;
     }

private:
     mutable DrawInfo draw_info_;
     TransformMatrix model_matr_;
     int layer_;
};


int main()
{
     const char *vertex_source = R"(
          #version 330 core
          layout (location = 0) in vec3 aPos;
          uniform mat4 view_matr;
          uniform mat4 model_matr;
          uniform mat4 proj_matr;
          void main()
          {
               vec4 pos = vec4(aPos.x, aPos.y, aPos.z, 1.0);
               gl_Position = proj_matr * view_matr * model_matr * pos;
          }
     )";

     const char *fragment_source = R"(
          #version 330 core
          out vec4 FragColor;
          void main()
          {
               FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
          } 
     )";

     const float vertices[] = {
          -10.0f, -10.0f, 0.0f,
          10.0f, -10.0f, 0.0f,
          0.0f,  10.0f, 0.0f
     };

     Logger::instance().add_writer( std::make_unique< TerminalLogWriter >() );
     Logger::instance().set_log_level( ILogWriter::LogLevel::Debug );

     LoadParams< ResourceType::VertexBuffer > vertex_buffer;
     LoadParams< ResourceType::Shader > shader;
     DrawInfo draw_info;

     vertex_buffer.buffer.data = DataSharedPtr{ new char[ sizeof( vertices ) ], std::default_delete< char[] >() };
     std::memcpy( vertex_buffer.buffer.data.get(), vertices, sizeof( vertices ) );
     vertex_buffer.buffer.size = sizeof( vertices );
     vertex_buffer.buffer.type = BufferType::StaticDraw;
     vertex_buffer.attributes.push_back( { 3, DataType::Float, false } );

     DataSharedPtr vertex_shader_ptr{ new char[ std::strlen( vertex_source ) + 1 ]{}, std::default_delete< char[] >()};
     std::memcpy( vertex_shader_ptr.get(), vertex_source, std::strlen( vertex_source ) );
     shader.shaders.push_back( { "", std::strlen( vertex_source ) + 1, vertex_shader_ptr, ShaderType::Vertex, true } );

     DataSharedPtr fragment_shader_ptr{ new char[ std::strlen( fragment_source ) + 1 ]{}, std::default_delete< char[] >()};
     std::memcpy( fragment_shader_ptr.get(), fragment_source, std::strlen( fragment_source ) );
     shader.shaders.push_back( { "", std::strlen( fragment_source ) + 1, fragment_shader_ptr, ShaderType::Fragment, true } );

     Game::init();

     // scope to close window and destroy api before game deinitialization
     {
          Window window( Vec2i{ 800, 600 }, "opengl_st_render_api_test" );
          Camera2D camera{ Vec2f{ 0, 0 }, 100, 100 };
          window.make_context_current();
          opengl::RenderApi api{ ProfileType::SingleThreaded };
          constructor::Quadrant quad{ FloatRect{ Vec2f{ -1000, -1000 }, 2000, 2000 } };

          constructor::QTreeRenderSystem render_system{ api, window };
          render_system.set_camera( &camera );
          render_system.set_root_quadrant( &quad );

          VertexBuffer vb_id = VertexBuffer( api.load( ResourceType::VertexBuffer, vertex_buffer ).id);
          Shader sh_id = Shader( api.load( ResourceType::Shader, shader ).id );

          TestDrawable obj{ vb_id, sh_id, &render_system };
          camera.move( Vec2f{ 0, 40 } );
          for ( std::size_t i = 0; i < 80; i++ )
          {
               render_system.clear_screen();
               render_system.select_objects();
               render_system.draw_objects();
               std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );

               camera.move( Vec2f{ 0, 1 } );
               camera.rotate( 10 );
               auto rect_pos = camera.get_global_bounds().get_pos();
               camera.zoom( 0.9 );

               obj.get_model().move( Vec2f{ 0, -2 } );
          }
     }

     Game::deinit();
     return 0;
}
