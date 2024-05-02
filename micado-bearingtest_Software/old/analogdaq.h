#ifndef ANALOGDAQ_H
#define ANALOGDAQ_H



class AnalogDAQ
{
public:
    AnalogDAQ();
    int Setup();
    float get_AD(int channel);

private:
    // DAQ
    int		BoardNum;
    int		ULStat;
    int		Chan, NumAIChans;
    int		Range;
    float   EngUnits;
    double  HighResEngUnits;
    float   RevLevel;
    bool	HighResAD;
    int		ADRes;
    int		DefaultTrig;
    int		Options;
    int		ChannelType;
    char	*BoardName;
    int		dummy;
    float ADval[16];
};

#endif // ANALOGDAQ_H
