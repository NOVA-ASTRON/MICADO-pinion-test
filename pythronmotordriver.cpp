#include "pythronmotordriver.h"

#include <QThread>
#include <cmath>
#define lock(mut) const std::lock_guard<std::mutex> lock(mut);


static const char _initPhytron[] = { 2, 0x30, 'X', 'M', 'A', 3 };
static char _disablePhytron[] = { 2, 0x30, 'X', 'M', 'D', 3 };
static char _readAnalogPhytron[] = { 2, 0x30, 'A', 'D', '1', 'R', 3 };
static char _requestState[] = { 2, 0x30, 'S', 'E', 3 };
static char _enablePowerStage[] = { 2, 0x30, 'E', 'A', 'S', '1', '1', '1', '1', '1', '1', '1', '1', 3 };
static char _resetPhytron[] = { 2, 0x30, 'C', 'R', 3 };

static char _move_cw_zero[] = {2,0x30,'X','0','-','I',3};
static char _move_ccw_zero[] = {2,0x30,'X','0','+','I',3};
static char _mop_min[] = {2,0x30,'X','0','-',3};
static char _mop_plus[] = {2,0x30,'X','0','+',3};
static char _read_encoder_A[] = {2,0x30,'X','P','2','1','R',3};
static char _read_encoder_B[] = {2,0x30,'X','P','2','2','R',3};
static char _read_pos_A[] = {2,0x30,'X','P','2','0','R',3};

static Command initPhytron      ={.cmd=_initPhytron,      .length=sizeof (_initPhytron)};
static Command disablePhytron   ={.cmd=_disablePhytron,   .length=sizeof (_disablePhytron)};
static Command readAnalogPhytron={.cmd=_readAnalogPhytron,.length=sizeof (_readAnalogPhytron)};
static Command requestState     ={.cmd=_requestState,     .length=sizeof (_requestState)};
static Command enablePowerStage ={.cmd=_enablePowerStage, .length=sizeof (_enablePowerStage)};
static Command resetPhytron     ={.cmd=_resetPhytron,     .length=sizeof (_resetPhytron)};

static Command moveCWZero       ={.cmd=_move_cw_zero,     .length=sizeof (_move_cw_zero)};
static Command moveCCWZero      ={.cmd=_move_ccw_zero,     .length=sizeof (_move_ccw_zero)};


static Command MOPplus          ={.cmd=_mop_plus,     .length=sizeof (_mop_plus)};
static Command MOPmin           ={.cmd=_mop_min,     .length=sizeof (_mop_min)};

static Command readEncoderA     ={.cmd=_read_encoder_A,     .length=sizeof (_read_encoder_A)};
static Command readEncoderB     ={.cmd=_read_encoder_B,     .length=sizeof (_read_encoder_B)};

static Command readPosA         ={.cmd=_read_pos_A,     .length=sizeof (_read_pos_A)};

MotorDriver::MotorDriver(const optional_devs *port, LogFile _log, std::string name)
{
    InitDriverDev(port, _log, name);
    reply_received = 0;

    position = 0;
}

void MotorDriver::incomingbytes(QByteArray arr)
{
    for (int index=0;index<arr.length();index+=1){
        unsigned char tmp = arr[index];
        switch (tmp)
        {
        case 0x2:
            reply = "";
            is_ack_message=false;
            bitindex = 0;
            reply_value=0;
            is_float=false;
            float_offset=0;
            reply_is_value=false;
            reply_value_is_negative=false;
            break;
        case 0x3:
            if(getDebug()) std::cerr << "Reply : " << reply << " ; " << reply_received  << " " << "\n";
            if (reply_received==-1 && (reply=="")){
                break;
            }
            if (reply_received <-1) {
                reply_received=0;
                break;
            }
            //if (is_ack_message){
            if (reply_received==-1){
                if (reply=="") break;


                // is someone waiting?
                reply_received=1;
                switch (Interest){

                case FloatPosition:
                    f_reply_value = atof(reply.c_str());
                    reply_received=3; // 1 means not properly received.
                    break;
                case IntPosition:
                    reply_value=atoi(reply.c_str());
                    reply_received=3; // 1 means not properly received.
                    break;
                case GetStatus:
                    if (is_ack_message){
                        StatusMotorIsRunning = (ackbits[8] == false);
                        StatusM0PositionAchieved = (ackbits[9] == true);
                        reply_received=3;
                    } else reply_received=1;
                    break;
                default:
                    reply_received=1;
                    break;

                }
            }
            is_ack_message = false;
            break;
        case 0x6:
            is_ack_message = true;
            bitindex = 15;
            reply_value=0;
            is_float=false;
            float_offset=0;
            reply_is_value=false;
            reply_value_is_negative=false;
            break;
        default:
            reply.append(1,tmp);
            if (tmp=='.'){
                is_float=true;
                break;
            }
            if (tmp=='-'){

                reply_value_is_negative=true;

            }

            if (is_float) float_offset+=1;

            if (is_ack_message && (bitindex >= 3))
            {
                int nibble = 0;
                if ((tmp >= '0') && (tmp <= '9')) nibble = tmp - '0';
                else nibble = tmp - 'A' + 10;

                ackbits[bitindex--] = ((nibble & 8) != 0);
                nibble = nibble * 2;
                ackbits[bitindex--] = ((nibble & 8) != 0);
                nibble = nibble * 2;
                ackbits[bitindex--] = ((nibble & 8) != 0);
                nibble = nibble * 2;
                ackbits[bitindex--] = ((nibble & 8) != 0);
                nibble = nibble * 2;
            }

            if ((tmp >= '0') && (tmp <= '9')) {
                reply_value=reply_value*10+tmp-'0';
                reply_is_value=true;
            } else reply_is_value=false;


            break;
        }
    }
}

void MotorDriver::SetupMicroSteps()
{
    // send command to controller.

    /* DISABLED!!
           byte[] buffer = new byte[10];
           buffer[0] = 2; // <STX>
           buffer[1] = Convert.ToByte(0x30); // address phytron
           buffer[2] = Convert.ToByte('X');
           buffer[3] = Convert.ToByte('P');
           buffer[4] = Convert.ToByte('4');
           buffer[5] = Convert.ToByte('5');
           buffer[6] = Convert.ToByte('S');
           buffer[7] = Convert.ToByte('0' + microsteps / 10);
           buffer[8] = Convert.ToByte('0' + microsteps % 10);
           buffer[9] = 3; // <ETX>

           SendCommand(disablePhytron, "Error:Disable to allow microstepping change.");
           Thread.Sleep(200);
           SendCommand(buffer, "Error:Set Microstepping to " + microsteps.ToString());
           Thread.Sleep(200);
           SendCommand(initPhytron, "Error:Enable to allow microstepping to become active.");
           Thread.Sleep(200);
           SendCommand(enablePowerStage, "Error: unable to send EnablePowerStage");
           Thread.Sleep(1000);
           */
}

void MotorDriver::SetupStepFrequency()
{
    // send command to controller.
    /* DISABLED
           byte[] buffer = new byte[12];
           buffer[0] = 2; // <STX>
           buffer[1] = Convert.ToByte(0x30); // address phytron
           buffer[2] = Convert.ToByte('X');
           buffer[3] = Convert.ToByte('P');
           buffer[4] = Convert.ToByte('1');
           buffer[5] = Convert.ToByte('4');
           buffer[6] = Convert.ToByte('S');
           buffer[7] = Convert.ToByte('2');
           buffer[8] = Convert.ToByte('7');
           buffer[9] = Convert.ToByte('0');
           buffer[10] = Convert.ToByte('0');
           buffer[11] = 3; // <ETX>

           byte[] origspeed = new byte[12];
           origspeed[0] = 2; // <STX>
           origspeed[1] = Convert.ToByte(0x30); // address phytron
           origspeed[2] = Convert.ToByte('X');
           origspeed[3] = Convert.ToByte('P');
           origspeed[4] = Convert.ToByte('1');
           origspeed[5] = Convert.ToByte('4');
           origspeed[6] = Convert.ToByte('S');
           origspeed[7] = Convert.ToByte('4');
           origspeed[8] = Convert.ToByte('0');
           origspeed[9] = Convert.ToByte('0');
           origspeed[10] = Convert.ToByte('0');
           origspeed[11] = 3; // <ETX>

           SendCommand(disablePhytron, "Error:Disable to allow microstepping change.");
           Thread.Sleep(200);

           // LOW speed
           // SendCommand(buffer, "Error:Set Stepping speed to 1000.");

           // ORIG speed
           // SendCommand(origspeed, "Error:Set Stepping speed to 1000.");

           Thread.Sleep(200);
           SendCommand(initPhytron, "Error:Enable to allow microstepping to become active.");
           Thread.Sleep(200);
           SendCommand(enablePowerStage, "Error: unable to send EnablePowerStage");
           Thread.Sleep(1000);
           */
}

void MotorDriver::SetMicroSteps(int steps)
{
    microsteps = steps;
    // SetupStepFrequency();
    SetupMicroSteps();
}




void MotorDriver::Init()
{
    log->Write("Reset Phytron.");
    Reset();
    log->Write("Init Phytron.");
    addHandler();
    SendCommand(initPhytron, "Error: Init Phytron failed. Serial port not connected.");

    SetMicroSteps(94);
    if (!SendAndWaitForReply(requestState, "Error: Request Phytron state at INIT.",GetStatus,false))  std::cerr << "WARNING: Could not get status at INIT!!\n";
    else if (StatusMotorIsRunning)  std::cerr << "WARNING: motor is running at INIT!!\n";

}

void MotorDriver::Reset(int pos)
{
    SendCommand(resetPhytron, "Error: Reset Phytron failed. Serial port not connected.",false);
    // wait 2 seconds for the phytron to properly reset.
    //msdelay(2000);
    position = pos;
}

void MotorDriver::Disconnect()
{
    log->Write("Disable Phytron.");
    SendCommand(disablePhytron, "Error: Disable Phytron failed. Serial port not connected.");
    DriverDev::Disconnect();
}

static const int BUF_SIZE = 256;


void MotorDriver::DoSteps(int steps)
{
    char plusmin = '+';
    position += steps;

#if REVERSE_POLARITY_MOTOR
    steps = -steps;
#endif
    if (steps < 0)
    {
        plusmin = '-';
        steps = -steps;
    }

    totalsteps += steps;
    std::string value = std::to_string(steps * microsteps);

    // assemble command
    char buffer[BUF_SIZE];// new char[value.length() + 10];
    int index = 0;
    buffer[index++] = 2; // <STX>
    buffer[index++] = (0x30); // address phytron
    buffer[index++] = ('X'); // Rotate motor with ammount of steps
    buffer[index++] = (plusmin);
    for (unsigned int i = 0; (i < value.length()) && index<BUF_SIZE; i++)
        buffer[index++] = (value[i]);
    if (index < BUF_SIZE) buffer[index++] = 3; // <ETX>
    if (index < BUF_SIZE){
        Command cmd={.cmd=buffer,.length=index};
        SendCommand(cmd, "Error: Move motor " + value + " steps");
    }
    //Thread.Sleep(100);
    //SendCommand(requestState, "Error: Request Phytron state.");
}

bool MotorDriver::IsMotorRunning(char const * reftxt)
{
    if (SendAndWaitForReply(requestState, reftxt,GetStatus,true))
        return (StatusMotorIsRunning);

    return false;
}

void MotorDriver::Move(int pos)
{
    DoSteps(pos);
}

void MotorDriver::MoveTo(int pos)
{
    Move(pos - position);
}

// goto positio absolute with zero is M0P-
void MotorDriver::GoToPos(int pos)
{
    if (pos < 0)
    {
        pos =0 ;
    }
    char plusmin = '+';
    totalsteps += abs(position-pos);
    position = pos;

    //pos+=4; // relatve to MOP-
    std::string value = std::to_string(pos);

    // assemble command
    char buffer[BUF_SIZE];// = new char[value.length() +10];
    int index = 0;
    buffer[index++] = 2; // <STX>
    buffer[index++] = (0x30); // address phytron
    buffer[index++] = ('X'); // Move to position
    buffer[index++] = ('A'); // Absolute
    buffer[index++] = (plusmin);
    for (unsigned int i = 0; (i < value.length()) && index<BUF_SIZE; i++)
        buffer[index++] = (value[i]);

    if (index < BUF_SIZE) buffer[index++] = 3; // <ETX>
    if (index < BUF_SIZE){
        Command cmd={.cmd=buffer,.length=index};
        SendCommand(cmd, "Error: Move to position " + value + " mm ",true);
    }
}

void MotorDriver::MoveCWzero()
{
    SendCommand(moveCWZero, "Error: Request move cw zero.");
}

void MotorDriver::MoveCCWzero()
{
    SendCommand(moveCCWZero, "Error: Request move ccw zero.");
}

void MotorDriver::DoMOPplus()
{
    StatusM0PositionAchieved=false;
    SendCommand(MOPplus, "Error: Request MOP+.");
}

void MotorDriver::DoMOPmin()
{
    StatusM0PositionAchieved=false;
    SendCommand(MOPmin, "Error: Request MOP-.",false);
}

bool MotorDriver::ReadEncoderA(int & dest)
{
    if (SendAndWaitForReply(readEncoderA, "Error: Request read Pos A.",IntPosition,false)){
        dest=reply_value;
        return true;
    }

    /*
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 1);
    replyreceived = -1;
    if (SendCommand(readEncoderA, "Error: Request readEncoderA.")) {
        //wait up to 200ms for confirmation
        int step = 5;
        for (int ms=0;(ms<200) && (replyreceived == -1);ms+=step) {
            msdelay(step);
        }
        if ((replyreceived == 0) && reply_is_value) return reply_value;
    }
    */
    return false;
}

bool MotorDriver::ReadEncoderB(int & dest)
{
    if (SendAndWaitForReply(readEncoderB, "Error: Request read Pos A.",IntPosition,false)){
        dest=reply_value;
        return true;
    }

    /*
    //QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 1);
    reply_received = -1;
    if (SendCommand(readEncoderB, "Error: Request readEncoderB."))

    {
        //wait up to 200ms for confirmation
        int step = 5;
        for (int ms=0;(ms<200) && (reply_received == -1);ms+=step){
            msdelay(step);
        }
        if ((reply_received == 0) && reply_is_value && is_float) return 42424242;
        if ((reply_received == 0) && reply_is_value) return reply_value;
    }
    */
    return false;
}

bool MotorDriver::ReadPosA(float &dest)
{
    if (SendAndWaitForReply(readPosA, "Error: Request read Pos A.",FloatPosition,false)){
        dest=f_reply_value;
        return true;
    }
    return false;
}

bool MotorDriver::ReadStatus()
{
    if (SendAndWaitForReply(requestState, "Error: Request read Status.",GetStatus,false)){
        return true;
    }
    return false;
}




int MotorDriver::AngleToSteps(double angle)
{
    double exact_angle_steps = (angle * steps_per_adc_revolution * gear_ratio) / (360.0);
    int steps = (int)exact_angle_steps;
    return steps;
}

void MotorDriver::Angle(double angle)
{
    MoveTo(AngleToSteps(angle));
}

void MotorDriver::relAngle(double angle)
{
    Move(AngleToSteps(angle));
}

void MotorDriver::SetGearbox(bool p)
{
    if (p) gear_ratio = 49.0;
    else gear_ratio = 2.0;
}

bool MotorDriver::isM0P()
{
    return StatusM0PositionAchieved;
}

bool MotorDriver::SendCommand(Command cmd, std::string alttxt,bool has_reply)
{
    bool ret=false;
    lock (_lock2) {
        if ((reply_received<0)&&getDebug()) {
            std::cerr << "SendCommand: concurrency detected! :" << alttxt << "\n";
        }
        int num_waits=0;
        while(reply_received<0 && num_waits<100){
            msdelay(10);
            num_waits+=1;
        }
        if (has_reply) reply_received=-6;
        ret=_SendCommand(cmd,alttxt);
        if (ret){

            if (has_reply){
                ret=false;
                auto start = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::high_resolution_clock::now().time_since_epoch())
                        .count();
                auto  now = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::high_resolution_clock::now().time_since_epoch())
                        .count();
                while(reply_received<0 && (now-start)<10000)
                {
                    msdelay(1);

                    now = std::chrono::duration_cast<std::chrono::microseconds>(
                                std::chrono::high_resolution_clock::now().time_since_epoch())
                            .count();
                }

                reply_received=0;
            }
            ret=true;
        } else reply_received=0;
    }
    return ret;
}

bool MotorDriver::SendAndWaitForReply(Command cmd, char const * alttxt, MotorDriver::ReceiverInterest_t interest, bool isfatal)
{
    if (reply_received <0) {
        std::cerr << "SendAndWaitForReply: concurrency detected! :" << alttxt << "\n";
        if (isfatal) exit(-1);
        Error=true;
        return false;
    }
    lock (_lock2) {
        //Skip_timers=true;
        num_requests+=1;
        if (reply_received <0) {
            std::cerr << "SendAndWaitForReply2: concurrency detected during Lock! :" << alttxt << "\n";
            return false;
        }

        // wait for queue to be empty
        auto t = GetDeviceState();
        if (!t.queue_empty){
            while(!t.queue_empty){
                msdelay(1);
                t = GetDeviceState();
            }
        }

        Interest = interest;
        reply_received = -1;
        _SendCommand(cmd,alttxt);

        if (reply_received == -1){
            //wait up to 200ms for confirmation
            int step = 10;
            for (int ms=0;(ms<50) && (reply_received == -1);ms+=1) {
                msdelay(step);
            }
            if (reply_received >0) return true;
            reply_received=0;
            std::cerr << "Phytron request " << num_requests << " : " << alttxt << " got no reply!\n";
            if (isfatal) exit(-1);
        }
    }

    return (reply_received > 0);
}


void MotorDriver::SetTotalSteps(int steps)
{
    totalsteps = steps;
}

int MotorDriver::GetTotalSteps()
{
    return totalsteps;
}

void MotorDriver::SetPos(int steps)
{
    position = steps;
}

int MotorDriver::GetPos()
{
    return position;
}

