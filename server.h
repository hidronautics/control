#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QSerialPort>
#include <QTest>
#include <qtimer.h>
#include <stdint.h>
#include <messages.h>
#include <iostream>
#include <math.h>
#include <string>
#include <cstdlib>

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
    explicit Server(QObject *parent = 0);
    ~Server();
    Joystick* j;

    uint8_t msg_to_send[MESSAGES_ARRAY_SIZE];
    QByteArray msg_in;

    uint8_t nextMessageType;
    uint8_t currentMessageType;

    QTimer *sendTimer;

    Settings* settings;

    float temperature = 0;
    int16_t imu_roll;
    int16_t imu_pitch;
    int16_t imu_yaw;

    int16_t imu_roll_speed;
    int16_t imu_pitch_speed;
    int16_t imu_yaw_speed;

    bool emulation_mode = false;

private:
    QSerialPort *newPort;

    bool COMconnect(int com_name);

    void addCheckSumm16b(uint8_t * msg, uint16_t length);
    uint8_t isCheckSumm16bCorrect(uint8_t * msg, uint16_t length);

    void sendMessageNormal();
    void sendMessageDirect();
    void sendMessageConfig();
    void receiveMessage();

    void addFloat(uint8_t * msg, int position, float val);
    void addSNP(uint8_t * msg);

    float encodeTemperature(uint8_t MS, uint8_t LS);



signals:
    void imSleeping();
    void info(QString s);

public slots:
    void connect_fake();

    void connect_com();
    void disconnect_com();

private slots:
    void sendMessage();
};

#endif // SERVER_H
