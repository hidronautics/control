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

struct Stabilization {
    float k1;
    float k2;
    float start;
    float gain;
    bool const_time;
};

class Motor;

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

    Motor* motors;
    Stabilization depth, roll, pitch, yaw;

    bool loadFromJSONFile();
    bool saveToJSONFIle() const;

private:
    void initialize();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    /*
    void readMotors(const QJsonObject &json);
    void writeMotors(QJsonObject &json);

    void readStabilization(const QJsonObject &json);
    void writeStabilization(QJsonObject &json);
    */

signals:

public slots:
    void changeMotorSetting(int slot, QString motorID, bool inverse);
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
