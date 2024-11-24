#include <16nar/render/opengl/vertex_buffer_loader.h>

#include <16nar/render/opengl/glad.h>

#include <16nar/system/exceptions.h>
#include <16nar/logger/logger.h>

namespace _16nar::opengl
{

bool VertexBufferLoader::load( const ResourceManagerMap&,
     const LoadParamsType& params, HandlerType& handler )
{
     unsigned int buffers[ 2 ] = { 0, 0 };
     glGenVertexArrays( 1, &handler.vao_descriptor );
     glGenBuffers( ( params.index_buffer.data != nullptr ) ? 2 : 1, &buffers[ 0 ] );
     handler.vbo_descriptor = buffers[ 0 ];

     glBindVertexArray( handler.vao_descriptor );
     glBindBuffer( GL_ARRAY_BUFFER, handler.vbo_descriptor );
     glBufferData( GL_ARRAY_BUFFER, params.buffer.size, params.buffer.data.get(),
          buffer_type_to_int( params.buffer.type ) );
     if ( params.index_buffer.data != nullptr )
     {
          handler.ebo_descriptor = buffers[ 1 ];
          glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, handler.ebo_descriptor );
          glBufferData( GL_ELEMENT_ARRAY_BUFFER, params.index_buffer.size, params.index_buffer.data.get(),
               buffer_type_to_int( params.index_buffer.type ) );
     }

     std::size_t offset = 0;
     for ( std::size_t i = 0; i < params.attributes.size(); i++ )
     {
          const auto& attr = params.attributes[ i ];
          glVertexAttribPointer( i, attr.size, data_type_to_int( attr.data_type ),
               attr.normalized ? GL_TRUE : GL_FALSE, 0, reinterpret_cast< void * >( offset ) );
          glEnableVertexAttribArray( i );
          offset += attr.size * ( attr.data_type == DataType::Byte ? sizeof( std::uint8_t ) : sizeof( float ) );
     }

     glBindVertexArray( 0 );
     if ( params.index_buffer.data != nullptr )
     {
          glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
     }
     glBindBuffer( GL_ARRAY_BUFFER, 0 );

     LOG_16NAR_DEBUG( "Vertex buffer " << handler.vao_descriptor << " was loaded" );
     return true;
}


bool VertexBufferLoader::unload( const HandlerType& handler )
{
     unsigned int buffers[ 2 ] = { handler.vbo_descriptor, handler.ebo_descriptor };
     glDeleteBuffers( handler.ebo_descriptor != 0 ? 2 : 1, &buffers[ 0 ] );
     glDeleteVertexArrays( 1, &handler.vao_descriptor );
     LOG_16NAR_DEBUG( "Vertex buffer " << handler.vao_descriptor << " was unloaded" );
     return true;
}

} // namespace _16nar::opengl
