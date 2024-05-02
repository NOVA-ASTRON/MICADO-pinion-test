#include <sstream>
#include "threadedtimer.h"
#include <chrono>

threadedTimer::threadedTimer()
{
    measurements = new sample[MAX_SAMPLES];
    TorqueSensorAvailable = TorqueSensor.Setup();
    avg_usec=1;
    requested_sample_time=1;
    running = false;
    start_measurements=false;
    stop_measurements=false;
    sample_index=0;
}

void threadedTimer::unit_of_work()
{
    // --------------------------------------------
    //  compute delay to match requested sample time
    // --------------------------------------------

    uint64_t stamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();
    //long long stamp = ns;//std::chrono::steady_clock::now().time_since_epoch().count();
    //uint64_t dtime = stamp-start_of_measurements;

    uint64_t current_dt = stamp-last_measurement_time;


    // --------------------------------------------
    // frequency tuning with 10 usec accuracy
    // --------------------------------------------
    if ((stamp<next_measurement_time) && (current_dt<(requested_sample_time*1000000ull))){
        //usleep(1);
        return;
    }

    // --------------------------------------------
    //  get the sample
    // --------------------------------------------
    if (runcount == 0){
        curr_sample={
            stamp,
            {
                TorqueSensor.get_AD(0),
                TorqueSensor.get_AD(1),
                TorqueSensor.get_AD(2),
                TorqueSensor.get_AD(3),
            },phytron_position,
        };
    } else {
       curr_sample.stamp=stamp;
       curr_sample.val[0]=TorqueSensor.get_AD(0);
    }
    runcount=(runcount+1)&1023;

    // keep some statistics
    last_measurement_time=stamp;
    avg_usec = (avg_usec+(current_dt/1000ull))/2;

    // advance to next sample time
    next_measurement_time += requested_sample_time*1000000ull;

    // --------------------------------------------
    //  handle when time taken was more than 2x sample time
    // --------------------------------------------
    if (stamp>next_measurement_time) {
        // if we want to remain in phase we should do some more calculations..
        // for now just try to keep the frequency constant.
        next_measurement_time=stamp+requested_sample_time*1000000ull;
    }

    // --------------------------------------------
    //  handle requests and act according to flags
    // --------------------------------------------

    if (!running){
        // start a new measurement batch?
        if (start_measurements) {
            sample_index = 0;
            running=true;
            stop_measurements=false;
        } else
            return;
    }
    start_measurements=false;

    if (stop_measurements) {
        running = false;
        stop_measurements=false;
        num_measurements=sample_index;
        return;
    }

    // --------------------------------------------
    // store the sample if there is room left
    // --------------------------------------------
    if (sample_index<MAX_SAMPLES)
        measurements[sample_index++]=curr_sample;

}

void threadedTimer::run()
{
    // prime our timings on 1ms sample time.
    last_measurement_time=std::chrono::high_resolution_clock::now().time_since_epoch().count();
    next_measurement_time=last_measurement_time+requested_sample_time*1000000ull;
    while (1){
        unit_of_work();
    }
}
