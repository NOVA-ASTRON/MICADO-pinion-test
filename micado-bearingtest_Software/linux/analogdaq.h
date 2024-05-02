#ifndef ANALOGDAQ_H
#define ANALOGDAQ_H


#include "libusb.h"
#include "linux/usb-2408.h"
typedef struct mcc_daq_t {
    int flag;
    libusb_device_handle *udev = NULL;
    libusb_context *ctx;
    struct tm calDate;
    int ch;
    int temp;
    int transferred;
    int i, j;
    int tc_type;
    uint8_t input;
    uint8_t channel, gain, rate, mode, flags;
    char serial[9];
    uint16_t version[4];
    float cjc[2];
    int queue_index;
    int data;
    int ret;
    double table_AIN[NGAINS_2408][2];
    double table_AO[NCHAN_AO_2408][2];
    float table_CJCGrad[nCJCGrad_2408];
    AInScanQueue scanQueue;
    int usb2408_2AO;
    double voltage;
    double temperature;
    double frequency;
    double sine[512];
    uint16_t sdata[512];  // holds 16 bit unsigned analog output data
    int32_t idata[512];   // holds 24 bit signed analog input data
    uint8_t status;
    uint16_t depth;
    uint16_t count;
} mcc_daq;


class AnalogDAQ
{
public:
    AnalogDAQ();
    int Setup();
    float get_AD(int channel);

private:
    // DAQ
    mcc_daq mccdev;
};

#endif // ANALOGDAQ_H
