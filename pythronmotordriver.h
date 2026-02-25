#ifndef PYTHRONMOTORDRIVER_H
#define PYTHRONMOTORDRIVER_H

#include "driverdev.h"
#include "logfile.h"

class MotorDriver : public DriverDev
   {
public:
    typedef enum {
        GetStatus,
        FloatPosition,
        IntPosition,
    } ReceiverInterest_t ;

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

       bool IsMotorRunning(char const * reftxt);

       void Move(int pos);

       void MoveTo(int pos);
       void GoToPos(int pos); // in mm

       void MoveCWzero();
       void MoveCCWzero();

       void DoMOPplus();
       void DoMOPmin();


       bool ReadEncoderA(int &dest);
       bool ReadEncoderB(int & dest);

       bool ReadPosA(float& dest);
       bool ReadStatus();


       int AngleToSteps(double angle);

       void Angle(double angle);
       // send forward step to motor
       void relAngle(double angle);

       void SetGearbox(bool p);

       bool isM0P();
private:
        int position = 0;
        int totalsteps = 0;
        int microsteps;
        std::string reply = "";
        bool is_float;
        int float_offset;
        int reply_value;
        float f_reply_value;
        bool reply_is_value;
        bool reply_value_is_negative;
        bool is_ack_message = false;
        int bitindex = 0;
        volatile int reply_received = 0;
        bool ackbits[16];
        double gear_ratio = 1.0;
        double steps_per_adc_revolution = 1267.5;
        float posA = 0.0;
        volatile bool Error=false;

        ReceiverInterest_t Interest;
        std::mutex _lock2;
        bool SendCommand(Command cmd, std::string alttxt, bool has_reply=true);
        bool SendAndWaitForReply(Command cmd,char const * alttxt,ReceiverInterest_t interest,bool isfatal=false);

        volatile bool StatusMotorIsRunning=false;
        volatile bool StatusM0PositionAchieved=false;
        volatile int num_requests = 0;

   };


#endif // PYTHRONMOTORDRIVER_H
