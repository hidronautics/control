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
#include <QDataStream>

#define STABILIZATION_AMOUNT 4

enum STAB_CIRCUITS {
    STAB_DEPTH = 0,
    STAB_YAW,
    STAB_ROLL,
    STAB_PITCH
};

#define STABILIZATION_FILTERS 2

enum STAB_FILTERS {
    POS_FILTER = 0,
    SPEED_FILTER
};

#define THRUSTERS_AMOUNT 8
enum Thrusters {
    HLB = 0,
    HLF,
    HRB,
    HRF,
    VB,
    VF,
    VL,
    VR
};

struct ThrusterConfig {
    uint8_t id = 0;
    int8_t velocity = 0;
    bool reverse = 0;

    float kForward = 0;
    float kBackward = 0;

    int8_t forward_saturation = 0;
    int8_t backward_saturation = 0;
};


struct Stabilization {
    bool enable;
    struct RobotStabilizationConstants {
        // Before P
        float pJoyUnitCast;
        float pSpeedDyn;
        float pErrGain;
        // Feedback aperiodic filters
        struct AperiodicFilter {
            float T;
            float K;
        } aFilter[STABILIZATION_FILTERS];
        // PID
        struct PidConstants {
            float pGain;
            float iGain;
            float iMax;
            float iMin;
        } pid;
        // Thrusters unit cast
        float pThrustersCast;
        float pThrustersMin;
        float pThrustersMax;

        friend QDataStream& operator<<(QDataStream &ds, const RobotStabilizationConstants &req)
        {
            ds.setByteOrder(QDataStream::LittleEndian);
            ds << req.pJoyUnitCast;
            ds << req.pSpeedDyn;
            ds << req.pErrGain;
            ds << req.aFilter[POS_FILTER].T;
            ds << req.aFilter[POS_FILTER].K;
            ds << req.aFilter[SPEED_FILTER].T;
            ds << req.aFilter[SPEED_FILTER].K;
            ds << req.pid.pGain;
            ds << req.pid.iGain;
            ds << req.pid.iMax;
            ds << req.pid.iMin;
            ds << req.pThrustersCast;
            ds << req.pThrustersMin;
            ds << req.pThrustersMax;
            return ds;
        }
    } stabConstants;

    struct RobotStabilizationState {
        float inputSignal;
        float speedSignal;
        float posSignal;

        float oldSpeed;
        float oldPos;

        float joyUnitCasted;
        float joy_iValue;
        float posError;
        float speedError;
        float dynSummator;
        float pidValue;
        float posErrorAmp;
        float speedFiltered;
        float posFiltered;

        float pid_iValue;

        friend QDataStream& operator>>(QDataStream &ds, RobotStabilizationState &req)
        {
            ds.setByteOrder(QDataStream::LittleEndian);
            ds >> req.inputSignal;
            ds >> req.speedSignal;
            ds >> req.posSignal;
            ds >> req.oldSpeed;
            ds >> req.oldPos;
            ds >> req.posError;
            ds >> req.speedError;
            ds >> req.dynSummator;
            ds >> req.posErrorAmp;
            ds >> req.speedFiltered;
            ds >> req.posFiltered;
            ds >> req.pid_iValue;
            return ds;
        }
    } stabState;
};

class Connection;

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    Connection* connection;
    Stabilization stabContour[STABILIZATION_AMOUNT];

    uint8_t current_thrusters_numb;
    ThrusterConfig thrusters_configs[THRUSTERS_AMOUNT];

    float controlMultiplier = 1;
    uint8_t pcreset = 0;

    bool loadFromJSONFile();
    bool saveToJSONFIle() const;

    int CS;
    int oldCS;

private:
    void initialize();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

signals:

public slots:
    void jetson_on_off_btn_clicked(bool checked);
};



class Connection
{
public:
   // TODO what is this? that was caused warning
   //ConnectionSettings() {}

   int num;
   QSerialPort::BaudRate baudRate;
   QSerialPort::DataBits dataBits;
   QSerialPort::Parity parity;
   QSerialPort::StopBits stopBits;
   QSerialPort::FlowControl flowControl;
   int pause_after_sent;
   int pause_after_received;

   QSerialPort::BaudRate getBaudRate(int num) {
       switch (num) {
       case 1200:
           return QSerialPort::BaudRate::Baud1200;
       case 2400:
           return QSerialPort::BaudRate::Baud2400;
       case 4800:
           return QSerialPort::BaudRate::Baud4800;
       case 9600:
           return QSerialPort::BaudRate::Baud9600;
       case 19200:
           return QSerialPort::BaudRate::Baud19200;
       case 38400:
           return QSerialPort::BaudRate::Baud38400;
       case 57600:
           return QSerialPort::BaudRate::Baud57600;
       case 115200:
           return QSerialPort::BaudRate::Baud115200;
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
       case 6:
           return QSerialPort::DataBits::Data6;
       case 7:
           return QSerialPort::DataBits::Data7;
       case 8:
           return QSerialPort::DataBits::Data8;
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


#endif // SETTINGS_H
