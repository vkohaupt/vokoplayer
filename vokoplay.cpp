#include "vokoplay.h"
#include "QvkVideoSurface.h"

#include <QKeyEvent>
#include <QDropEvent>
#include <QSettings>
#include <QPalette>

vokoplay::vokoplay()
{
  setAcceptDrops( true );
}

void vokoplay::prepare()
{
  myUi.setupUi( this );

  // Read from file VERSION progname and versionsnumber
  QSettings versionSettings(":/VERSION", QSettings::IniFormat );
  versionSettings.beginGroup("Info");
    bool beta = versionSettings.value( "Beta" ).toBool();
    QString Beta;
    if ( beta )
      Beta = " Beta"; 
    else
      Beta = "";
 
    QString progName = versionSettings.value( "Progname" ).toString();
    QString Version = versionSettings.value( "Version" ).toString() + Beta;
  versionSettings.endGroup();

  setWindowTitle( progName + " " + Version );
  
  QSize IconSize = QSize( 50, 50 );
  
  QIcon myIcon = QIcon::fromTheme( "preferences-system", QIcon( ":/pictures/save.png" ) );
  myUi.pushButton->setIcon( myIcon);
  myUi.pushButton->setIconSize( IconSize );
  myUi.pushButton->setText( "" );
  
  myIcon = QIcon::fromTheme( "view-fullscreen", QIcon( ":/pictures/save.png" ) );
  myUi.pushButton_1->setIcon( myIcon);
  myUi.pushButton_1->setIconSize( IconSize );
  myUi.pushButton_1->setText( "" );
  
  myUi.videoBackgroundWidget->setStyleSheet( "background-color: black;" );
  
  myUi.videoLabel->setAlignment( Qt::AlignCenter );
  myUi.videoLabel->move( 0, 0 );
  myUi.videoLabel->resize( QSize( myUi.videoBackgroundWidget->size() ) );
  
  player = new QMediaPlayer;
  //connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
  connect( player, SIGNAL( stateChanged( QMediaPlayer::State ) ), this, SLOT( playerStateChanged( QMediaPlayer::State ) ) );
  player->setMedia( QUrl::fromLocalFile( getVideoName() ) );
  
  QvkVideoSurface *videoSurface = new QvkVideoSurface( this );
  connect( videoSurface, SIGNAL( newPicture( QImage ) ), this, SLOT( setNewImage( QImage ) ) );
  player->setVideoOutput( videoSurface );

  connect( myUi.play, SIGNAL( clicked() ), SLOT( play() ) );
  connect( myUi.pause, SIGNAL( clicked() ), SLOT( pause() ) );
  connect( myUi.mute, SIGNAL( clicked() ), SLOT( mute() ) );
  myUi.mute->setText( "" );
  myUi.mute->setIcon( QIcon::fromTheme( "audio-volume-medium", QIcon( ":/pictures/save.png" ) ) );
  connect( myUi.horizontalSlider, SIGNAL( sliderMoved( int ) ), SLOT( sliderMoved( int ) ) );

  play();
  
}

vokoplay::~vokoplay()
{
}

void vokoplay::play()
{
  myUi.play->setEnabled( false );
  myUi.pause->setEnabled( true );
  player->play();
}

void vokoplay::pause()
{
  myUi.play->setEnabled( true );
  myUi.pause->setEnabled( false );
  player->pause(); 
}

void vokoplay::mute()
{
  if ( player->isMuted() == false )
  {
    myUi.mute->setIcon( QIcon::fromTheme( "audio-volume-muted", QIcon( ":/pictures/save.png" ) ) );
    player->setMuted( true );
  }
  else
  {
    player->setMuted( false );
    myUi.mute->setIcon( QIcon::fromTheme( "audio-volume-medium", QIcon( ":/pictures/save.png" ) ) );
  }
}

void vokoplay::sliderMoved( int position )
{
  qDebug() << position;
  player->setPosition( position ); //qint64
}

// Wenn sich Playerstats ändernt dann länge vom Video ermitteln
void vokoplay::playerStateChanged( QMediaPlayer::State state )
{
  if ( state == QMediaPlayer::PlayingState )
    qDebug() << "länge des Videos" << player->duration()/1000 << "Sekunden";
}

void vokoplay::setVideoName( QString value )
{
  videoName = value;
}

QString vokoplay::getVideoName()
{
  return videoName; 
}

void vokoplay::mouseDoubleClickEvent( QMouseEvent *event )
{
  (void) event;
  if ( isFullScreen() == true )
  {
    myUi.sideBarFrame->show();
    myUi.frame->show();
    myUi.frame_2->show();
    myUi.horizontalSlider->show();
    showNormal();
  }
  else
  {
    myUi.sideBarFrame->hide();
    myUi.frame->hide();
    myUi.frame_2->hide();
    myUi.horizontalSlider->hide();
    showFullScreen();
  }
}

void vokoplay::keyPressEvent( QKeyEvent *event )
{
    if ( event->key() == Qt::Key_Escape )
    {
      myUi.sideBarFrame->show();
      myUi.frame->show();
      myUi.frame_2->show();
      showNormal();
      return;
    }
    
    if ( event->key() == Qt::Key_F11 )
    {
      if ( isFullScreen() == true )
      {
        myUi.sideBarFrame->show();
	myUi.frame->show();
	myUi.frame_2->show();
        showNormal();
      }
      else
      {
        myUi.sideBarFrame->hide();
	myUi.frame->hide();
	myUi.frame_2->hide();
        showFullScreen();
      }
      return;
    }
}

void vokoplay::dropEvent( QDropEvent *event )
{
  qDebug() << event->mimeData();
}

void vokoplay::setNewImage( QImage image )
{
  myUi.videoLabel->resize( myUi.videoBackgroundWidget->size() );
  
  QTransform transform;
  transform.rotate( 0 );
  QImage transformedImage = image.transformed( transform );

  // Passt Bild beim resizen des Fensters an
  transformedImage = transformedImage.scaled( myUi.videoLabel->width(), myUi.videoLabel->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
  
  myUi.videoLabel->setPixmap( QPixmap::fromImage( transformedImage, Qt::AutoColor) );
}

