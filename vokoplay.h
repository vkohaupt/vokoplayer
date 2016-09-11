#ifndef vokoplay_H 
#define vokoplay_H

#include "ui_vokoplay.h"

#include <QObject>
#include <QMainWindow>
#include <QVideoWidget>
#include <QLabel>
#include <QMediaPlayer>


class vokoplay: public QMainWindow
{
Q_OBJECT
public:    
  vokoplay();
  virtual ~vokoplay();
    
public:

  
public slots:
  void setVideoName( QString video );
  void prepare();

  
private:
  Ui_MainWindow myUi;
  QLabel *videoLabel;
  QMediaPlayer *player;
  QString videoName;
  
  
private slots:
  void setNewImage( QImage image );
  QString getVideoName();
  void play();
  void pause();  
  void mute();
  void sliderMoved( int value );
  void playerStateChanged( QMediaPlayer::State state );

  
protected:
  void mouseDoubleClickEvent( QMouseEvent *event );
  void keyPressEvent( QKeyEvent *event );
  void dropEvent( QDropEvent *event );
};

#endif // vokoplay_H