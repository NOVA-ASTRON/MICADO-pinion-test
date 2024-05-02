#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "driverdev.h"

class led_driver : public DriverDev
{
public:
    led_driver(const optional_devs *port, LogFile _log, std::string name);

    void set_intensity(uint8_t channel, int val, int range);
    void enable_led(uint8_t channel);
    void disable_led(int channel);
    void enable(bool en);

private:
    void apply_changes();
    void set_override(bool state);
    float values[2];
    bool enabled[2];
    bool manual;
};

#endif // LED_DRIVER_H
