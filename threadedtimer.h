#ifndef THREADEDTIMER_H
#define THREADEDTIMER_H

#include <QObject>
#include <QThread>
#include <QTime>
#include <QTimer>
#include <analogdaq.h>

#include "logfile.h"

typedef struct {
    uint64_t stamp;
    qreal val[4];
    float phytron_pos;
} sample;

typedef enum {
    REQ_IDLE,
    REQ_START_MEASUREMENTS,
    REQ_STOP_MEASUREMENTS,
    REQ_UNDEF,
} Request;

const int MAX_SAMPLES = 1000*120;

class threadedTimer : public QThread
{
    Q_OBJECT
public:
    threadedTimer();

    AnalogDAQ TorqueSensor;
    bool TorqueSensorAvailable;

    // ---------------------------------
    // flags for interactions
    // ---------------------------------
    volatile bool start_measurements;
    volatile bool stop_measurements;
    volatile bool running;
    volatile bool exit;

    // ---------------------------------
    // sample buffer, has limited size
    // will not store more than MAX_SAMPLES
    // ---------------------------------
    volatile int num_measurements;
    volatile float phytron_position;
    int sample_index;
    unsigned int runcount;
    sample *measurements;
    sample curr_sample;

    // ---------------------------------
    // time keeping
    // ---------------------------------
    uint64_t avg_usec;
    uint64_t requested_sample_time;
    uint64_t next_measurement_time;
    uint64_t total_amount_of_samples;
    uint64_t last_measurement_time;
    std::mutex wdmutex;
    uint64_t watchdogstamp;

    void StampWD();

    void setDebug(volatile bool value);

    void setRequested_sample_time(const uint64_t &value);

    void DoRequest(Request ask);

public slots:
    void unit_of_work();
protected:
    void run();
private:
    void watchdog();
    volatile bool Debug;
    volatile Request request;

};

#endif // THREADEDTIMER_H
