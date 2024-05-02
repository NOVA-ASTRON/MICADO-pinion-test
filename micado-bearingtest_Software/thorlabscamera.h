#ifndef THORLABSCAMERA_H
#define THORLABSCAMERA_H


#include "qdatetime.h"
#include <QImage>

// fps*secs = NUM_IMAGE_BUFFERS

#define NUM_IMAGE_BUFFERS 50

typedef struct capturebuffer_t {
    unsigned int current_index;
    volatile int image_available;
    volatile bool capture_movie;
    volatile bool active;
    unsigned int movie_index;
    QImage * triplebuffer[3];
    QImage * movie[NUM_IMAGE_BUFFERS];
    QDateTime timestamps[NUM_IMAGE_BUFFERS];
} capturebuffer;

class ThorlabsCamera
{
public:
    ThorlabsCamera();
    ~ThorlabsCamera();
    bool init_camera();
    void setExposure(long long exposure=10000);
    bool setup_callbacks();
    bool start_capture();
    bool stop_capture();
    QImage * getImage();
    void start_movie_capture();
    void stop_movie_capture();
    int getMovieSize();
    QImage * getMovieFrame(int num);
    QDateTime getMovieStamp(int num);
private:
    QImage * currImage;
    capturebuffer capbuf;
    int is_camera_sdk_open = 0;
    int is_camera_dll_open = 0;
    void* camera_handle = 0;
    bool running;
    bool capture_movie;

    bool report_error_and_cleanup_resources(const char* error_string);
    //int initialize_camera_resources();

};

#endif // THORLABSCAMERA_H
