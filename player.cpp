#include "glib.h"
#include "gst/gst.h"
#include "gst/video/videooverlay.h"

#include <QApplication>
#include <QWidget>
#include <QDebug>

// ./play --platform wayland

 int main(int argc, char *argv[])
 {

   gst_init (&argc, &argv);
   QApplication app(argc, argv);

// gst-launch-1.0 filesrc location=/home/vk/Videos/Mission-Impossible.mp4 ! decodebin ! videoconvert ! xvimagesink

   GstElement *pipeline = gst_pipeline_new( "myPipeline" );
   GstElement *filesrc = gst_element_factory_make( "videotestsrc", nullptr );
//   GstElement *filesrc = gst_element_factory_make( "filesrc", nullptr );
   GstElement *videoconvert = gst_element_factory_make( "videoconvert", nullptr );
   GstElement *sink = gst_element_factory_make( "xvimagesink", nullptr );

//   gst_bin_add_many( GST_BIN( pipeline ), filesrc, decodebin, videoconvert, sink, nullptr );
   gst_bin_add_many( GST_BIN( pipeline ), filesrc, videoconvert, sink, nullptr );
   gst_element_link( filesrc, videoconvert  );
   gst_element_link( videoconvert, sink );

   QWidget *window = new QWidget;
   window->resize( 320, 240 );
   window->setWindowTitle( "voko-player" );
   window->show();

   WId xwinid = window->winId();
   gst_video_overlay_set_window_handle( GST_VIDEO_OVERLAY( sink ), xwinid );

   GstStateChangeReturn sret = gst_element_set_state( pipeline, GST_STATE_PLAYING );
   if ( sret == GST_STATE_CHANGE_FAILURE )
   {
     gst_element_set_state ( pipeline, GST_STATE_NULL );
     gst_object_unref( pipeline );
   }

   int ret = app.exec();

   gst_element_set_state (pipeline, GST_STATE_NULL);
   gst_object_unref (pipeline);

   return ret;
 }
