#include "driverdev.h"
#include <iostream>
#include <mutex>
#include <sstream>
#include <string_view>
//#include <syncstream>
#include <thread>
#include <QCoreApplication>
#include <QSerialPortInfo>
#include <chrono>

#define lock(mut) const std::lock_guard<std::mutex> lock(mut);

static const deviceinfo rs485_id          = { .ID = "FT2JDUB8A", .baud = 19200 };
static const deviceinfo autocollimator_id = { .ID = "FT1IEXMOA", .baud = 9600 };
static const deviceinfo renishaw_ids[]    = {{ .ID = "48F524753138", .baud = 115200 },
                                            { .ID = "48EB43743138", .baud = 115200 }};
static const deviceinfo motrona_id        = { .ID = "FT1IF7NZA", .baud = 38400 };
static const deviceinfo phytron_ids[]     = {{ .ID = "0403:6001", .baud = 115200 }};


std::map<std::string, const optional_devs> deviceInfoLookup = {
    {"rs485",{&rs485_id,1}},
    {"autocollimator",{&autocollimator_id,1}},
    {"renishaw",{&renishaw_ids[0],2}},
    {"motrona",{&motrona_id,1}},
    {"phytron",{&phytron_ids[0],1}}
};

const optional_devs* DeviceInfoFromName(const std::string &name){
    if (deviceInfoLookup.count(name))
        return &deviceInfoLookup[name];
    return nullptr;
}

DriverDev::DriverDev(QObject *parent):QObject(parent)
{
    //std::lock_guard<std::mutex> lock(_lock);
}


QSerialPort *DriverDev::FindSerialPort(const optional_devs *devs)
{
    for (int i=0;i<devs->num_infos;i+=1){
        deviceinfo dev = devs->infos[i];
        log->Write("Searching device with VID:PID " + dev.ID);
        long vid = strtol(dev.ID.substr(0, 4).c_str(), NULL, 16);
        long pid = strtol(dev.ID.substr(5, 4).c_str(), NULL, 16);
        const auto infos = QSerialPortInfo::availablePorts();
        for (auto &info:infos){
            std::stringstream ss;
            ss << "serial : "<< info.serialNumber().toStdString() <<"\n";
            ss << "desc : " << info.description().toStdString() << "\n";
            ss << "pid (decimal): " << info.productIdentifier() << "\n";
            ss << "vid (decimal): " << info.vendorIdentifier() << "\n";
            log->Write(ss.str());
            if (info.vendorIdentifier()==vid && info.productIdentifier()==pid){
                QSerialPort *ret = new QSerialPort(info);
                // if correctly created apply settings
                if (ret) {
                    ret->setBaudRate(dev.baud);
                }
                return ret;
            }
        }
    }

    log->Write("Failed to find COM port.");
    return null;
}

bool DriverDev::Connect()
{
    lock (_lock)
    {
        serial = FindSerialPort(devs);

        if (serial == nullptr)
        {
            log->Write("SerialDev: failed to attched serial for " + name);
            return false;
        }
        log->Write("SerialDev: attched serial for " + name);

        if (serial->open(QIODevice::ReadWrite) == false)
        {
            log->Write("SerialDev(" + name + "): failed to open serial port");
            std::cerr << "SerialDev(" << name << "): failed to open serial port\n";
            return false;
        } else {
            serial->flush();
        }

        log->Write("SerialDev: outgoing bytes buffer : 0");
        log->Write("SerialDev: incoming bytes buffer : " + QString::number(serial->readBufferSize()).toStdString());
        //log->Write("SerialDev: incoming bytes trigger " + serial.ReceivedBytesThreshold.Tostd::string());
        //log->Write("SerialDev: read time out " + serial.ReadTimeout.Tostd::string());

    }
    return true;
}

void DriverDev::Disconnect()
{
    // grace time for handling incoming data.
    msdelay(500);
    lock (_lock)
    {
        if (serial != nullptr) serial->close();
        serial = nullptr;
    }

}

void DriverDev::InitDriverDev(const optional_devs *ID, LogFile l, std::string n)
{
    log = l;
    log->Write("Add device " + n);
    devs = ID;
    name = n;
    valid=Connect();
}

void DriverDev::addHandler()
{
    lock (_lock)
    {
        if (serial)
            connect(serial, &QSerialPort::readyRead,this,&DriverDev::processincomingbytes);
    }
}

void DriverDev::processincomingbytes()
{
    lock (_lock)
    {
        if (serial) {
            IncomingData.append(serial->readAll());
            incomingbytes(IncomingData);
        }
    }
}

bool DriverDev::_SendCommand(Command &command, std::string err)
{
    bool retval = false;
    lock (_lock)
    {
        if (serial)
        {
            try
            {
                serial->write(command.cmd,command.length);
                retval = true;
                num_sent+=1;

            }
            catch (const std::exception& e)
            {
                log->Write(err);
                std::cerr << err <<"\n";
            }
        } else std::cerr << "SerialDev(" << name << ") Not present:" << err << "\n";

    }
    if (retval == false)
        log->Write("SerialDev(" + name + ") Not present:" + err);
    return retval;
}

deviceState DriverDev::GetDeviceState()
{
    return {
        .name = name,
        .connected = ((serial != nullptr) ? serial->isOpen() : false),
        .queue_empty = serial->bytesToWrite()==0
    };
}

bool DriverDev::getValid() const
{
    return valid;
}

int DriverDev::getNum_sent() const
{
    return num_sent;
}

void msdelay(int ms)
{
    auto begin = std::chrono::steady_clock::now();
    int passed=0;
    while (ms>passed) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, ms-passed);
        auto end = std::chrono::steady_clock::now();
        passed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }
}
