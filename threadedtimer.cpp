#include <sstream>
#include "threadedtimer.h"
#include <chrono>
#define lock(mut) const std::lock_guard<std::mutex> lock(mut)

static uint64_t Now(){
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}
static int64_t Sec(uint64_t t){
    return 1000000000ULL*t;
}
static int64_t mSec(uint64_t t){
    return 1000000ULL*t;
}
static int64_t uSec(uint64_t t){
    return 1000ULL*t;
}
threadedTimer::threadedTimer()
{
    measurements = new sample[MAX_SAMPLES];
    TorqueSensorAvailable = TorqueSensor.Setup();
    avg_usec=1;
    running = false;
    start_measurements=false;
    stop_measurements=false;
    sample_index = 0;
    exit = false;
    setRequested_sample_time(10);
    StampWD();
}

void threadedTimer::setRequested_sample_time(const uint64_t &value)
{
    requested_sample_time = mSec(value);
}

void threadedTimer::DoRequest(Request ask)
{
    request = ask;
}

void threadedTimer::StampWD()
{
    lock(wdmutex);
    exit=false;
    watchdogstamp = Now();
}

void threadedTimer::unit_of_work()
{
    // --------------------------------------------
    //  compute delay to match requested sample time
    // --------------------------------------------

    uint64_t stamp = Now();
    //long long stamp = ns;//std::chrono::steady_clock::now().time_since_epoch().count();
    //uint64_t dtime = stamp-start_of_measurements;

    uint64_t current_dt = stamp-last_measurement_time;


    // --------------------------------------------
    // frequency tuning with 10 usec accuracy
    // --------------------------------------------
    if ((stamp<next_measurement_time) && (current_dt<(requested_sample_time))){
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
       curr_sample.phytron_pos=phytron_position;
       if (Debug) std::cerr << "Get sample br2 "<< curr_sample.val[0] << "\n";
    }
    runcount=(runcount+1)&1023;

    // keep some statistics
    last_measurement_time=stamp;
    avg_usec = (avg_usec+(current_dt/1000))/2;

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
    // store the sample if there is room left
    // --------------------------------------------

    if (running && sample_index<MAX_SAMPLES){
        std::cerr <<"Add sample " << sample_index << "\n";
        measurements[sample_index++]=curr_sample;
        num_measurements=sample_index;
    }

    // --------------------------------------------
    //  handle requests and act according to flags
    // --------------------------------------------


    switch (request){
    case REQ_START_MEASUREMENTS:
        std::cerr << "Start measure\n";
        request=REQ_IDLE;
        if (running) break;
        running=true;
        sample_index=0;
        break;
    case REQ_STOP_MEASUREMENTS:
        std::cerr << "Stop measure\n";
        request=REQ_IDLE;
        if (!running) break;
        num_measurements=sample_index;
        running=false;
        break;
    default:
        break;

    }
}

void threadedTimer::run()
{
    // prime our timings on 1ms sample time.
    last_measurement_time=Now();
    next_measurement_time=last_measurement_time+requested_sample_time;
    StampWD();
    std::cerr << "THREAD STARTED!\n";
    while (!exit){
        unit_of_work();
        watchdog();
        //usleep(1);
    }
    std::cerr << "Threaded timer stopped\n";
}

void threadedTimer::watchdog()
{
    lock(wdmutex);

    uint64_t WDtime = Now() - watchdogstamp;
    if (WDtime>Sec(120)) // 5 seconds watchdog
    {
        //exit=true;
        std::cerr << "WatchDog triggered!! :" << std::chrono::seconds(120).count() << "\n";
    }
}

void threadedTimer::setDebug(volatile bool value)
{
    Debug = value;
}
