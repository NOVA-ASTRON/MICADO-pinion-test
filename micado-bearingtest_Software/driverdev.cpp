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

static const deviceinfo rs485id          = { .ID = "FT2JDUB8A", .baud = 19200 };
static const deviceinfo autocollimatorid = { .ID = "FT1IEXMOA", .baud = 9600 };
static const deviceinfo renishawids[]    = {{ .ID = "48F524753138", .baud = 115200 },
                                            { .ID = "48EB43743138", .baud = 115200 }};
static const deviceinfo motronaid        = { .ID = "FT1IF7NZA", .baud = 38400 };
static const deviceinfo phytronids[]     = { { .ID = "6&347f23d1&0&1", .baud = 115200 },
                                             { .ID = "6&32A5DFEC&1&3", .baud = 115200 },
                                            { .ID = "6&2EF6BFFB&0&1", .baud = 115200 },
                                            { .ID = "5&3B489C26&0&1", .baud = 115200 },
                                            { .ID = "6&347F23D1&0&1", .baud = 115200 },
                                            { .ID = "3B3F6816", .baud = 115200 },
                                            { .ID = "12c8f4c0", .baud = 115200 },
                                          };


std::map<std::string, const optional_devs> deviceInfoLookup = {
    {"rs485",{&rs485id,1}},
    {"autocollimator",{&autocollimatorid,1}},
    {"renishaw",{&renishawids[0],2}},
    {"motrona",{&motronaid,1}},
    {"phytron",{&phytronids[0],7}}
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
        log->Write("Searching device with ID " + dev.ID);
        QString IDtoFind(dev.ID.c_str());
        const auto infos = QSerialPortInfo::availablePorts();
        for (auto &info:infos){
            std::stringstream ss;
            ss << "serial : "<< info.serialNumber().toStdString() <<"\n";
            ss << "desc : " << info.description().toStdString() <<"\n";
            log->Write(ss.str());
            if (info.serialNumber()==IDtoFind){
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

bool DriverDev::SendCommand(Command &command, std::string err)
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
            }
            catch (const std::exception& e)
            {
                log->Write(err);
            }
        }
    }
    if (retval == false)
        log->Write("SerialDev(" + name + ") Not present:" + err);
    return retval;
}

deviceState DriverDev::GetDeviceState()
{
    return {
        .name = name,
        .connected = ((serial != nullptr) ? serial->isOpen() : false)
    };
}

bool DriverDev::getValid() const
{
    return valid;
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
