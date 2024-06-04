#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "analogdaq.h"
#include "pythronmotordriver.h"
#include "thorlabscamera.h"
#include "threadedtimer.h"

#include <QtCharts>
#include <QLineSeries>
#include <QLineSeries>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



/*
typedef struct {
    QDateTime stamp;
    qreal val[4];
} sample;
*/
const int num_series = 4;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void update_monitoring();
    void update_cam_image();

private slots:
    void on_led_intensity_horizontalSlider_valueChanged(int value);

    void on_start_run_button_clicked();

    void on_led_intensity_horizontalSlider_2_valueChanged(int value);

    void on_timer();
    void on_updates_timer();
    void on_camera();
    void on_testbench_tick();

    void on_camera_exp_time_spinBox_valueChanged(int arg1);

    void on_sampling_time_editingFinished();

    void on_doCCWindex_clicked();

    void on_doCWindex_clicked();

private:
    void store_measurements(LogFile where, sample *samp, int num_samples);
    void store_movie();
    Ui::MainWindow *ui;
    QLineSeries *series[num_series];
    QChart *chart;
    int num;
    qreal min,max;
    MotorDriver *phytron;
    LogFile phytron_log;
    LogFile ledcontrol_log;
    AnalogDAQ TorqueSensor;
    bool TorqueSensorAvailable;

    QtCharts::QDateTimeAxis *chart_axisX;
    QtCharts::QValueAxis *chart_axisY,*temp_axis;
    QPixmap camera_pixmap;
    ThorlabsCamera cam;
    bool cam_valid;
    volatile bool saveImage;
    QString imgDir;
    QDateTime vidStartTime;
    QDateTime testbench_wait_start;
    int vidCycles;
    QTimer *timer,*camtimer,*testbench_timer,*updatesTimer;
    threadedTimer backgroundMeasurements;
    int testbench_total_num_revs;
    int testbench_waited_ms;
    int testbench_tick_ms;
    volatile bool testbench_stop;
    int tcount;
    LogFile testbench_log;
    volatile int curr_queue,sample_batch;
    sample measurements[4][200];
    int batch_size[4];

    int interval;
    int num_not_changed;
    int last_phytron;
    volatile bool test_bench_busy=false;
    volatile bool movie_capture_busy=false;
    volatile bool movie_capture_save=false;

    testbench_states testbench_main_state;
    testbench_states testbench_next_state;

};
#endif // MAINWINDOW_H
