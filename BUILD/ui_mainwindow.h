/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *MotorSettings;
    QSplitter *splitter_8;
    QLabel *label_19;
    QSpinBox *motor_revolutions_per_direction_spinBox;
    QSplitter *splitter;
    QLabel *label_2;
    QSpinBox *motor_total_revolutions_spinBox;
    QSplitter *splitter_2;
    QLabel *label_4;
    QSpinBox *pause_time_spinBox;
    QSplitter *splitter_5;
    QSplitter *splitter_4;
    QLabel *label_3;
    QSpinBox *motor_speed_spinBox;
    QSplitter *splitter_3;
    QLabel *label_5;
    QSpinBox *motor_acceleration_spinBox;
    QRadioButton *motor_anti_CW_radioButton;
    QRadioButton *motor_CW_radioButton;
    QRadioButton *motor_alternate_direction_radioButton;
    QGroupBox *groupBox_2;
    QSlider *led_intensity_horizontalSlider;
    QSplitter *splitter_9;
    QLabel *label_21;
    QSpinBox *camera_exp_time_spinBox;
    QLabel *label_20;
    QSplitter *splitter_7;
    QLabel *label_9;
    QSpinBox *video_duration_spinBox;
    QSlider *led_intensity_horizontalSlider_2;
    QLabel *label_22;
    QLabel *image_camera_livestream;
    QCheckBox *CaptureImagesDuringRotation;
    QLabel *label_13;
    QSpinBox *video_cycle_spinBox;
    QGroupBox *groupBox_3;
    QPushButton *start_run_button;
    QSplitter *splitter_6;
    QLabel *label_8;
    QProgressBar *revolutions_counter;
    QPushButton *doCCWindex;
    QPushButton *doCWindex;
    QLabel *encoderAValue;
    QLabel *encoderBValue;
    QGroupBox *groupBox_4;
    QChartView *TorqueChart;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_10;
    QLCDNumber *temp_sensor_1_lcdNumber;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_11;
    QLCDNumber *temp_sensor_2_lcdNumber;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_12;
    QLCDNumber *temp_sensor_3_lcdNumber;
    QSplitter *splitter_10;
    QLabel *label_23;
    QSpinBox *sampling_time;
    QLabel *microseconds_current;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1828, 1031);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        MotorSettings = new QGroupBox(centralwidget);
        MotorSettings->setObjectName(QStringLiteral("MotorSettings"));
        MotorSettings->setGeometry(QRect(20, 10, 501, 601));
        splitter_8 = new QSplitter(MotorSettings);
        splitter_8->setObjectName(QStringLiteral("splitter_8"));
        splitter_8->setEnabled(true);
        splitter_8->setGeometry(QRect(20, 120, 460, 80));
        splitter_8->setOrientation(Qt::Vertical);
        splitter_8->setChildrenCollapsible(false);
        label_19 = new QLabel(splitter_8);
        label_19->setObjectName(QStringLiteral("label_19"));
        QFont font;
        font.setPointSize(12);
        label_19->setFont(font);
        splitter_8->addWidget(label_19);
        motor_revolutions_per_direction_spinBox = new QSpinBox(splitter_8);
        motor_revolutions_per_direction_spinBox->setObjectName(QStringLiteral("motor_revolutions_per_direction_spinBox"));
        motor_revolutions_per_direction_spinBox->setFont(font);
        motor_revolutions_per_direction_spinBox->setAlignment(Qt::AlignCenter);
        motor_revolutions_per_direction_spinBox->setMaximum(99999999);
        motor_revolutions_per_direction_spinBox->setValue(5);
        splitter_8->addWidget(motor_revolutions_per_direction_spinBox);
        splitter = new QSplitter(MotorSettings);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setEnabled(true);
        splitter->setGeometry(QRect(20, 30, 460, 80));
        splitter->setOrientation(Qt::Vertical);
        splitter->setChildrenCollapsible(false);
        label_2 = new QLabel(splitter);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);
        splitter->addWidget(label_2);
        motor_total_revolutions_spinBox = new QSpinBox(splitter);
        motor_total_revolutions_spinBox->setObjectName(QStringLiteral("motor_total_revolutions_spinBox"));
        motor_total_revolutions_spinBox->setFont(font);
        motor_total_revolutions_spinBox->setAlignment(Qt::AlignCenter);
        motor_total_revolutions_spinBox->setMinimum(0);
        motor_total_revolutions_spinBox->setMaximum(160000);
        motor_total_revolutions_spinBox->setValue(50);
        splitter->addWidget(motor_total_revolutions_spinBox);
        splitter_2 = new QSplitter(MotorSettings);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setGeometry(QRect(20, 220, 460, 80));
        splitter_2->setOrientation(Qt::Vertical);
        splitter_2->setChildrenCollapsible(false);
        label_4 = new QLabel(splitter_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font);
        splitter_2->addWidget(label_4);
        pause_time_spinBox = new QSpinBox(splitter_2);
        pause_time_spinBox->setObjectName(QStringLiteral("pause_time_spinBox"));
        pause_time_spinBox->setFont(font);
        pause_time_spinBox->setAlignment(Qt::AlignCenter);
        pause_time_spinBox->setMaximum(1300000);
        pause_time_spinBox->setValue(3);
        splitter_2->addWidget(pause_time_spinBox);
        splitter_5 = new QSplitter(MotorSettings);
        splitter_5->setObjectName(QStringLiteral("splitter_5"));
        splitter_5->setGeometry(QRect(20, 310, 460, 161));
        splitter_5->setOrientation(Qt::Vertical);
        splitter_5->setChildrenCollapsible(false);
        splitter_4 = new QSplitter(splitter_5);
        splitter_4->setObjectName(QStringLiteral("splitter_4"));
        splitter_4->setOrientation(Qt::Vertical);
        splitter_4->setChildrenCollapsible(false);
        label_3 = new QLabel(splitter_4);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);
        splitter_4->addWidget(label_3);
        motor_speed_spinBox = new QSpinBox(splitter_4);
        motor_speed_spinBox->setObjectName(QStringLiteral("motor_speed_spinBox"));
        motor_speed_spinBox->setFont(font);
        motor_speed_spinBox->setAlignment(Qt::AlignCenter);
        motor_speed_spinBox->setValue(2);
        splitter_4->addWidget(motor_speed_spinBox);
        splitter_5->addWidget(splitter_4);
        splitter_3 = new QSplitter(splitter_5);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setOrientation(Qt::Vertical);
        splitter_3->setChildrenCollapsible(false);
        label_5 = new QLabel(splitter_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setFont(font);
        splitter_3->addWidget(label_5);
        motor_acceleration_spinBox = new QSpinBox(splitter_3);
        motor_acceleration_spinBox->setObjectName(QStringLiteral("motor_acceleration_spinBox"));
        motor_acceleration_spinBox->setFont(font);
        motor_acceleration_spinBox->setAlignment(Qt::AlignCenter);
        motor_acceleration_spinBox->setValue(5);
        splitter_3->addWidget(motor_acceleration_spinBox);
        splitter_5->addWidget(splitter_3);
        motor_anti_CW_radioButton = new QRadioButton(MotorSettings);
        motor_anti_CW_radioButton->setObjectName(QStringLiteral("motor_anti_CW_radioButton"));
        motor_anti_CW_radioButton->setGeometry(QRect(20, 540, 460, 20));
        QFont font1;
        font1.setPointSize(10);
        motor_anti_CW_radioButton->setFont(font1);
        motor_CW_radioButton = new QRadioButton(MotorSettings);
        motor_CW_radioButton->setObjectName(QStringLiteral("motor_CW_radioButton"));
        motor_CW_radioButton->setGeometry(QRect(20, 510, 460, 20));
        motor_CW_radioButton->setFont(font1);
        motor_alternate_direction_radioButton = new QRadioButton(MotorSettings);
        motor_alternate_direction_radioButton->setObjectName(QStringLiteral("motor_alternate_direction_radioButton"));
        motor_alternate_direction_radioButton->setGeometry(QRect(20, 480, 460, 20));
        motor_alternate_direction_radioButton->setFont(font1);
        motor_alternate_direction_radioButton->setChecked(true);
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(540, 10, 1281, 491));
        led_intensity_horizontalSlider = new QSlider(groupBox_2);
        led_intensity_horizontalSlider->setObjectName(QStringLiteral("led_intensity_horizontalSlider"));
        led_intensity_horizontalSlider->setGeometry(QRect(20, 390, 400, 22));
        led_intensity_horizontalSlider->setOrientation(Qt::Horizontal);
        splitter_9 = new QSplitter(groupBox_2);
        splitter_9->setObjectName(QStringLiteral("splitter_9"));
        splitter_9->setGeometry(QRect(20, 40, 400, 80));
        splitter_9->setOrientation(Qt::Vertical);
        label_21 = new QLabel(splitter_9);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setFont(font);
        splitter_9->addWidget(label_21);
        camera_exp_time_spinBox = new QSpinBox(splitter_9);
        camera_exp_time_spinBox->setObjectName(QStringLiteral("camera_exp_time_spinBox"));
        camera_exp_time_spinBox->setFont(font);
        camera_exp_time_spinBox->setAlignment(Qt::AlignCenter);
        camera_exp_time_spinBox->setMaximum(999999);
        camera_exp_time_spinBox->setValue(100);
        splitter_9->addWidget(camera_exp_time_spinBox);
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(20, 350, 400, 33));
        label_20->setFont(font);
        label_20->setAlignment(Qt::AlignCenter);
        splitter_7 = new QSplitter(groupBox_2);
        splitter_7->setObjectName(QStringLiteral("splitter_7"));
        splitter_7->setGeometry(QRect(20, 120, 400, 80));
        splitter_7->setOrientation(Qt::Vertical);
        label_9 = new QLabel(splitter_7);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font);
        splitter_7->addWidget(label_9);
        video_duration_spinBox = new QSpinBox(splitter_7);
        video_duration_spinBox->setObjectName(QStringLiteral("video_duration_spinBox"));
        video_duration_spinBox->setFont(font);
        video_duration_spinBox->setAlignment(Qt::AlignCenter);
        video_duration_spinBox->setValue(5);
        splitter_7->addWidget(video_duration_spinBox);
        led_intensity_horizontalSlider_2 = new QSlider(groupBox_2);
        led_intensity_horizontalSlider_2->setObjectName(QStringLiteral("led_intensity_horizontalSlider_2"));
        led_intensity_horizontalSlider_2->setGeometry(QRect(20, 460, 400, 22));
        led_intensity_horizontalSlider_2->setOrientation(Qt::Horizontal);
        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(20, 420, 400, 33));
        label_22->setFont(font);
        label_22->setAlignment(Qt::AlignCenter);
        image_camera_livestream = new QLabel(groupBox_2);
        image_camera_livestream->setObjectName(QStringLiteral("image_camera_livestream"));
        image_camera_livestream->setGeometry(QRect(480, 50, 651, 421));
        image_camera_livestream->setAutoFillBackground(false);
        CaptureImagesDuringRotation = new QCheckBox(groupBox_2);
        CaptureImagesDuringRotation->setObjectName(QStringLiteral("CaptureImagesDuringRotation"));
        CaptureImagesDuringRotation->setGeometry(QRect(50, 310, 361, 38));
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(20, 203, 400, 33));
        label_13->setFont(font);
        video_cycle_spinBox = new QSpinBox(groupBox_2);
        video_cycle_spinBox->setObjectName(QStringLiteral("video_cycle_spinBox"));
        video_cycle_spinBox->setGeometry(QRect(20, 240, 400, 43));
        video_cycle_spinBox->setFont(font);
        video_cycle_spinBox->setAlignment(Qt::AlignCenter);
        video_cycle_spinBox->setMinimum(1);
        video_cycle_spinBox->setMaximum(999);
        video_cycle_spinBox->setValue(10);
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 610, 501, 381));
        start_run_button = new QPushButton(groupBox_3);
        start_run_button->setObjectName(QStringLiteral("start_run_button"));
        start_run_button->setEnabled(false);
        start_run_button->setGeometry(QRect(20, 150, 461, 91));
        QFont font2;
        font2.setPointSize(14);
        start_run_button->setFont(font2);
        splitter_6 = new QSplitter(groupBox_3);
        splitter_6->setObjectName(QStringLiteral("splitter_6"));
        splitter_6->setGeometry(QRect(20, 40, 461, 91));
        splitter_6->setOrientation(Qt::Vertical);
        label_8 = new QLabel(splitter_6);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font);
        splitter_6->addWidget(label_8);
        revolutions_counter = new QProgressBar(splitter_6);
        revolutions_counter->setObjectName(QStringLiteral("revolutions_counter"));
        revolutions_counter->setValue(24);
        splitter_6->addWidget(revolutions_counter);
        doCCWindex = new QPushButton(groupBox_3);
        doCCWindex->setObjectName(QStringLiteral("doCCWindex"));
        doCCWindex->setGeometry(QRect(50, 250, 131, 61));
        doCWindex = new QPushButton(groupBox_3);
        doCWindex->setObjectName(QStringLiteral("doCWindex"));
        doCWindex->setGeometry(QRect(300, 250, 161, 61));
        encoderAValue = new QLabel(groupBox_3);
        encoderAValue->setObjectName(QStringLiteral("encoderAValue"));
        encoderAValue->setGeometry(QRect(30, 330, 171, 32));
        encoderBValue = new QLabel(groupBox_3);
        encoderBValue->setObjectName(QStringLiteral("encoderBValue"));
        encoderBValue->setGeometry(QRect(290, 330, 181, 32));
        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(540, 500, 1281, 491));
        TorqueChart = new QChartView(groupBox_4);
        TorqueChart->setObjectName(QStringLiteral("TorqueChart"));
        TorqueChart->setGeometry(QRect(10, 40, 1261, 351));
        TorqueChart->setAutoFillBackground(false);
        layoutWidget = new QWidget(groupBox_4);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(50, 410, 451, 67));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setFont(font1);

        verticalLayout->addWidget(label_10);

        temp_sensor_1_lcdNumber = new QLCDNumber(layoutWidget);
        temp_sensor_1_lcdNumber->setObjectName(QStringLiteral("temp_sensor_1_lcdNumber"));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setWeight(75);
        temp_sensor_1_lcdNumber->setFont(font3);
        temp_sensor_1_lcdNumber->setCursor(QCursor(Qt::ArrowCursor));

        verticalLayout->addWidget(temp_sensor_1_lcdNumber);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(5);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setFont(font1);

        verticalLayout_2->addWidget(label_11);

        temp_sensor_2_lcdNumber = new QLCDNumber(layoutWidget);
        temp_sensor_2_lcdNumber->setObjectName(QStringLiteral("temp_sensor_2_lcdNumber"));
        temp_sensor_2_lcdNumber->setFont(font3);
        temp_sensor_2_lcdNumber->setCursor(QCursor(Qt::ArrowCursor));

        verticalLayout_2->addWidget(temp_sensor_2_lcdNumber);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(5);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_12 = new QLabel(layoutWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setFont(font1);

        verticalLayout_3->addWidget(label_12);

        temp_sensor_3_lcdNumber = new QLCDNumber(layoutWidget);
        temp_sensor_3_lcdNumber->setObjectName(QStringLiteral("temp_sensor_3_lcdNumber"));
        temp_sensor_3_lcdNumber->setFont(font3);
        temp_sensor_3_lcdNumber->setCursor(QCursor(Qt::ArrowCursor));

        verticalLayout_3->addWidget(temp_sensor_3_lcdNumber);


        horizontalLayout_2->addLayout(verticalLayout_3);

        splitter_10 = new QSplitter(groupBox_4);
        splitter_10->setObjectName(QStringLiteral("splitter_10"));
        splitter_10->setGeometry(QRect(640, 400, 400, 80));
        splitter_10->setOrientation(Qt::Vertical);
        label_23 = new QLabel(splitter_10);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setFont(font);
        splitter_10->addWidget(label_23);
        sampling_time = new QSpinBox(splitter_10);
        sampling_time->setObjectName(QStringLiteral("sampling_time"));
        sampling_time->setFont(font);
        sampling_time->setAlignment(Qt::AlignCenter);
        sampling_time->setMaximum(999999);
        sampling_time->setValue(100);
        splitter_10->addWidget(sampling_time);
        microseconds_current = new QLabel(groupBox_4);
        microseconds_current->setObjectName(QStringLiteral("microseconds_current"));
        microseconds_current->setGeometry(QRect(1060, 420, 211, 32));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1828, 37));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        MotorSettings->setTitle(QApplication::translate("MainWindow", "Bearing test settings", Q_NULLPTR));
        label_19->setText(QApplication::translate("MainWindow", "Revolutions/Direction", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Total Revolutions", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Pause time (sec)", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Motor Speed (rev/min)", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Motor Acceleration (rev/min^2)", Q_NULLPTR));
        motor_anti_CW_radioButton->setText(QApplication::translate("MainWindow", "Turn &motor anti-CW only", Q_NULLPTR));
        motor_CW_radioButton->setText(QApplication::translate("MainWindow", "Turn motor CW only", Q_NULLPTR));
        motor_alternate_direction_radioButton->setText(QApplication::translate("MainWindow", "Alternate CW and anti-CW", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Video Capture settings", Q_NULLPTR));
        label_21->setText(QApplication::translate("MainWindow", "Camera Exp. time (ms)", Q_NULLPTR));
        label_20->setText(QApplication::translate("MainWindow", "LED 1 Intensity", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "Video Duration (sec)", Q_NULLPTR));
        label_22->setText(QApplication::translate("MainWindow", "LED 2 Intensity", Q_NULLPTR));
        image_camera_livestream->setText(QApplication::translate("MainWindow", "CAMERA image", Q_NULLPTR));
        CaptureImagesDuringRotation->setText(QApplication::translate("MainWindow", "Capture images during rotation", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "Create Video every N cycles", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Bearing test control and progress", Q_NULLPTR));
        start_run_button->setText(QApplication::translate("MainWindow", "Start Run", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Progress", Q_NULLPTR));
        doCCWindex->setText(QApplication::translate("MainWindow", "CCW index", Q_NULLPTR));
        doCWindex->setText(QApplication::translate("MainWindow", "CW index", Q_NULLPTR));
        encoderAValue->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        encoderBValue->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Monitoring", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Temperature 1 (K)", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "Temperature 2 (K)", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "Temperature 3 (K)", Q_NULLPTR));
        label_23->setText(QApplication::translate("MainWindow", "Sampling time (ms)", Q_NULLPTR));
        microseconds_current->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
