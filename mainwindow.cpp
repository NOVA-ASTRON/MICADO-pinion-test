#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
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

    // add Phytron controller
    const optional_devs *ph=DeviceInfoFromName("phytron");
    if (ph){
        phytron_log=new LogFile_t("phytronlog.txt");
        phytron= new MotorDriver(ph,phytron_log,"phytron");



        if (phytron->getValid()){
            phytron->setDebug(false);
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

#ifdef USE_UPDATES_TIMER
    updatesTimer = new QTimer(this);
    connect(updatesTimer, SIGNAL(timeout()),
            this, SLOT(on_updates_timer()));

    updatesTimer->start(50);// 20 Hz
    connect(this, SIGNAL(destroyed()), updatesTimer, SLOT(stop()));
#endif

    //------------------------------------------------------------------------------------------------
    // process data every 0.1 sec
    //------------------------------------------------------------------------------------------------
#ifdef USE_TIMER_TWO
    timer = new QTimer(this);
    tcount=0;

    //------------------------------------------------------------------------------------------------
    // setup signal and slot
    //------------------------------------------------------------------------------------------------


    connect(timer, SIGNAL(timeout()),
            this, SLOT(on_timer()));
    last_measurement= std::chrono::high_resolution_clock::now();
    timer->start(interval);
    connect(this, SIGNAL(destroyed()), timer, SLOT(stop()));

#endif

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


    //------------------------------------------------------------------------------------------------
    // image capture timer
    //------------------------------------------------------------------------------------------------

    if (cam_valid){
        // process data every 0.1 sec

        camtimer = new QTimer(this);

        // setup signal and slot
        connect(camtimer, SIGNAL(timeout()),
                this, SLOT(on_camera()));
        camtimer->start(50);
    }

    //------------------------------------------------------------------------------------------------
    // disable camera settings if no camera
    //------------------------------------------------------------------------------------------------

    ui->camera_exp_time_spinBox->setEnabled(cam_valid);
    ui->video_duration_spinBox->setEnabled(cam_valid);
    ui->video_cycle_spinBox->setEnabled(cam_valid);
    ui->CaptureImagesDuringRotation->setEnabled(cam_valid);
    ui->label_9->setEnabled(cam_valid);
    ui->label_13->setEnabled(cam_valid);
    ui->label_21->setEnabled(cam_valid);


    //------------------------------------------------------------------------------------------------
    ui->label_8->setEnabled(false);
    ui->revolutions_counter->setEnabled(false);

    //------------------------------------------------------------------------------------------------
    // start background thread
    //------------------------------------------------------------------------------------------------

    backgroundMeasurements.setRequested_sample_time(ui->sampling_time->value());
    backgroundMeasurements.setDebug(true);
    backgroundMeasurements.start();

    //------------------------------------------------------------------------------------------------
    // cleanup threads and timers when we are done
    //------------------------------------------------------------------------------------------------

    testbench_timer->start(testbench_tick_ms);
    Debug=false;
}

MainWindow::~MainWindow()
{
    std::cerr << " MANWINDOW EXIT!\n";
    backgroundMeasurements.exit = true;
    backgroundMeasurements.wait();
    if(phytron) delete phytron;
    if (phytron_log) delete phytron_log;
    delete ui;
}

void MainWindow::update_monitoring()
{
    // hopefully this has no real impact on the measurement thread.
    if (Debug) std::cerr <<"STARTING update_monitoring()\n";

    backgroundMeasurements.StampWD();
    if (phytron){

        phytron->ReadStatus();


        if (!M0Pachieved && phytron->isM0P()){
            M0Pachieved=true;
            QPalette pa = ui->doCWindex->palette();
            pa.setColor(QPalette::Button,QColor(Qt::green));
            ui->doCWindex->setAutoFillBackground(true);
            ui->doCWindex->setPalette(pa);
            ui->doCWindex->update();
        }
        if (M0Pachieved && !phytron->isM0P()){
            M0Pachieved=false;
            QPalette pa = ui->doCWindex->palette();
            pa.setColor(QPalette::Button,QColor(Qt::red));
            ui->doCWindex->setAutoFillBackground(true);
            ui->doCWindex->setPalette(pa);
            ui->doCWindex->update();
        }
    }


    QLCDNumber * ref[]={
        nullptr,
        ui->temp_sensor_1_lcdNumber,
        ui->temp_sensor_2_lcdNumber,
        ui->temp_sensor_3_lcdNumber,
    };

    for (int tsensor=1;tsensor<4;tsensor+=1){
        ref[tsensor]->display(backgroundMeasurements.curr_sample.val[tsensor]*24.0+73.15);
    }
    ui->microseconds_current->setText(QString::number((double)backgroundMeasurements.avg_usec/1000.0));


    if (phytron) {
        //if (phytron->ReadStatus()){
            QString a_value = phytron->IsMotorRunning("Is motor running in update_monitoring()")?"Running":"Stopped";// QString::number(phytron->ReadEncoderA());
            ui->encoderAValue->setText(a_value);
        //}

        if (phytron->ReadPosA(rack_pos)) {
            QString b_value = QString::number(rack_pos);
            ui->encoderBValue->setText(b_value);
        } else ui->encoderBValue->setText("--??--");

    }
    if (testbench_timer && testbench_timer->isActive()&&testbench_main_state==TEST_FINISHED){
        ui->start_run_button->setText("Done. press to start again.");
        //testbench_timer->stop();
        STOP_testbench=true;
        testbench_main_state=TEST_IDLE;
    }
    if (Debug) std::cerr <<"DONE update_monitoring()\n";

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
    if (testbench_timer && testbench_timer->isActive()&&testbench_main_state!=TEST_IDLE){
        saveImage=false;
        STOP_testbench=true;
        //testbench_timer->stop();
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

            if (backgroundMeasurements.isRunning()==false){
                backgroundMeasurements.StampWD();

                backgroundMeasurements.start();
            }

            /*
            if (testbench_log){
                testbench_main_state=ui->motor_anti_CW_radioButton->isChecked()?TEST_CCW_LONG:TEST_CW_LONG;
                testbench_total_num_revs=0;
                testbench_timer->start(testbench_tick_ms);
                ui->start_run_button->setText("STOP");
            }*/

            if (testbench_log){
                testbench_main_state=TEST_MOVE_ZERO; // this will zero the setup and then start with move forward.
                testbench_total_num_revs=0;
                //testbench_timer->start(testbench_tick_ms);
                ui->start_run_button->setText("STOP");
            }
        }
    }
}

void MainWindow::on_timer()
{
    if (Debug)     std::cerr <<"STARTING on_timer()\n";
    if (saveImage) {
        auto time_running = QDateTime::currentSecsSinceEpoch()-vidStartTime.toSecsSinceEpoch();
        if (time_running>=ui->video_duration_spinBox->value()) {
            //saveImage = false;
            printf("No image saved! \r\n");
        }
    }
    if (Debug)  std::cerr <<"DONE on_timer()\n";
}

void MainWindow::on_updates_timer()
{
    if (Debug) std::cerr <<"STARTING on_updates_timer()\n";

    backgroundMeasurements.StampWD();
    if (updates_busy) return;
    updates_busy=true;
    if(!test_bench_busy) update_monitoring();
    updates_busy=false;
    if (Debug) std::cerr <<"DONE on_updates_timer()\n";
}

void MainWindow::on_camera()
{
    if (Debug) std::cerr <<"STARTING on_camera()\n";
    update_cam_image();
    if (Debug) std::cerr <<"Done on_camera()\n";
}

void MainWindow::on_testbench_tick()
{
    if (Debug) std::cerr <<"STARTING on_testbench_tick()\n";
    sync_cnt=sync_cnt+1;
    if (test_bench_busy) return;
    if (updates_busy) return;
    test_bench_busy=true;

    if (Debug) std::cerr <<"DOING on_testbench_tick()\n";
    if(sync_cnt>=5) {
        update_monitoring();
        sync_cnt=0;
    }

    if (STOP_testbench){
        testbench_main_state=TEST_IDLE;
        STOP_testbench=false;
    }

    switch(testbench_main_state){
    case TEST_IDLE:
        break;
    case TEST_FINISHED:
        if (Debug) std::cerr << "TestBench: Finished\n";
        break;
    case TEST_CCW_ZERO:
    case TEST_CW_ZERO:
    case TEST_CW_LONG:
    case TEST_CCW_LONG:
        std::cerr << "TestBench: Unsupported State\n";
        testbench_main_state=TEST_FINISHED;
        break;
    case TEST_MOVE_FORWARD:
    case TEST_MOVE_BACKWARDS:
        if (Debug) std::cerr << "TestBench: TEST_MOVE_FORWARD/TEST_MOVE_BACKWARDS\n";
        backgroundMeasurements.start_measurements=true;
        if (phytron){
            int steps = ui->motor_revolutions_per_direction_spinBox->value();//ui->motor_revolutions_per_direction_spinBox->value()*200*1.591549431;
                std::stringstream ss;
                ss << "Step at: " << testbench_total_num_revs+1 << "\n";
                if (phytron_log) phytron_log->Write(ss.str());
            if (testbench_main_state==TEST_MOVE_BACKWARDS)
                steps=0;
            phytron->GoToPos(steps); // absolute position in mm

            if (cam_valid && ui->CaptureImagesDuringRotation->isChecked()){
                if ((vidCycles%ui->video_cycle_spinBox->value())==0){
                    vidStartTime=QDateTime::currentDateTime();
                    //saveImage=true;
                    movie_capture_busy=true;
                    cam.start_movie_capture();
                    printf("Movie capture started! \r\n");
                }
                vidCycles++;
            }
        }

        testbench_total_num_revs+=1;
        testbench_next_state=(testbench_main_state==TEST_MOVE_FORWARD)?TEST_MOVE_BACKWARDS:TEST_MOVE_FORWARD;


        num_not_changed=0;
        testbench_main_state=TEST_WAIT_PHYTRON;
        if (Debug) std::cerr << "TestBench: Done TEST_MOVE_FORWARD/TEST_MOVE_BACKWARDS\n";

        break;

    case TEST_MOVE_ZERO:
         if (phytron){

             phytron->DoMOPmin();
             testbench_next_state=TEST_MOVE_FORWARD;
             testbench_main_state=TEST_WAIT_PHYTRON;
         } else {
             testbench_main_state=TEST_FINISHED;
         }
        break;

    case TEST_WAIT_PHYTRON:
        if (Debug) std::cerr << "TEST_WAIT_PHYTRON started\n";
        // we should make sure that reading out the phytron is done
        // without interfering the readout of the position..
        if (movie_capture_busy) {
            if (std::abs(vidStartTime.msecsTo(QDateTime::currentDateTime()))>=(ui->video_duration_spinBox->value()*1000)){
                cam.stop_movie_capture();
                movie_capture_busy=false;
                movie_capture_save=true;
                printf("Movie capture is finished, movie can be saved now (duration of video is reached)! \r\n");
            }
        }
        if (phytron){
            if (phytron->ReadPosA(rack_pos))
                backgroundMeasurements.phytron_position=rack_pos;
            /*
            if (last_phytron!=backgroundMeasurements.phytron_position){
                num_not_changed=0;
                last_phytron=backgroundMeasurements.phytron_position;
                break;
            }
            num_not_changed+=1;
            */
            if (phytron->IsMotorRunning("Is motor running during TEST_WAIT_PHYTRON")) break;
        }

        // The movement has finished here

        if (movie_capture_busy) {
            cam.stop_movie_capture();
            movie_capture_busy=false;
            movie_capture_save=true;
            printf("Movie capture is finished, movie can be saved now! \r\n");
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
        if (Debug) std::cerr << "TEST_WAIT_PHYTRON finished\n";
        break;
    case TEST_WAIT_TIME:
        if (Debug) std::cerr << "TEST_WAIT_TIME iteration\n";
        if (phytron && phytron->IsMotorRunning("is motor running during TEST_WAIT_TIME")) break;

        if (backgroundMeasurements.num_measurements>0){
            // save our test data
            std::cerr << "TEST_WAIT_TIME save data\n";
            if (testbench_log){
                store_measurements(testbench_log,backgroundMeasurements.measurements,backgroundMeasurements.num_measurements);
            }
            backgroundMeasurements.num_measurements=0;
            std::cerr << "TEST_WAIT_TIME save data DONE\n";
        } else std::cerr << "TEST_WAIT_TIME backgroundMeasurements.num_measurements=0\n";

        if (movie_capture_save){
            movie_capture_save=false;
            store_movie();
            printf("Movie is stored! \r\n");
        }
        testbench_waited_ms = std::abs(testbench_wait_start.msecsTo(QDateTime::currentDateTime()));
        if (testbench_waited_ms>=ui->pause_time_spinBox->value()*1000){
            testbench_main_state=testbench_next_state;
            if (Debug) std::cerr << "TEST_WAIT_TIME finished\n";
            if (testbench_total_num_revs>=ui->motor_total_revolutions_spinBox->value()) {
                testbench_main_state = TEST_FINISHED;
                testbench_log->close();
            }
        }
        break;
    default:
        testbench_main_state=TEST_FINISHED;
        break;
    }
    test_bench_busy=false;
    if (Debug) std::cerr <<"DONE on_testbench_tick()\n";
}

void MainWindow::on_camera_exp_time_spinBox_valueChanged(int arg1)
{
    if (cam_valid) cam.setExposure(arg1*1000);
}

void MainWindow::on_sampling_time_editingFinished()
{
    //timer->stop();

    interval = ui->sampling_time->value();
    backgroundMeasurements.setRequested_sample_time(interval*1000);
    //timer->start(ui->sampling_time->value());
}

void MainWindow::store_measurements(LogFile  where, sample *samp, int num_samples)
{
    if (where && samp) {
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
    if (cam_valid){
        for (int i=0;i<cam.getMovieSize();i+=1){
            QImage * frame = cam.getMovieFrame(i);
            QDateTime stamp = cam.getMovieStamp(i);
            if (frame){
                QString frame_name = imgDir + "/imgframe" + QString::number(stamp.toMSecsSinceEpoch()) +".png";
                frame->save(frame_name,"PNG");
                printf("Image is saved! \r\n");
            }
        }
    }

}

void MainWindow::on_doCCWindex_clicked()
{
    if (phytron) {
        phytron->DoMOPplus();
        ui->GoTenBackwards->setEnabled(false);
        ui->GoTenForward->setEnabled(false);

    }
}

void MainWindow::on_doCWindex_clicked()
{
    if (phytron) {
        phytron->DoMOPmin();
        ui->GoTenBackwards->setEnabled(true);
        ui->GoTenForward->setEnabled(true);

    }
}

void MainWindow::on_GoTenForward_clicked()
{
    if (phytron) {
        phytron->GoToPos(ui->motor_revolutions_per_direction_spinBox->value());
    }
}

void MainWindow::on_GoTenBackwards_clicked()
{
    if (phytron) {
        phytron->GoToPos(0);
    }
}
