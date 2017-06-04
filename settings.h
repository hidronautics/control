#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <stdint.h>

#include <QObject>
#include <QString>
#include <QFile>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QSerialPort>

struct Stabilization {
    float k1;
    float k2;
    float start;
    float gain;
    bool const_time;
};

class Motor;
class Connection;

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    Motor* motors;
    Connection* connection;
    Stabilization depth, roll, pitch, yaw;


    bool loadFromJSONFile();
    bool saveToJSONFIle() const;

private:
    void initialize();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

signals:

public slots:
    void changeMotorSetting(int slot, QString motorID, bool inverse);
};



class Connection
{
public:
   ConnectionSettings() {}

   int num;
   QSerialPort::BaudRate baudRate;
   QSerialPort::DataBits dataBits;
   QSerialPort::Parity parity;
   QSerialPort::StopBits stopBits;
   QSerialPort::FlowControl flowControl;

   QSerialPort::BaudRate getBaudRate(int num) {
       switch (num) {
       case 1200:
           return QSerialPort::BaudRate::Baud1200;
           break;
       case 2400:
           return QSerialPort::BaudRate::Baud2400;
           break;
       case 9600:
           return QSerialPort::BaudRate::Baud9600;
           break;
       case 19200:
           return QSerialPort::BaudRate::Baud19200;
           break;
       case 38400:
           return QSerialPort::BaudRate::Baud38400;
           break;
       case 57600:
           return QSerialPort::BaudRate::Baud57600;
           break;
       case 115200:
           return QSerialPort::BaudRate::Baud115200;
           break;
       default:
           return QSerialPort::BaudRate::UnknownBaud;
       }
   }

   int setBaudRate(QSerialPort::BaudRate baudRate) {
       return baudRate;
   }

   QSerialPort::DataBits getDataBits(int num) {
       switch (num) {
       case 5:
           return QSerialPort::DataBits::Data5;
           break;
       case 6:
           return QSerialPort::DataBits::Data6;
           break;
       case 7:
           return QSerialPort::DataBits::Data7;
           break;
       case 8:
           return QSerialPort::DataBits::Data8;
           break;
       default:
           return QSerialPort::DataBits::UnknownDataBits;
       }
   }

   int setDataBits(QSerialPort::DataBits dataBits) {
       return dataBits;
   }

   QSerialPort::Parity getParity(QString str) {
       QString lower = str.toLower();

       if (!lower.compare("no"))
           return QSerialPort::Parity::NoParity;
       else if (!lower.compare("even"))
           return QSerialPort::Parity::EvenParity;
       else if (!lower.compare("odd"))
           return QSerialPort::Parity::OddParity;
       else if (!lower.compare("space"))
           return QSerialPort::Parity::SpaceParity;
       else if (!lower.compare("mark"))
           return QSerialPort::Parity::MarkParity;
       else
           return QSerialPort::Parity::UnknownParity;
   }

   QString setParity(QSerialPort::Parity parity) {
       switch(parity) {
       case QSerialPort::Parity::NoParity:
           return "no";
       case QSerialPort::Parity::EvenParity:
           return "even";
       case QSerialPort::Parity::OddParity:
           return "odd";
       case QSerialPort::Parity::SpaceParity:
           return "space";
       case QSerialPort::Parity::MarkParity:
           return "mark";
       default:
           return "unknown";
       }
   }

   QSerialPort::StopBits getStopBits(QString str) {
       QString lower = str.toLower();

       if (!lower.compare("one"))
           return QSerialPort::StopBits::OneStop;
       else if (!lower.compare("oneandhalf"))
           return QSerialPort::StopBits::OneAndHalfStop;
       else if (!lower.compare("two"))
           return QSerialPort::StopBits::TwoStop;
       else
           return QSerialPort::StopBits::UnknownStopBits;
   }

   QString setStopBits(QSerialPort::StopBits stopBits) {
       switch(stopBits) {
       case QSerialPort::StopBits::OneStop:
           return "one";
       case QSerialPort::StopBits::OneAndHalfStop:
           return "oneandhalf";
       case QSerialPort::StopBits::TwoStop:
           return "two";
       default:
           return "unknown";
       }
   }

   QSerialPort::FlowControl getFlowControl(QString str) {
       QString lower = str.toLower();

       if (!lower.compare("no"))
           return QSerialPort::FlowControl::NoFlowControl;
       else if (!lower.compare("hardware"))
           return QSerialPort::FlowControl::HardwareControl;
       else if (!lower.compare("software"))
           return QSerialPort::FlowControl::SoftwareControl;
       else
           return QSerialPort::FlowControl::UnknownFlowControl;
   }

   QString setFlowControl(QSerialPort::FlowControl flowControl) {
       switch(flowControl) {
       case QSerialPort::FlowControl::NoFlowControl:
           return "no";
       case QSerialPort::FlowControl::HardwareControl:
           return "hardware";
       case QSerialPort::FlowControl::SoftwareControl:
           return "software";
       default:
           return "unknown";
       }
   }
};



class Motor
{
public:
    Motor() {}

    enum MotorCode {
        HLB, HLF, HRB, HRF, VB, VF, VL, VR
    };

    MotorCode code;
    uint8_t adress; // 0..7
    int16_t speed;  // -32 768..+32 767
    bool inverse;
    bool enabled;

    double k_forward;
    double k_backward;



    QString getCode() const{
        switch (code) {
        case HLB:
            return "HLB";
        case HLF:
            return "HLF";
        case HRB:
            return "HRB";
        case HRF:
            return "HRF";
        case VB:
            return "VB";
        case VF:
            return "VF";
        case VL:
            return "VL";
        case VR:
            return "VR";
        }
        return NULL;
    }

    void setCode(QString code) {
        if (!code.compare("HLB")) {
            this->code = HLB;
            return;
        }
        if (!code.compare("HLF")) {
            this->code = HLF;
            return;
        }
        if (!code.compare("HRB")) {
            this->code = HRB;
            return;
        }
        if (!code.compare("HRF")) {
            this->code = HRF;
            return;
        }
        if (!code.compare("VB")) {
            this->code = VB;
            return;
        }
        if (!code.compare("VF")) {
            this->code = VF;
            return;
        }
        if (!code.compare("VL")) {
            this->code = VL;
            return;
        }
        if (!code.compare("VR")) {
            this->code = VR;
            return;
        }
    }
};

#endif // SETTINGS_H
