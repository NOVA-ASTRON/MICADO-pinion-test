#ifndef PYTHRONMOTORDRIVER_H
#define PYTHRONMOTORDRIVER_H

#include "driverdev.h"
#include "logfile.h"

class MotorDriver : public DriverDev
   {
public:

       LogFile_t SerialLog;// = new LogFile("C:\\MICADO\\phytronserial.txt");

       //int deltapos=0;
       //bool runsmooth = false;

       void SetTotalSteps(int steps);
       int GetTotalSteps();
       void SetPos(int steps);
       int GetPos();

       MotorDriver(const optional_devs *port, LogFile _log, std::string name);

       void incomingbytes(QByteArray arr);

       void SetupMicroSteps();

       void SetupStepFrequency();

       void SetMicroSteps(int steps);

       void Init();

       void Reset(int pos = 0);

       void Disconnect();

       void DoSteps(int steps);

       bool IsMotorRunning();

       void Move(int pos);

       void MoveTo(int pos);

       void MoveCWzero();
       void MoveCCWzero();

       int ReadEncoderA();
       int ReadEncoderB();


       int AngleToSteps(double angle);

       void Angle(double angle);
       // send forward step to motor
       void relAngle(double angle);

       void SetGearbox(bool p);
       private:
        int position = 0;
        int totalsteps = 0;
        int microsteps;
        std::string reply = "";
        int reply_value;
        bool reply_is_value;
        bool reply_value_is_negative;
        bool is_ack_message = false;
        int bitindex = 0;
        int replyreceived = 0;
        bool ackbits[16];
        double gear_ratio = 1.0;
        double steps_per_adc_revolution = 1267.5;

   };


#endif // PYTHRONMOTORDRIVER_H
