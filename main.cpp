#include <QApplication>
#include "vokoplay.h"

#include <QDir>

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    QString filename = argv[ 1 ];
    
    // Überprüfen ob Videodatei im selben Ordner ist wie das Programm
    QDir dir;
    QFile file( dir.dirName() + QDir::separator() + filename );
    if ( file.exists() == true )
    {
      filename = dir.currentPath() + QDir::separator() + filename;
    }
    
    vokoplay player;
    player.setVideoName( filename );
    player.prepare();
    player.show();
    
    return app.exec();
}
