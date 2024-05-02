#include "analogdaq.h"
#include <iostream>

#ifdef __WINDOWS__
#include "win11/Analog.h"

AnalogDAQ::AnalogDAQ()
{
    // DAQ
    BoardNum = 0;
    ULStat = NOERRORS;
    Range = BIP10VOLTS;
    RevLevel = (float)CURRENTREVNUM;
    HighResAD = FALSE;
    ADRes = 0;
    DefaultTrig = TRIG_POS_EDGE;
    Options = FOREGROUND;
    ChannelType = ANALOGINPUT;
    BoardName=new char[BOARDNAMELEN];
}

int AnalogDAQ::Setup()
{
    /* Declare UL Revision Level */
    ULStat = cbDeclareRevision(&RevLevel);

    InitUL();	// Set up error handling

    // get the name of the board
    if (!GetNameOfBoard(BoardNum, BoardName)) {
        //DisplayMessage(NOERRORS);
        return 0;
    }

    printf("Demonstration of cbVIn() using %s\n\n", BoardName);

    // Determine if device is compatible with this example
    NumAIChans = FindAnalogChansOfType(BoardNum, ChannelType, &ADRes, &Range, &Chan, &DefaultTrig);
    if (NumAIChans == 0) {
        printf("%s (board %u) does not have analog input channels.\n", BoardName, BoardNum);
        //DisplayMessage(NOERRORS);
        return 0;
    }

    /* check If the resolution of A/D is higher than 16 bit.
               If it is, then the A/D is high resolution. */
    if(ADRes > 16)
        HighResAD = TRUE;
    return 1;
}

float AnalogDAQ::get_AD(int channel)
{
    /*Parameters:
            BoardNum    :number used by CB.CFG to describe this board
            Chan        :input channel number
            Range        :Range for the board in BoardNum
            DataValue   :value collected from Chan */

    if(HighResAD) {
        ULStat = cbVIn32(BoardNum, channel, Range, &HighResEngUnits, Options);
        return HighResEngUnits;
    }
    ULStat = cbVIn(BoardNum, channel, Range, &EngUnits, Options);
    return EngUnits;
}
#else
#include "linux/pmd.h"

#include "linux/usb-2408.h"
typedef struct mcc_daq_t {
    int flag;
    libusb_device_handle *udev = NULL;
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

static mcc_daq mccdev;
AnalogDAQ::AnalogDAQ()
{
    // DAQ


}

int AnalogDAQ::Setup()
{
    int ret = libusb_init(NULL);
    mccdev.usb2408_2AO=false;
    if (ret < 0) {
        std::cerr << "usb_device_find_USB_MCC: Failed to initialize libusb\n";
        return 0;
    }
    mccdev.udev = NULL;
    if ((mccdev.udev = usb_device_find_USB_MCC(USB2408_PID, NULL))) {
        std::cerr << "Success, found a USB 2408!\n";
      } else if ((mccdev.udev = usb_device_find_USB_MCC(USB2408_2AO_PID, NULL))) {
        std::cerr << "Success, found a USB 2408_2AO!\n";
        mccdev.usb2408_2AO = true;
      } else {
        printf("Failure, did not find a USB 2408 or 2408_2AO!\n");
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

    return 1;
}
float AnalogDAQ::get_AD(int channel)
{
    float ret =0.0;
    return ret;
}

#endif



