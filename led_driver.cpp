#include "led_driver.h"
float const full_scale=1024.f;

led_driver::led_driver(const optional_devs *port, LogFile _log, std::string name)
{
    InitDriverDev(port, _log, name);
    for (int channel=0;channel<2;channel+=1){
        values[channel]=0.f;
        enabled[channel]=false;
    }
    manual=true;
}

void led_driver::set_intensity(uint8_t channel, int val,int range)
{
    if (channel<2){
        float valf = (float)val/(float)range;
        if (valf>0.0f){
            values[channel]=valf;
            enable_led(channel);
        }
    }
}

void led_driver::enable_led(uint8_t channel)
{
    if (channel<2){
        if (manual)
            enable(true);
        enabled[channel]=true;
        int val=values[channel]*full_scale;
        Command cmd={"set led value ...",4};
        SendCommand(cmd,"cannot send command");
    }
}

void led_driver::disable_led(int channel)
{
    if (channel<2){
        if (manual)
            enable(true);
        enabled[channel]=false;
        Command cmd={"set led value 0",4};
        SendCommand(cmd,"cannot send command");
    }
}

void led_driver::enable(bool en)
{
    if (manual!=en){
        Command cmd={"test",4};
        SendCommand(cmd,"cannot send command");
    }
}
