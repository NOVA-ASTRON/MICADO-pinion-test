#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <logfile.h>
#include <QWidget>

// clock stuff
#include <chrono>
#include <sstream>
#include <iomanip>


static const int num_samples=25;
auto last_measurement=std::chrono::high_resolution_clock::now();



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QString sname[num_series]={
        "torque","temp1","temp2","temp3",
    };


    ui->setupUi(this);
    chart = new QChart();
    interval=100;
    QDateTime now=QDateTime::currentDateTime();
    for (int i=0;i<num_series;i+=1){
        series[i] = new QLineSeries(chart);
        series[i]->setName(sname[i]);
        for (int j=0;j<num_samples;j++){
            QDateTime pt = now.addMSecs((j-num_samples)*interval);
            if (i==0)
             series[i]->append(pt.toMSecsSinceEpoch(),0.f);
            else
             series[i]->append(pt.toMSecsSinceEpoch(),315.f);
        }
    }

    curr_queue=0;
    sample_batch=-1;

    //chart->createDefaultAxes();
    chart_axisX = new QDateTimeAxis();
    chart->addAxis(chart_axisX,Qt::AlignBottom);
    chart_axisX->setTickCount(5);
    chart_axisX->setFormat("hh:mm:ss");
    chart_axisX->setTitleText("Time");
    chart_axisX->setRange(now.addMSecs(-num_samples*interval),now);
    //chart->setTitle("torque sensor and temperatures");
    //chart->title()->setAlignment(Qt::AlignRight);


    chart_axisY = new QtCharts::QValueAxis();
    chart->addAxis(chart_axisY,Qt::AlignLeft);
    chart_axisY->setRange(-2,2);
    chart_axisY->setTitleText("torque");

    temp_axis = new QtCharts::QValueAxis();
    chart->addAxis(temp_axis,Qt::AlignRight);
    temp_axis->setRange(80,328);
    temp_axis->setTitleText("temp K");
    for(int i=0;i<num_series;i+=1){
        chart->addSeries(series[i]);
        series[i]->attachAxis(chart_axisX);
        if (i>0)
            series[i]->attachAxis(temp_axis);
        else
            series[i]->attachAxis(chart_axisY);
        series[i]->setName(sname[i]);
    }


    //chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->layout()->setContentsMargins(0, 0, 0, 0);

    ui->TorqueChart->setChart(chart);
    num=21;
    ui->TorqueChart->setRenderHint(QPainter::Antialiasing);

    // add Phytron controller
    const optional_devs *ph=DeviceInfoFromName("phytron");
    if (ph){
        phytron_log=new LogFile_t("phytronlog.txt");
        phytron= new MotorDriver(ph,phytron_log,"phytron");

        if (phytron->getValid()){
            phytron->Init();
            ui->MotorSettings->setAutoFillBackground(true);
            ui->MotorSettings->setEnabled(true);
           // ui->camera_exp_time_spinBox
            ui->start_run_button->setEnabled(true);
        } else {
            ui->MotorSettings->setEnabled(false);
            ui->start_run_button->setText("NO Phytron connected");
            delete phytron;
            phytron=nullptr;
        }
    }

    timer = nullptr;

    //------------------------------------------------------------------------------------------------
    // setup testbench timer each 500 ms
    //------------------------------------------------------------------------------------------------
    testbench_log=nullptr;
    testbench_main_state=TEST_IDLE;
    testbench_next_state=TEST_IDLE;

    testbench_timer = new QTimer();
    test_bench_busy=false;
    connect(testbench_timer, SIGNAL(timeout()),
            this, SLOT(on_testbench_tick()));
    testbench_tick_ms=10;

    //------------------------------------------------------------------------------------------------
    // add Torque sensor
    //------------------------------------------------------------------------------------------------

    //TorqueSensorAvailable = TorqueSensor.Setup();


    updatesTimer = new QTimer(this);
    connect(updatesTimer, SIGNAL(timeout()),
            this, SLOT(on_updates_timer()));

    updatesTimer->start(50);// 20 Hz

    //------------------------------------------------------------------------------------------------
    // process data every 0.1 sec
    //------------------------------------------------------------------------------------------------

    timer = new QTimer(this);
    tcount=0;

    //------------------------------------------------------------------------------------------------
    // setup signal and slot
    //------------------------------------------------------------------------------------------------
    connect(timer, SIGNAL(timeout()),
            this, SLOT(on_timer()));
    last_measurement= std::chrono::high_resolution_clock::now();
    timer->start(interval);


    //------------------------------------------------------------------------------------------------
    // Thorlabs camera
    //------------------------------------------------------------------------------------------------
    cam_valid=cam.init_camera();
    if (cam_valid){
        cam.setExposure(100000);
        bool error = cam.setup_callbacks();
        if (!error) error= cam.start_capture();
        if (error){
            ui->image_camera_livestream->setText("!! CAMERA ERROR !!");
            cam_valid = false;
        }
    } else {

        ui->image_camera_livestream->setText("!! NO CAMERA DETECTED !!");
    }
    ui->camera_exp_time_spinBox->setEnabled(cam_valid);
    ui->video_duration_spinBox->setEnabled(cam_valid);
    ui->video_cycle_spinBox->setEnabled(cam_valid);
    ui->CaptureImagesDuringRotation->setEnabled(cam_valid);
    ui->label_9->setEnabled(cam_valid);
    ui->label_13->setEnabled(cam_valid);
    ui->label_21->setEnabled(cam_valid);

    //------------------------------------------------------------------------------------------------
    // image capture timer
    //------------------------------------------------------------------------------------------------

    if (cam_valid){
        // process data every 0.1 sec

        camtimer = new QTimer(this);

        // setup signal and slot
        connect(camtimer, SIGNAL(timeout()),
                this, SLOT(on_camera()));
        camtimer->start(20);
    }

    //------------------------------------------------------------------------------------------------
    ui->label_8->setEnabled(false);
    ui->revolutions_counter->setEnabled(false);

    //------------------------------------------------------------------------------------------------
    // start background thread
    //------------------------------------------------------------------------------------------------

    backgroundMeasurements.start();

    //------------------------------------------------------------------------------------------------
    // cleanup threads and timers when we are done
    //------------------------------------------------------------------------------------------------
    connect(this, SIGNAL(destroyed()), timer, SLOT(stop()));
    connect(this, SIGNAL(destroyed()), updatesTimer, SLOT(stop()));
}

MainWindow::~MainWindow()
{
    backgroundMeasurements.exit = true;
    backgroundMeasurements.wait();
    if(phytron) delete phytron;
    if (phytron_log) delete phytron_log;
    delete ui;
}

void MainWindow::update_monitoring()
{
    // hopefully this has no real impact on the measurement thread.

    if (backgroundMeasurements.running) return;

    QLCDNumber * ref[]={
        nullptr,
        ui->temp_sensor_1_lcdNumber,
        ui->temp_sensor_2_lcdNumber,
        ui->temp_sensor_3_lcdNumber,
    };

    for (int tsensor=1;tsensor<4;tsensor+=1){
        ref[tsensor]->display(backgroundMeasurements.curr_sample.val[tsensor]*24.0+93.15);
    }
    ui->microseconds_current->setText(QString::number((double)backgroundMeasurements.avg_usec/1000.0));


    if (phytron) {

        QString a_value = QString::number(phytron->ReadEncoderA());
        QString b_value = QString::number(phytron->ReadEncoderB());
        ui->encoderAValue->setText(a_value);
        ui->encoderBValue->setText(b_value);
    }
    if (testbench_timer->isActive()&&testbench_main_state==TEST_FINISHED){
        ui->start_run_button->setText("Done. press to start again.");
        testbench_timer->stop();
        testbench_main_state=TEST_IDLE;
    }

}
static int seq=0;
void MainWindow::update_cam_image()
{
    if (cam_valid){
        QImage * im = cam.getImage();
        if (im){

            if (saveImage) {
                QString suggestion = imgDir+"/Image_"+ QDateTime::currentDateTimeUtc().toString("yyyy_MM_dd_hh_mm_ss_zzz_") + QString::number(seq++) +".bmp";
                //QFile savefile(suggestion);
                //savefile.open(OpenMode::Create)
                im->save(suggestion);
            } else ui->image_camera_livestream->setPixmap(QPixmap::fromImage(*im,Qt::AutoColor));
        }
    }
}


void MainWindow::on_led_intensity_horizontalSlider_valueChanged(int value)
{
    //update_monitoring();
}
using Clock = std::chrono::high_resolution_clock;
static std::string timePointToString(const Clock::time_point &tp, const std::string &format, bool withMs = true, bool utc = true)
{
    const Clock::time_point::duration tt = tp.time_since_epoch();
    const time_t durS = std::chrono::duration_cast<std::chrono::seconds>(tt).count();
    std::stringstream ss;
    if (const std::tm *tm = (utc ? std::gmtime(&durS) : std::localtime(&durS))) {
        ss << std::put_time(tm, format.c_str());
        if (withMs) {
            const long long durMs = std::chrono::duration_cast<std::chrono::milliseconds>(tt).count();
            ss << std::setw(3) << std::setfill('0') << int(durMs - durS * 1000);
        }
    }
    // gmtime/localtime() returned null ?
    else {
        ss << "<FORMAT ERROR>";
    }
    return ss.str();
}
void MainWindow::on_start_run_button_clicked()
{
    if (testbench_timer->isActive()){
        saveImage=false;
        testbench_timer->stop();
        ui->label_8->setEnabled(false);
        ui->revolutions_counter->setEnabled(false);
        if (phytron) phytron->Move(0); // stop?
        if (testbench_log){
            delete testbench_log;
            testbench_log=nullptr;
        }

        ui->start_run_button->setText("START");
    } else
    if (phytron_log && phytron){
        vidCycles=0;

        ui->revolutions_counter->setValue(0);
        ui->revolutions_counter->setEnabled(true);
        ui->label_8->setEnabled(true);

        if (testbench_log){
            delete testbench_log;
            testbench_log=nullptr;
        }

        const auto tp = Clock::now();
        std::string timestamp=timePointToString(tp,"%Y%m%d%H%M%S");
        QString filehint = "bearingtestdata_"+QString::fromStdString(timestamp)+".csv";
        QString filter ="*.csv";
        QString filename = 	QFileDialog::getSaveFileName(this,"Save CSV data to file", filehint, filter);
        if (filename!=""){
            QFileInfo fi(filename);
            imgDir = fi.absolutePath();
            testbench_log=new LogFile_t(filename.toStdString());
            if (testbench_log){
                testbench_main_state=ui->motor_anti_CW_radioButton->isChecked()?TEST_CCW_LONG:TEST_CW_LONG;
                testbench_total_num_revs=0;
                testbench_timer->start(testbench_tick_ms);
                ui->start_run_button->setText("STOP");
            }
        }
    }
}

void MainWindow::on_led_intensity_horizontalSlider_2_valueChanged(int value)
{
    //update_monitoring();
}
void MainWindow::on_timer()
{
    if (saveImage) {
        auto time_running = QDateTime::currentSecsSinceEpoch()-vidStartTime.toSecsSinceEpoch();
        if (time_running>=ui->video_duration_spinBox->value()) {
            saveImage = false;
        }
    }
}

void MainWindow::on_updates_timer()
{
    update_monitoring();
}

void MainWindow::on_camera()
{
    update_cam_image();
}

void MainWindow::on_testbench_tick()
{
    if (test_bench_busy) return;
    test_bench_busy=true;
    switch(testbench_main_state){
    case TEST_IDLE:
        break;
    case TEST_FINISHED:
        break;
    case TEST_CCW_ZERO:
        backgroundMeasurements.start_measurements=true;
        if (phytron){
            phytron->MoveCCWzero();
        }
        if (ui->motor_CW_radioButton->isChecked())
            testbench_next_state=TEST_CW_LONG;
        else
            testbench_next_state=TEST_CCW_LONG;
        break;
    case TEST_CW_ZERO:
        backgroundMeasurements.start_measurements=true;
        if (phytron){
            phytron->MoveCWzero();
        }
        if (ui->motor_CW_radioButton->isChecked())
            testbench_next_state=TEST_CW_LONG;
        else
            testbench_next_state=TEST_CCW_LONG;
        break;
    case TEST_CW_LONG:
    case TEST_CCW_LONG:
        backgroundMeasurements.start_measurements=true;
        if (phytron){
            int steps =ui->motor_revolutions_per_direction_spinBox->value()*200;
            if (testbench_main_state==TEST_CCW_LONG) steps=-steps;
            phytron->Move(steps);

            if (cam_valid && ui->CaptureImagesDuringRotation->isChecked()){
                if ((vidCycles%ui->video_cycle_spinBox->value())==0){
                    vidStartTime=QDateTime::currentDateTime();
                    //saveImage=true;
                    movie_capture_busy=true;
                    cam.start_movie_capture();
                }
                vidCycles++;
            }
        }

        testbench_total_num_revs+=ui->motor_revolutions_per_direction_spinBox->value();

            if (ui->motor_CW_radioButton->isChecked())
                testbench_next_state=TEST_CW_LONG;
            else {
                if (ui->motor_alternate_direction_radioButton->isChecked()) {
                    testbench_next_state=(testbench_main_state==TEST_CW_LONG)?TEST_CCW_LONG:TEST_CW_LONG;
                } else {
                    // radio button so no need to check..
                    testbench_next_state=TEST_CCW_LONG;
                }
            }
        num_not_changed=0;
        testbench_main_state=TEST_WAIT_PHYTRON;
        break;
    case TEST_WAIT_PHYTRON:
        // we should make sure that reading out the phytron is done
        // without interfering the readout of the position..
        if (movie_capture_busy) {
            if (std::abs(vidStartTime.msecsTo(QDateTime::currentDateTime()))>=(ui->video_duration_spinBox->value()*1000)){
                cam.stop_movie_capture();
                movie_capture_busy=false;
                movie_capture_save=true;
            }
        }
        if (phytron){
            backgroundMeasurements.phytron_position=phytron->ReadEncoderA();
            if (last_phytron!=backgroundMeasurements.phytron_position){
                num_not_changed=0;
                last_phytron=backgroundMeasurements.phytron_position;
                break;
            }
            num_not_changed+=1;
            if (num_not_changed<10) break;
        }
        if (movie_capture_busy) {
            cam.stop_movie_capture();
            movie_capture_busy=false;
            movie_capture_save=true;
        }
        if (backgroundMeasurements.running) {
            backgroundMeasurements.stop_measurements=true;
            break;
        }
        // update progress
        ui->revolutions_counter->setValue(testbench_total_num_revs * (ui->revolutions_counter->maximum()-1) / ui->motor_total_revolutions_spinBox->value());
        // wait if necessary
        testbench_main_state=TEST_WAIT_TIME;
        testbench_wait_start = QDateTime::currentDateTime();
        testbench_waited_ms = testbench_tick_ms;
        break;
    case TEST_WAIT_TIME:
        if (backgroundMeasurements.num_measurements>0){
            // save our test data
            if (testbench_log){
                store_measurements(testbench_log,backgroundMeasurements.measurements,backgroundMeasurements.num_measurements);
                if (testbench_next_state==TEST_FINISHED)
                    testbench_log->close();
            }
            backgroundMeasurements.num_measurements=0;
        }

        if (movie_capture_save){
            movie_capture_save=false;
            store_movie();
        }
        testbench_waited_ms = std::abs(testbench_wait_start.msecsTo(QDateTime::currentDateTime()));
        if (testbench_waited_ms>=ui->pause_time_spinBox->value()*1000){
            testbench_main_state=testbench_next_state;
            if (testbench_total_num_revs>=ui->motor_total_revolutions_spinBox->value())
                testbench_main_state=TEST_FINISHED;
        }
        break;
    default:
        testbench_main_state=TEST_FINISHED;
        break;
    }
    test_bench_busy=false;
}

void MainWindow::on_camera_exp_time_spinBox_valueChanged(int arg1)
{
    cam.setExposure(arg1*1000);
}

void MainWindow::on_sampling_time_editingFinished()
{
    //timer->stop();

    interval = ui->sampling_time->value();
    //timer->start(ui->sampling_time->value());
}

void MainWindow::store_measurements(LogFile  where, sample *samp, int num_samples)
{
    if (where) {
        for (int s=0;s<num_samples;s+=1) {
            std::stringstream ss;
            ss << samp[s].stamp << ",";
            for (int i=0;i<4;i+=1)
                ss << samp[s].val[i] << ",";
            ss << samp[s].phytron_pos << ",";
            ss << (int) testbench_main_state ;
            where->Write(ss.str());
        }
        where->sync();
    }
}

void MainWindow::store_movie()
{
    for (int i=0;i<cam.getMovieSize();i+=1){
        QImage * frame = cam.getMovieFrame(i);
        QDateTime stamp = cam.getMovieStamp(i);
        if (frame){
            QString frame_name = imgDir + "/imgframe" + QString::number(stamp.toMSecsSinceEpoch()) +".png";
            frame->save(frame_name,"PNG");
        }
    }

}

void MainWindow::on_doCCWindex_clicked()
{
    if (phytron) {
        phytron->MoveCCWzero();
    }
}

void MainWindow::on_doCWindex_clicked()
{
    if (phytron) {
        phytron->MoveCWzero();
    }
}
