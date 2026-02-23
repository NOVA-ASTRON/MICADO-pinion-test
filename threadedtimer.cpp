#include <sstream>
#include "threadedtimer.h"
#include <chrono>
#define lock(mut) const std::lock_guard<std::mutex> lock(mut)

threadedTimer::threadedTimer()
{
    measurements = new sample[MAX_SAMPLES];
    TorqueSensorAvailable = TorqueSensor.Setup();
    avg_usec=1;
    requested_sample_time=1000;
    running = false;
    start_measurements=false;
    stop_measurements=false;
    sample_index = 0;
    exit = false;
}

void threadedTimer::setRequested_sample_time(const uint64_t &value)
{
    requested_sample_time = value;
}

void threadedTimer::StampWD()
{
    lock(wdmutex);
    exit=false;
    watchdogstamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();
}

void threadedTimer::unit_of_work()
{
    // --------------------------------------------
    //  compute delay to match requested sample time
    // --------------------------------------------

    uint64_t stamp = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();
    //long long stamp = ns;//std::chrono::steady_clock::now().time_since_epoch().count();
    //uint64_t dtime = stamp-start_of_measurements;

    uint64_t current_dt = stamp-last_measurement_time;


    // --------------------------------------------
    // frequency tuning with 10 usec accuracy
    // --------------------------------------------
    if ((stamp<next_measurement_time) && (current_dt<(requested_sample_time))){
        //usleep(1);
        return;
    }

    // --------------------------------------------
    //  get the sample
    // --------------------------------------------
    if (runcount == 0){
        if (Debug) std::cerr << "Get sample br1\n";
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
       if (Debug) std::cerr << "Get sample br2 "<< curr_sample.val[0] << "\n";
    }
    runcount=(runcount+1)&1023;

    // keep some statistics
    last_measurement_time=stamp;
    avg_usec = (avg_usec+current_dt)/2;

    // advance to next sample time
    next_measurement_time += requested_sample_time;

    // --------------------------------------------
    //  handle if we already will miss our next deadline.
    // --------------------------------------------
    if (stamp>next_measurement_time) {
        // if we want to remain in phase we should do some more calculations..
        // for now just try to keep the frequency constant.
        next_measurement_time=stamp+(((stamp-next_measurement_time)/requested_sample_time)+1)*requested_sample_time;
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
    last_measurement_time=std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch())
                .count();//std::chrono::high_resolution_clock::now().time_since_epoch().count();
    next_measurement_time=last_measurement_time+requested_sample_time;
    StampWD();
    std::cerr << "THREAD STARTED!\n";
    while (!exit){
        unit_of_work();
        watchdog();
    }
    std::cerr << "Threaded timer stopped\n";
}

void threadedTimer::watchdog()
{
    lock(wdmutex);

    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    if ((now-watchdogstamp)>20000) // 5 seconds watchdog
    {
        exit=true;
        std::cerr << "WatchDog triggered!!\n";
    }
}

void threadedTimer::setDebug(volatile bool value)
{
    Debug = value;
}
