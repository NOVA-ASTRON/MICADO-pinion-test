#include "thorlabscamera.h"
#include "tl_camera_sdk_load.h"
#include "tl_camera_sdk.h"
#include <cmath>
#include <iostream>

static uchar gamma_lookup[65536];
void frame_available_callback(void* sender, unsigned short* image_buffer, int frame_count, unsigned char* metadata, int metadata_size_in_bytes, void* context){
    capturebuffer * buf  = (capturebuffer *)context;
    if (image_buffer)
    {
        if (buf->capture_movie && (buf->movie_index<NUM_IMAGE_BUFFERS))
        {
            uchar * dest = buf->movie[buf->movie_index]->bits();
            buf->timestamps[buf->movie_index]=QDateTime::currentDateTime();
            for(int i=0;i<1440*1080;i+=1)
                dest[i]=gamma_lookup[image_buffer[i]];
            buf->movie_index+=1;
            // stop when buffer is full
            if (buf->movie_index>=NUM_IMAGE_BUFFERS)
                buf->capture_movie=false;
            return;
        }
        if (buf->active){
            unsigned int index=(buf->current_index+1)%3;
            uchar * dest = buf->triplebuffer[index]->bits();
            for(int i=0;i<1440*1080;i+=1)
                dest[i]=gamma_lookup[image_buffer[i]];
            buf->current_index=index;
            buf->image_available=index;
        }
    }
}
void camera_connect_callback(char* cameraSerialNumber, enum TL_CAMERA_USB_PORT_TYPE usb_bus_speed, void* context){

}
void camera_disconnect_callback(char* cameraSerialNumber, void* context){

}
ThorlabsCamera::ThorlabsCamera()
{
    capbuf.active=false;
    capbuf.capture_movie=false;
    capbuf.movie_index=0;
    capbuf.current_index=0;
    capbuf.image_available=-1;

    currImage = new QImage(1440,1080,QImage::Format_Grayscale8);
    for (int i=0;i<3;i+=1){
        capbuf.triplebuffer[i]=new QImage(1440,1080,QImage::Format_Grayscale8);
    }
    for (int i=0;i<NUM_IMAGE_BUFFERS;i+=1){
        capbuf.movie[i]=new QImage(1440,1080,QImage::Format_Grayscale8);
    }
    for (int i=0;i<0x10000;i+=1){
        //float v = (float)i/65535.0f;
        float v = (float)i/1024.0f;
        float g = powf(v,1.0f/2.2f)*255.f;
        gamma_lookup[i] = (int) g;
    }
    running=false;
}

ThorlabsCamera::~ThorlabsCamera()
{
    if (running) stop_capture();
    report_error_and_cleanup_resources("finished");
}

bool ThorlabsCamera::init_camera()
{
    // Initializes camera dll
    if (tl_camera_sdk_dll_initialize())
        return report_error_and_cleanup_resources("Failed to initialize dll!\n");
    printf("Successfully initialized dll\n");
    is_camera_dll_open = 1;

    // Open the camera SDK
    if (tl_camera_open_sdk())
        return report_error_and_cleanup_resources("Failed to open SDK!\n");
    printf("Successfully opened SDK\n");
    is_camera_sdk_open = 1;

    char camera_ids[1024];
    camera_ids[0] = 0;

    // Discover cameras.
    if (tl_camera_discover_available_cameras(camera_ids, 1024))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());
    printf("camera IDs: %s\n", camera_ids);

    // Check for no cameras.
    if (!strlen(camera_ids))
        return report_error_and_cleanup_resources("Did not find any cameras!\n");

    // Camera IDs are separated by spaces.
    char* p_space = strchr(camera_ids, ' ');
    if (p_space)
        *p_space = '\0'; // isolate the first detected camera
    char first_camera[256];

    // Copy the ID of the first camera to separate buffer (for clarity)
#ifdef _WIN32
    strcpy_s(first_camera, 256, camera_ids);
#elif defined __linux__
    strcpy(first_camera, camera_ids);
#endif
    printf("First camera_id = %s\n", first_camera);

    // Connect to the camera (get a handle to it).
    if (tl_camera_open_camera(first_camera, &camera_handle))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());
    printf("Camera handle = 0x%p\n", camera_handle);

    return true;
}

void ThorlabsCamera::setExposure(long long exposure)
{
    bool was_running=running;
    if (running) stop_capture();
    tl_camera_set_exposure_time(camera_handle, exposure);
    if(was_running) start_capture();
}

bool ThorlabsCamera::setup_callbacks()
{
    // Set the camera connect event callback. This is used to register for run time camera connect events.
    if (tl_camera_set_camera_connect_callback(camera_connect_callback, 0))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());

    // Set the camera disconnect event callback. This is used to register for run time camera disconnect events.
    if (tl_camera_set_camera_disconnect_callback(camera_disconnect_callback, 0))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());

    // Set the exposure
    long long const exposure = 10000; // 10 ms
    if (tl_camera_set_exposure_time(camera_handle, exposure))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());
    printf("Camera exposure set to %lld\n", exposure);

    // Set the gain
    int gain_min;
    int gain_max;
    if (tl_camera_get_gain_range(camera_handle, &gain_min, &gain_max))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());
    if (gain_max > 0)
    {
        // this camera supports gain, set it to 6.0 decibels
        const double gain_dB = 6.0;
        int gain_index;
        if (tl_camera_convert_decibels_to_gain(camera_handle, gain_dB, &gain_index))
            return report_error_and_cleanup_resources(tl_camera_get_last_error());
        tl_camera_set_gain(camera_handle, gain_index);
    }

    // Configure camera for continuous acquisition by setting the number of frames to 0.
    // This project only waits for the first frame before exiting
    if (tl_camera_set_frames_per_trigger_zero_for_unlimited(camera_handle, 0))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());

    // Set the frame available callback
    if (tl_camera_set_frame_available_callback(camera_handle, frame_available_callback, &capbuf))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());
    return 0;
}

bool ThorlabsCamera::start_capture()
{
    capbuf.active=false;
    capbuf.capture_movie=false;
    capbuf.movie_index=0;
    capbuf.current_index=0;
    capbuf.image_available=-1;

    if (tl_camera_arm(camera_handle, 2))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());
    printf("Camera armed\n");
    if (tl_camera_issue_software_trigger(camera_handle))
        return report_error_and_cleanup_resources(tl_camera_get_last_error());
    printf("Software trigger sent\n");
    std::cout << std::flush;
    running=true;
    capbuf.active=true;
    return false;
}

bool ThorlabsCamera::stop_capture()
{
    // Stop the camera.
    if (tl_camera_disarm(camera_handle)){
        printf("Failed to stop the camera!\n");
        return false;
    }
    running = false;
    return true;

}

QImage *ThorlabsCamera::getImage()
{
    if (capbuf.image_available>=0){
        currImage=capbuf.triplebuffer[capbuf.image_available];
        capbuf.image_available=-1;
        return currImage;
    }
    return nullptr;
}

void ThorlabsCamera::start_movie_capture()
{
    capbuf.capture_movie=false;
    capbuf.active=false;
    capbuf.movie_index=0;
    capbuf.capture_movie=true;
}

void ThorlabsCamera::stop_movie_capture()
{
    // save the movie here?
    capbuf.capture_movie=false;
    capbuf.active=true;
}

int ThorlabsCamera::getMovieSize()
{
    return capbuf.movie_index;
}

QImage *ThorlabsCamera::getMovieFrame(int num)
{
    if (num>=0 && num<NUM_IMAGE_BUFFERS) {
        return capbuf.movie[num];
    }
    return nullptr;
}

QDateTime ThorlabsCamera::getMovieStamp(int num)
{
    if (num>=0 && num<NUM_IMAGE_BUFFERS) {
        return capbuf.timestamps[num];
    }
    return QDateTime::currentDateTime();
}

bool ThorlabsCamera::report_error_and_cleanup_resources(const char *error_string)
{

    int num_errors = 0;

    if (error_string)
    {
        printf("Error: %s\n", error_string);
        num_errors++;
    }

    printf("Closing all resources...\n");

    if (camera_handle)
    {
        if (tl_camera_close_camera(camera_handle))
        {
            printf("Failed to close camera!\n%s\n", tl_camera_get_last_error());
            num_errors++;
        }
        camera_handle = 0;
    }
    if (is_camera_sdk_open)
    {
        if (tl_camera_close_sdk())
        {
            printf("Failed to close SDK!\n");
            num_errors++;
        }
        is_camera_sdk_open = 0;
    }
    if (is_camera_dll_open)
    {
        if (tl_camera_sdk_dll_terminate())
        {
            printf("Failed to close dll!\n");
            num_errors++;
        }
        is_camera_dll_open = 0;
    }

    printf("Closing resources finished.\n");
    return false;

}
