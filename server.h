#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QSerialPort>
#include <QTest>
#include <QDebug>
#include <qtimer.h>
#include <stdint.h>
#include <messages.h>
#include <iostream>
#include <math.h>
#include <string>
#include <cstdlib>
#include "qcustomplot.h"

#include "joystick.h"
#include "settings.h"

#define REQUEST_DELAY					25
#define REQUEST_TIMEOUT					25
#define CHANGE_TYPE_TIMEOUT             200

#define MESSAGES_ARRAY_SIZE             REQUEST_CONFIG_LENGTH



class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    Joystick* j;

    QByteArray msg_to_send;
    QByteArray msg_in;

    uint8_t nextMessageType;
    uint8_t currentMessageType;

    QTimer *sendTimer;

    QFile file_csv_response;
    QFile file_csv_request;

    QTextStream stream_response;
    QTextStream stream_request;

    Settings* settings;

    //float temperature = 0;
    float imu_roll;
    float imu_pitch;
    float imu_yaw;

    float imu_roll_speed;
    float imu_pitch_speed;
    float imu_yaw_speed;

    // For plots
    float imu_roll_d;
    float imu_pitch_d;
    float imu_yaw_d;

    float imu_roll_speed_d;
    float imu_pitch_speed_d;
    float imu_yaw_speed_d;

    double key1;

    // _______________________________

    //int16_t imu_depth;
    float imu_pressure;

    uint8_t acoustic_state;
    int16_t leak_sensor;
    float in_pressure;

    uint16_t current_HLB;
    uint16_t current_HLF;
    uint16_t current_HRB;
    uint16_t current_HRF;
    uint16_t current_VB;
    uint16_t current_VF;
    uint16_t current_VL;
    uint16_t current_VR;

    int8_t velocity_HLB;
    int8_t velocity_HLF;
    int8_t velocity_HRB;
    int8_t velocity_HRF;
    int8_t velocity_VB;
    int8_t velocity_VF;
    int8_t velocity_VL;
    int8_t velocity_VR;

    uint16_t current_light;
    uint16_t current_bottom_light;
    uint16_t current_agar;
    uint16_t current_grab;
    uint16_t current_grab_rotate;
    uint16_t current_tilt;

    uint16_t err_vma;
    uint16_t err_dev;
    uint8_t err_pc;

    uint16_t msg_received_counter;
    uint16_t msg_lost_counter;
    float msg_lost_percent;

    QString bt_data;
    QString path_csv_request;
    QString path_csv_response;

    bool emulation_mode = false;

private:
    QSerialPort *newPort;
    const QString log_folder_path = "C:\\logs\\";

    bool COMconnect(int com_name);

    void addCheckSumm16b(QByteArray * msg, uint16_t length);
    uint16_t getCheckSumm16b(QByteArray * msg, uint16_t length);
    uint8_t isCheckSumm16bCorrect(uint8_t * msg, uint16_t length);

    void sendMessageNormal();
    void sendMessageDirect();
    void sendMessageConfig();

    void receiveMessage();
    void receiveNormalMessage();
    void receiveConfigMessage();

    void addFloat(uint8_t * msg, int position, float val);
    float getFloat(QByteArray msg, int position);
    void addSNP(uint8_t * msg);

    //float encodeTemperature(uint8_t MS, uint8_t LS);

    void writeCSV(QTextStream* stream, uint8_t * msg, uint16_t length);


signals:
    void imSleeping();
    void info(QString s);
    void updateCsView();

public slots:
    void connect_fake();

    void connect_com();
    void disconnect_com();

private slots:
    void sendMessage();
};

#endif // SERVER_H
