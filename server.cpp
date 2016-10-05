#include "server.h"

const int MAX_COM_ID = 42;

Server::Server(QObject *parent) : QObject(parent)
{
    sendTimer = new QTimer(this);
    connect(sendTimer, SIGNAL(timeout()), this, SLOT( sendMessage() ));
    newPort = 0;
    nextMessageType = 0xFF;
    currentMessageType = 0xFF;
}

bool Server::COMconnect(int com_num)
{
    int openFlag;

    QString str = "COM";
    str.append(QString::number(com_num));

    std::cout << "Trying to open port |" << str.toStdString() << "|" << std::endl;

    newPort = new QSerialPort(str);
    newPort->setBaudRate(QSerialPort::Baud57600, QSerialPort::AllDirections);
    newPort->setDataBits(QSerialPort::Data8);
    newPort->setParity(QSerialPort::NoParity);
    newPort->setStopBits(QSerialPort::OneStop);
    newPort->setFlowControl(QSerialPort::NoFlowControl);

    std::cout << "Initializing port COM" << com_num << std::endl;
    try {
        openFlag = newPort->open(QIODevice::ReadWrite);
    } catch(...) {
        std::cout << "Serial port openning error" << std::endl;
        emit info("Serial port openning error");
        return false;
    }

    if (openFlag) {
        std::cout << "Serial port was successfully opened!" << std::endl;
        sendTimer->start(REQUEST_DELAY);
    } else {
        std::cout << "Cannot open serial port" << std::endl;
        emit info("Cannot open serial port");
        return false;
    }
    return true;
}


void Server::sendMessage() {
    if (currentMessageType != nextMessageType)
        currentMessageType = nextMessageType;
    switch (currentMessageType) {
    case REQUEST_NORMAL_CODE:
        sendMessageNormal();
        break;
    case REQUEST_CONFIG_CODE:
        sendMessageConfig();
        break;
    case REQUEST_DIRECT_CODE:
        sendMessageDirect();
        break;
    default:
        std::cout << "Unknown current message code!" << std::endl;
    }
}

void Server::sendMessageNormal()
{
    for (int i = 0; i < REQUEST_NORMAL_LENGTH; ++i) {
        msg_to_send[i] = 0x00;
    }

    j->update();

    msg_to_send[REQUEST_NORMAL_TYPE] = REQUEST_NORMAL_CODE;

    msg_to_send[REQUEST_NORMAL_MARCH+1] = j->march;
    msg_to_send[REQUEST_NORMAL_MARCH] = j->march >> 8;

    msg_to_send[REQUEST_NORMAL_LAG+1] = j->lag;
    msg_to_send[REQUEST_NORMAL_LAG] = j->lag >> 8;

    msg_to_send[REQUEST_NORMAL_DEPTH+1] = j->depth;
    msg_to_send[REQUEST_NORMAL_DEPTH] = j->depth >> 8;

    msg_to_send[REQUEST_NORMAL_ROLL+1] = j->roll;
    msg_to_send[REQUEST_NORMAL_ROLL] = j->roll >> 8;

    msg_to_send[REQUEST_NORMAL_PITCH+1] = j->pitch;
    msg_to_send[REQUEST_NORMAL_PITCH] = j->pitch >> 8;

    msg_to_send[REQUEST_NORMAL_YAW+1] = j->yaw;
    msg_to_send[REQUEST_NORMAL_YAW] = j->yaw >> 8;


    msg_to_send[REQUEST_NORMAL_LIGHT]      = j->light;
    msg_to_send[REQUEST_NORMAL_GRAB]       = j->grab;
    msg_to_send[REQUEST_NORMAL_TILT]       = j->tilt;
    msg_to_send[REQUEST_NORMAL_GRAB2_ROTATE] = j->grab2_rotate;
    msg_to_send[REQUEST_NORMAL_GRAB2_SQUEEZE] = j->grab2_squeeze;

    msg_to_send[REQUEST_NORMAL_STABILIZE_DEPTH] = false;
    msg_to_send[REQUEST_NORMAL_STABILIZE_ROLL] = false;
    msg_to_send[REQUEST_NORMAL_STABILIZE_PITCH] = false;
    msg_to_send[REQUEST_NORMAL_STABILIZE_YAW] = false;
    msg_to_send[REQUEST_NORMAL_RESET_IMU] = false;

    addCheckSumm16b(msg_to_send, REQUEST_NORMAL_LENGTH);


    std::cout << "Sending NORMAL message:" << std::endl;
    for (int i = 0; i < REQUEST_NORMAL_LENGTH; ++i) {
        std::cout << "|N" << i << "=" << unsigned(msg_to_send[i]) << std::endl;
    }

    newPort->write((char*)msg_to_send, REQUEST_NORMAL_LENGTH);

    emit imSleeping();
    QTest::qSleep (REQUEST_TIMEOUT);
    receiveMessage();
}

void Server::sendMessageDirect() {
    for (int i = 0; i < REQUEST_DIRECT_LENGTH; ++i) {
        msg_to_send[i] = 0x00;
    }

    msg_to_send[REQUEST_DIRECT_TYPE] = REQUEST_DIRECT_CODE;
    //addSNP(msg_to_send);

    msg_to_send[REQUEST_DIRECT_1] = settings->motors[0].speed;
    msg_to_send[REQUEST_DIRECT_2] = settings->motors[1].speed;
    msg_to_send[REQUEST_DIRECT_3] = settings->motors[2].speed;
    msg_to_send[REQUEST_DIRECT_4] = settings->motors[3].speed;
    msg_to_send[REQUEST_DIRECT_5] = settings->motors[4].speed;
    msg_to_send[REQUEST_DIRECT_6] = settings->motors[5].speed;
    msg_to_send[REQUEST_DIRECT_7] = settings->motors[6].speed;
    msg_to_send[REQUEST_DIRECT_8] = settings->motors[7].speed;

    addCheckSumm16b(msg_to_send, REQUEST_DIRECT_LENGTH);

    std::cout << "Sending DIRECT message:" << std::endl;
    for (int i = 0; i < REQUEST_DIRECT_LENGTH; ++i) {
        std::cout << "|N" << i << "=" << unsigned(msg_to_send[i]) << std::endl;
    }

    newPort->write((char*)msg_to_send, REQUEST_DIRECT_LENGTH);

    emit imSleeping();
    QTest::qSleep (REQUEST_TIMEOUT);
    receiveMessage();
}

void Server::sendMessageConfig() {
    for (int i = 0; i < REQUEST_CONFIG_LENGTH; ++i) {
        msg_to_send[i] = 0x00;
    }

    //addSNP(msg_to_send);
    msg_to_send[REQUEST_CONFIG_TYPE] = REQUEST_CONFIG_CODE;

    msg_to_send[REQUEST_CONFIG_CONST_TIME_DEPTH]    = settings->depth.const_time;
    msg_to_send[REQUEST_CONFIG_K1_DEPTH]            = settings->depth.k1;
    msg_to_send[REQUEST_CONFIG_K2_DEPTH]            = settings->depth.k2;
    msg_to_send[REQUEST_CONFIG_START_DEPTH]         = settings->depth.start;
    msg_to_send[REQUEST_CONFIG_GAIN_DEPTH]          = settings->depth.gain;

    msg_to_send[REQUEST_CONFIG_CONST_TIME_ROLL]     = settings->roll.const_time;
    msg_to_send[REQUEST_CONFIG_K1_ROLL]             = settings->roll.k1;
    msg_to_send[REQUEST_CONFIG_K2_ROLL]             = settings->roll.k2;
    msg_to_send[REQUEST_CONFIG_START_ROLL]          = settings->roll.start;
    msg_to_send[REQUEST_CONFIG_GAIN_ROLL]           = settings->roll.gain;

    msg_to_send[REQUEST_CONFIG_CONST_TIME_PITCH]    = settings->pitch.const_time;
    msg_to_send[REQUEST_CONFIG_K1_PITCH]            = settings->pitch.k1;
    msg_to_send[REQUEST_CONFIG_K2_PITCH]            = settings->pitch.k2;
    msg_to_send[REQUEST_CONFIG_START_PITCH]         = settings->pitch.start;
    msg_to_send[REQUEST_CONFIG_GAIN_PITCH]          = settings->pitch.gain;

    msg_to_send[REQUEST_CONFIG_CONST_TIME_YAW]      = settings->yaw.const_time;
    msg_to_send[REQUEST_CONFIG_K1_YAW]              = settings->yaw.k1;
    msg_to_send[REQUEST_CONFIG_K2_YAW]              = settings->yaw.k2;
    msg_to_send[REQUEST_CONFIG_START_YAW]           = settings->yaw.start;
    msg_to_send[REQUEST_CONFIG_GAIN_YAW]            = settings->yaw.gain;

    settings->motors;

    for (int i = 0; i < 8; ++i) {
        switch (settings->motors[i].code) {
        case settings->motors[i].HLB:
            msg_to_send[REQUEST_CONFIG_POSITION_HLB] = i;
            msg_to_send[REQUEST_CONFIG_INVERSE_HLB]  = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_HLB, settings->motors[i].k_backward);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_HLB,  settings->motors[i].k_forward);
            } else {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_HLB, 0.0);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_HLB,  0.0);
            }
            break;
        case settings->motors[i].HLF:
            msg_to_send[REQUEST_CONFIG_POSITION_HLF] = i;
            msg_to_send[REQUEST_CONFIG_INVERSE_HLF]  = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_HLF, settings->motors[i].k_backward);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_HLF,  settings->motors[i].k_forward);
            } else {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_HLF, 0.0);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_HLF,  0.0);
            }
            break;
        case settings->motors[i].HRB:
            msg_to_send[REQUEST_CONFIG_POSITION_HRB] = i;
            msg_to_send[REQUEST_CONFIG_INVERSE_HRB]  = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_HRB, settings->motors[i].k_backward);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_HRB,  settings->motors[i].k_forward);
            } else {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_HRB, 0.0);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_HRB,  0.0);
            }
            break;
        case settings->motors[i].HRF:
            msg_to_send[REQUEST_CONFIG_POSITION_HRF] = i;
            msg_to_send[REQUEST_CONFIG_INVERSE_HRF]  = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_HRF, settings->motors[i].k_backward);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_HRF,  settings->motors[i].k_forward);
            } else {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_HRF, 0.0);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_HRF,  0.0);
            }
            break;
        case settings->motors[i].VB:
            msg_to_send[REQUEST_CONFIG_POSITION_VB] = i;
            msg_to_send[REQUEST_CONFIG_INVERSE_VB]  = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_VB, settings->motors[i].k_backward);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_VB,  settings->motors[i].k_forward);
            } else {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_VB, 0.0);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_VB,  0.0);
            }
            break;
        case settings->motors[i].VF:
            msg_to_send[REQUEST_CONFIG_POSITION_VF] = i;
            msg_to_send[REQUEST_CONFIG_INVERSE_VF]  = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_VF, settings->motors[i].k_backward);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_VF,  settings->motors[i].k_forward);
            } else {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_VF, 0.0);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_VF,  0.0);
            }
            break;
        case settings->motors[i].VL:
            msg_to_send[REQUEST_CONFIG_POSITION_VL] = i;
            msg_to_send[REQUEST_CONFIG_INVERSE_VL]  = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_VL, settings->motors[i].k_backward);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_VL,  settings->motors[i].k_forward);
            } else {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_VL, 0.0);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_VL,  0.0);
            }
            break;
        case settings->motors[i].VR:
            msg_to_send[REQUEST_CONFIG_POSITION_VR] = i;
            msg_to_send[REQUEST_CONFIG_INVERSE_VR]  = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_VR, settings->motors[i].k_backward);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_VR,  settings->motors[i].k_forward);
            } else {
                addFloat(msg_to_send, REQUEST_CONFIG_K_BACKWARD_VR, 0.0);
                addFloat(msg_to_send, REQUEST_CONFIG_K_FORWARD_VR,  0.0);
            }
            break;
        default:
            std::cout << "ERROR: unknown motor code!" << std::endl;
        }

    }

    addCheckSumm16b(msg_to_send, REQUEST_CONFIG_LENGTH);

    std::cout << "Sending CONFIG message:" << std::endl;
    for (int i = 0; i < REQUEST_CONFIG_LENGTH; ++i) {
        std::cout << "|N" << i << "=" << unsigned(msg_to_send[i]) << std::endl;
    }

    newPort->write((char*)msg_to_send, REQUEST_CONFIG_LENGTH);

    emit imSleeping();
    QTest::qSleep (REQUEST_TIMEOUT);
    receiveMessage();
}





void Server::receiveMessage() {
    int buffer_size = newPort->bytesAvailable();
    std::cout << "In input buffer there are " << buffer_size << " bytes availible" << std::endl;
    if (buffer_size == 0)
        std::cout << "No message to read. Buffer is empty" << std::endl;
    else if (buffer_size != RESPONSE_LENGTH) {
        std::cout << "Wrong response message size! Got " << buffer_size << ", when " << RESPONSE_LENGTH << " expected." << std::endl;
        newPort->readAll();
    } else {
        msg_in = newPort->readAll();

        std::cout << "Got response. First symbol: " << msg_in[0] << std::endl;
        std::cout << "Checksum...";

        if (isCheckSumm16bCorrect((uint8_t*) msg_in.data(), RESPONSE_LENGTH)) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "INCORRECT!" << std::endl;
            return;
        }

        int16_t roll = msg_in[RESPONSE_ROLL];
        int16_t pitch = msg_in[RESPONSE_PITCH];
        int16_t yaw = msg_in[RESPONSE_YAW];

        int16_t roll_speed = msg_in[RESPONSE_ROLL_SPEED];
        int16_t pitch_speed = msg_in[RESPONSE_PITCH_SPEED];
        int16_t yaw_speed = msg_in[RESPONSE_YAW_SPEED];

        uint8_t temperature_MS = msg_in[RESPONSE_TEMPERATURE];
        uint8_t temperature_LS = msg_in[RESPONSE_TEMPERATURE+1];

        temperature = encodeTemperature(temperature_MS, temperature_LS);

        int16_t pressure = msg_in[RESPONSE_PRESSURE];

        uint16_t motor_errors = msg_in[RESPONSE_MOTOR_ERRORS];

        std::cout << "Received  data:" << std::endl;
        std::cout << "roll" << roll << "pitch" << pitch << "yaw" << yaw << std::endl;
        std::cout << "roll_speed" << roll_speed << "pitch_speed" << pitch_speed << "yaw_speed" << yaw_speed << std::endl;
        std::cout << "temperature=" << temperature << " pressure=" << pressure << std::endl;
        std::cout << "motor_errors" << motor_errors << std::endl;
    }
}


/* CRC16-CCITT algorithm */
uint8_t Server::isCheckSumm16bCorrect(uint8_t * msg, uint16_t length)
{
    uint16_t crcGot, crc = 0;
    int i;

    crcGot = (uint16_t)( msg[length-1] + (msg[length-2] << 8) );

    for(i=0; i < length - 2; i++){
        crc = (uint8_t)(crc >> 8) | (crc << 8);
        crc ^= msg[i];
        crc ^= (uint8_t)(crc & 0xff) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xff) << 4) << 1;
    }

    if(crc == crcGot )
        return 1;
    else return 0;
}

void Server::addCheckSumm16b(uint8_t * msg, uint16_t length)
{
    uint16_t crc = 0;
    int i = 0;

    for(i=0; i < length - 2; i++){
        crc = (uint8_t)(crc >> 8) | (crc << 8);
        crc ^= msg[i];
        crc ^= (uint8_t)(crc & 0xff) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xff) << 4) << 1;
    }

    msg[length-2] = (uint8_t) (crc >> 8);
    msg[length-1] = (uint8_t) crc;
}

void Server::addFloat(uint8_t * msg, int position, float value) {
    memcpy(msg + position, (unsigned char*) (&value), 4);
}


void Server::connect_fake() {
    std::cout << "Warning! Overriding COM port!" << std::endl;
    newPort = new QSerialPort();

    sendTimer->start(300);
}

void Server::connect_com()
{
    for (int i = 0; i < MAX_COM_ID; ++i) {
        if (COMconnect(i)){
            break;
        }
    }
}

void Server::disconnect_com() {
    std::cout << "Disconnecting" << std::endl;
    sendTimer->stop();
    newPort->disconnect();
    newPort->close();
    newPort->deleteLater();
}

void Server::addSNP(uint8_t * msg) {
    msg[0] = 'S';
    msg[1] = 'N';
    msg[2] = 'P';
}

float Server::encodeTemperature(uint8_t MS, uint8_t LS) {
    uint8_t sign_bytes = MS >> 3;
    int sign = 0;
    if (sign_bytes == 0) {
        sign = -1;
    } else {
        sign = 1;
    }
    int integer_part = ((MS << 5) >> 1) + (LS >> 4);
    float temperature = 0;
    uint8_t bit_power_minus_1 = LS & 0b00001000;
    uint8_t bit_power_minus_2 = LS & 0b00000100;
    uint8_t bit_power_minus_3 = LS & 0b00000010;
    uint8_t bit_power_minus_4 = LS & 0b00000001;
    temperature += integer_part;
    if (bit_power_minus_1) {
        temperature += 0.5;
        std::cout << "1/2 DETECTED!!!11" << std::endl;
    }
    if (bit_power_minus_2) temperature += 0.25;
    if (bit_power_minus_3) temperature += 0.125;
    if (bit_power_minus_4) temperature += 0.0625;
    return temperature;
}

Server::~Server() {
    std::cout << "Server shutting down..." << std::endl;
    if (newPort && newPort->isOpen()) {
        std::cout << "Stopping motors.." << std::endl;
        uint8_t msg_to_send[REQUEST_NORMAL_LENGTH];
        for (int i = 0; i < REQUEST_NORMAL_LENGTH; ++i) {
            msg_to_send[i] = 0x00;
        }
        addCheckSumm16b(msg_to_send, REQUEST_NORMAL_LENGTH);
        newPort->write((char*)msg_to_send, REQUEST_NORMAL_LENGTH);
        std::cout << "Success" << std::endl;
        std::cout << "Closing port" << std::endl;
        newPort->close();
        newPort->deleteLater();
    }
    std::cout << "Goodbye!" << std::endl;
}

