#include <16nar/render/opengl/render_api.h>

#include <16nar/system/window.h>
#include <16nar/logger/logger.h>
#include <16nar/logger/terminal_log_writer.h>
#include <16nar/game.h>

#include <thread>
#include <chrono>

int main()
{
     using namespace _16nar;

     const char *vertex_source = R"(
          #version 330 core
          layout (location = 0) in vec3 aPos;
          void main()
          {
               gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
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
          -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
          0.0f,  0.5f, 0.0f
     };

     Logger::instance().add_writer( std::make_unique< TerminalLogWriter >() );
     Logger::instance().set_log_level( ILogWriter::LogLevel::Debug );

     LoadParams< ResourceType::VertexBuffer > vertex_buffer;
     LoadParams< ResourceType::Shader > shader;
     RenderParams render_params;

     vertex_buffer.buffer.data = &vertices[ 0 ];
     vertex_buffer.buffer.size = sizeof( vertices );
     vertex_buffer.buffer.type = BufferType::StaticDraw;
     vertex_buffer.attributes.push_back( { 3, DataType::Float, false } );

     shader.shaders.push_back( { "", sizeof( vertex_source ), vertex_source, ShaderType::Vertex, true } );
     shader.shaders.push_back( { "", sizeof( fragment_source ), fragment_source, ShaderType::Fragment, true } );

     Game::init();

     // scope to close window and destroy api before game deinitialization
     {
          Window window( Vec2i{ 800, 600 }, "opengl_st_render_api_test" );
          window.make_context_current();
          opengl::RenderApi api{ ProfileType::SingleThreaded };

          VertexBuffer vb_id = VertexBuffer( api.load( ResourceType::VertexBuffer, vertex_buffer ).id );
          Shader sh_id = Shader( api.load( ResourceType::Shader, shader ).id );

          render_params.primitive = PrimitiveType::Triangles;
          render_params.vertex_buffer = vb_id;
          render_params.vertex_count = 3;

          api.get_device().bind_shader( sh_id );
          api.get_device().render( render_params );

          window.swap_buffers();
          std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
     }

     Game::deinit();
     return 0;
}
