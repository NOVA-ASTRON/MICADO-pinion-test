#include "pythronmotordriver.h"

#include <QThread>

MotorDriver::MotorDriver(const optional_devs *port, LogFile _log, std::string name)
{
    InitDriverDev(port, _log, name);
    position = 0;
    ackbits[8] = true; // motor is stopped.
}

void MotorDriver::incomingbytes(QByteArray arr)
{
    foreach(char tmp,arr){
        switch (tmp)
        {
        case 0x2:
            reply = "";
            break;
        case 0x3:
            if (reply != "")
            {
                // SerialLog.Write("Got reply " + reply);
            }
            if (is_ack_message){
                if(reply_value_is_negative&&reply_is_value)
                    reply_value=-reply_value;
                if (reply_is_value){

                }
                replyreceived = 0;
            }
            is_ack_message = false;
            break;
        case 0x6:
            is_ack_message = true;
            bitindex = 15;
            reply_value=0;
            reply_is_value=false;
            reply_value_is_negative=false;
            break;
        default:
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
            if (tmp=='-') reply_value_is_negative=true;
            reply += tmp;
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



static const char _initPhytron[] = { 2, 0x30, 'X', 'M', 'A', 3 };
static char _disablePhytron[] = { 2, 0x30, 'X', 'M', 'D', 3 };
static char _readAnalogPhytron[] = { 2, 0x30, 'A', 'D', '1', 'R', 3 };
static char _requestState[] = { 2, 0x30, 'S', 'E', 3 };
static char _enablePowerStage[] = { 2, 0x30, 'E', 'A', 'S', '1', '1', '1', '1', '1', '1', '1', '1', 3 };
static char _resetPhytron[] = { 2, 0x30, 'C', 'R', 3 };

static char _move_cw_zero[] = {2,0x30,'X','0','-','I',3};
static char _move_ccw_zero[] = {2,0x30,'X','0','+','I',3};

static char _read_encoder_A[] = {2,0x30,'X','P','2','1','R',3};
static char _read_encoder_B[] = {2,0x30,'X','P','2','2','R',3};

static Command initPhytron      ={.cmd=_initPhytron,      .length=sizeof (_initPhytron)};
static Command disablePhytron   ={.cmd=_disablePhytron,   .length=sizeof (_disablePhytron)};
static Command readAnalogPhytron={.cmd=_readAnalogPhytron,.length=sizeof (_readAnalogPhytron)};
static Command requestState     ={.cmd=_requestState,     .length=sizeof (_requestState)};
static Command enablePowerStage ={.cmd=_enablePowerStage, .length=sizeof (_enablePowerStage)};
static Command resetPhytron     ={.cmd=_resetPhytron,     .length=sizeof (_resetPhytron)};

static Command moveCWZero       ={.cmd=_move_cw_zero,     .length=sizeof (_move_cw_zero)};
static Command moveCCWZero      ={.cmd=_move_ccw_zero,     .length=sizeof (_move_ccw_zero)};
static Command readEncoderA     ={.cmd=_read_encoder_A,     .length=sizeof (_read_encoder_A)};
static Command readEncoderB     ={.cmd=_read_encoder_B,     .length=sizeof (_read_encoder_B)};


void MotorDriver::Init()
{
    log->Write("Reset Phytron.");
    Reset();
    log->Write("Init Phytron.");
    addHandler();
    SendCommand(initPhytron, "Error: Init Phytron failed. Serial port not connected.");
    SetMicroSteps(32);
}

void MotorDriver::Reset(int pos)
{
    SendCommand(resetPhytron, "Error: Reset Phytron failed. Serial port not connected.");
    // wait 2 seconds for the phytron to properly reset.
    // msdelay(2000);
    position = pos;
}

void MotorDriver::Disconnect()
{
    log->Write("Disable Phytron.");
    SendCommand(disablePhytron, "Error: Disable Phytron failed. Serial port not connected.");
    Disconnect();
}

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
    char * buffer = new char[value.length() + 5];
    int index = 0;
    buffer[index++] = 2; // <STX>
    buffer[index++] = (0x30); // address phytron
    buffer[index++] = ('X'); // Rotate motor with ammount of steps
    buffer[index++] = (plusmin);
    for (unsigned int i = 0; i < value.length(); i++)
        buffer[index++] = (value[i]);

    buffer[index++] = 3; // <ETX>
    Command cmd={.cmd=buffer,.length=index};

    SendCommand(cmd, "Error: Move motor " + value + " steps");
    //Thread.Sleep(100);
    SendCommand(requestState, "Error: Request Phytron state.");
}

bool MotorDriver::IsMotorRunning()
{
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 1);

    replyreceived = -1;
    SendCommand(requestState, "Error: Request Phytron state.");

    //wait up to 200ms for confirmation
    int step = 5;
    for (int ms=0;(ms<200) && (replyreceived == -1);ms+=step) {
        msdelay(step);
    }
    return (ackbits[8] == false);
}

void MotorDriver::Move(int pos)
{
    DoSteps(pos);
}

void MotorDriver::MoveTo(int pos)
{
    Move(pos - position);
}

void MotorDriver::MoveCWzero()
{
    SendCommand(moveCWZero, "Error: Request move cw zero.");
}

void MotorDriver::MoveCCWzero()
{
    SendCommand(moveCCWZero, "Error: Request move ccw zero.");
}

int MotorDriver::ReadEncoderA()
{
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
    return 0;
}

int MotorDriver::ReadEncoderB()
{
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 1);
    replyreceived = -1;
    if (SendCommand(readEncoderB, "Error: Request readEncoderB.")){
        //wait up to 200ms for confirmation
        int step = 5;
        for (int ms=0;(ms<200) && (replyreceived == -1);ms+=step){
            msdelay(step);
        }
        if ((replyreceived == 0) && reply_is_value) return reply_value;
    }
    return 0;
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
    else gear_ratio = 1.0;
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

