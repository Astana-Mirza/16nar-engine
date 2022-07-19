#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

#include <compiler.h>

void usage( const QString& prog_name )
{
     QTextStream( stdout ) << QObject::tr( "Usage:\n" ) << prog_name
                           << QObject::tr(" OPTION <input_file> <output_file>\n\n" )
                           << QObject::tr( "Options:\n\t-s\tcompile a scene,\n" )
                           << QObject::tr( "\t-r\tcompile a resource package." ) << Qt::endl;
}


int main( int argc, char *argv[] )
{
     QCoreApplication a( argc, argv );

     QTranslator translator;
     const QStringList uiLanguages = QLocale::system().uiLanguages();
     for ( const QString &locale : uiLanguages )
     {
          const QString baseName = "scene-compiler_" + QLocale( locale ).name();
          if ( translator.load( ":/i18n/" + baseName ) )
          {
               a.installTranslator( &translator );
               break;
          }
     }

     QStringList arg_list = a.arguments();
     if ( arg_list.size() != 4 )
     {
          usage( arg_list.at( 0 ) );
          return 1;
     }
     try
     {
          Compiler compiler( arg_list[ 2 ] );
          if ( arg_list[ 1 ] == "-r" )
          {
               compiler.compile_package( arg_list[ 3 ] );
          }
          else if ( arg_list[ 1 ] == "-s" )
          {
               compiler.compile_scene( arg_list[ 3 ] );
          }
          else
          {
               usage( arg_list.at( 0 ) );
               return 1;
          }
     }
     catch ( std::exception& ex )
     {
          QTextStream( stderr ) << QObject::tr( "Error: " ) << ex.what() << Qt::endl;
          return 1;
     }

     return 0;
}
