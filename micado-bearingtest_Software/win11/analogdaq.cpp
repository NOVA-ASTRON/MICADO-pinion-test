#include "analogdaq.h"
#include <iostream>

#include "Analog.h"

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

    Range=BIP10VOLTS;
    if(HighResAD) {

        ULStat = cbVIn32(BoardNum, channel, Range, &HighResEngUnits, Options);
        return HighResEngUnits;
    }
    ULStat = cbVIn(BoardNum, channel, Range, &EngUnits, Options);
    return EngUnits;
}




