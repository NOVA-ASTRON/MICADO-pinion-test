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
    TEST_IDLE,
    TEST_FINISHED,
    TEST_CW_ZERO,
    TEST_CCW_ZERO,
    TEST_CW_LONG,
    TEST_CW_SHORT,
    TEST_CCW_LONG,
    TEST_CCW_SHORT,
    TEST_START_MOVIE_CAPTURE,
    TEST_SAVE_IMAGE,
    TEST_WAIT_PHYTRON,
    TEST_WAIT_TIME,
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
