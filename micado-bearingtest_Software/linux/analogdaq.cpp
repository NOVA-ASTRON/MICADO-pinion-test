#include "analogdaq.h"
#include <iostream>

#include "pmd.h"

#include "usb-2408.h"

AnalogDAQ::AnalogDAQ()
{
    // DAQ


}

int AnalogDAQ::Setup()
{
#if 1
    int ret = libusb_init(&mccdev.ctx);
    mccdev.usb2408_2AO=false;
    if (ret < 0) {
        std::cerr << "usb_device_find_USB_MCC: Failed to initialize libusb\n";
        return 0;
    }
    mccdev.udev = nullptr;
    if ((mccdev.udev = usb_device_find_USB_MCC(USB2408_PID, nullptr))) {
        std::cerr << "Success, found a USB 2408!\n";
      } else if ((mccdev.udev = usb_device_find_USB_MCC(USB2408_2AO_PID, nullptr))) {
        std::cerr << "Success, found a USB 2408_2AO!\n";
        mccdev.usb2408_2AO = true;
      } else {
        printf("Failure, did not find a USB 2408 or 2408_2AO!\n");
        libusb_exit(mccdev.ctx);
        return 0;
      }

    //print out the wMaxPacketSize.  Should be 64
      printf("wMaxPacketSize = %d\n", usb_get_max_packet_size(mccdev.udev,0));

      usbBuildGainTable_USB2408(mccdev.udev, mccdev.table_AIN);
      for (int i = 0; i < NGAINS_2408; i++) {
        printf("Gain: %d    Slope = %lf    Offset = %lf\n", i, mccdev.table_AIN[i][0], mccdev.table_AIN[i][1]);
      }

      printf("\n");
      usbBuildCJCGradientTable_USB2408(mccdev.udev, mccdev.table_CJCGrad);
      for (int i = 0; i < nCJCGrad_2408; i++) {
        printf("Ch: %d    CJC gradient = %lf\n", i, mccdev.table_CJCGrad[i]);
      }

      if (mccdev.usb2408_2AO) {
        usbBuildGainTable_USB2408_2AO(mccdev.udev, mccdev.table_AO);
        printf("\n");
        for (int i = 0; i < NCHAN_AO_2408; i++) {
          printf("VDAC%d:    Slope = %lf    Offset = %lf\n", i, mccdev.table_AO[i][0], mccdev.table_AO[i][1]);
        }
      }

      usbCalDate_USB2408(mccdev.udev, &mccdev.calDate);
      printf("\n");
      printf("MFG Calibration date = %s\n", asctime(&mccdev.calDate));
#endif
    return 1;
}

uint16_t lfsr=12345;

float AnalogDAQ::get_AD(int channel)
{
    uint16_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
    lfsr = (lfsr >> 1) | (bit << 15);
    if (channel==0){
        return 0.0f+(float)((lfsr&511)-255)/300.f;
    }
    // 0..10 = -180 .. 60
    //
    return 8.45f+(float)(lfsr&255)/3000.f;
}



