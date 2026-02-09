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
    int phytron_pos;
} sample;

typedef enum testbench_states_t {
    TEST_IDLE=0,
    TEST_FINISHED=1,
    TEST_CW_ZERO=2,
    TEST_CCW_ZERO=3,
    TEST_CW_LONG=4,
    TEST_CW_SHORT=5,
    TEST_CCW_LONG=6,
    TEST_CCW_SHORT=7,
    TEST_SUB_STATE=8,
    TEST_START_MOVIE_CAPTURE=9,
    TEST_SAVE_IMAGE=10,
    TEST_WAIT_PHYTRON=11,
    TEST_WAIT_TIME=12,
} testbench_states;

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
    volatile int phytron_position;
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

public slots:
    void unit_of_work();
protected:
    void run();

};

#endif // THREADEDTIMER_H
