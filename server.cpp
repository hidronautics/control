#include "server.h"

const int MAX_COM_ID = 20;

Server::Server(QObject *parent) : QObject(parent)
{
    sendTimer = new QTimer(this);
    connect(sendTimer, SIGNAL(timeout()), this, SLOT( sendMessage() ));
    newPort = nullptr;
    nextMessageType = 0xA5;
    currentMessageType = 0xA5;
    msg_lost_counter = 0;
    msg_received_counter = 0;
}

bool Server::COMconnect(int com_num)
{
    int isOpened = false;

    QString str;
    if (OS == "unix") {
        str = "/dev/ttyUSB";
    }
    else if (OS == "win32") {
        str = "COM";
    }

    str.append(QString::number(com_num));

    std::cout << "Trying to open port |" << str.toStdString() << "|...";

    newPort = new QSerialPort(str);
    newPort->setBaudRate(settings->connection->baudRate, QSerialPort::AllDirections);
    newPort->setDataBits(settings->connection->dataBits);
    newPort->setParity(settings->connection->parity);
    newPort->setStopBits(settings->connection->stopBits);
    newPort->setFlowControl(settings->connection->flowControl);

    try {
        isOpened = newPort->open(QIODevice::ReadWrite);
    } catch(...) {
        std::cout << " serial port openning error" << std::endl;
        emit info("Serial port openning error");
        delete newPort;
        return false;
    }

    if (isOpened) {
        std::cout << " successfully opened!" << std::endl;
        sendTimer->start(REQUEST_DELAY);
    } else {
        std::cout << ". Unable to open serial port" << std::endl;
        emit info("Unable to open serial port");
        delete newPort;
        return false;
    }

    return true;
}


void Server::sendMessage() {
    std::cout << "Server::sendMessage()" << std::endl;
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
        std::cout << "ERROR: Unknown current message code!" << std::endl;
    }

    emit imSleeping();

    std::cout << "Go sleeping for " << settings->connection->pause_after_sent << " ms" << std::endl;
    QTest::qSleep (settings->connection->pause_after_sent);
    receiveMessage();
}

void Server::sendMessageNormal()
{
    std::cout << "Server::sendMessageNormal()" << std::endl;
    j->update();

    msg_to_send.clear();
    QDataStream stream(&msg_to_send, QIODevice::Append);

    // Defining request structure
    Request_s req;

    // Filling request structure
    req.type = REQUEST_NORMAL_CODE;
    req.flags = 0x00;

    req.march = j->march;
    req.lag = j->lag;
    req.depth = j->depth;

    req.roll = j->roll;
    req.pitch = j->pitch;
    req.yaw = j->yaw;

    req.light = j->light;
    req.grab = j->grab;
    req.tilt = j->tilt;
    req.grab_rotate = static_cast<int8_t>(j->grab_rotate);
    req.dev1 = 0;
    req.dev2 = 0;

    req.dev_flags = 0;
    req.stabilize_flags = 0;
    req.cameras = j->camera;
    req.pc_reset = j->pc_reset;

    // Moving structure to QByteArray
    stream << req;

    // Calculating checksum
    uint16_t checksum = getCheckSumm16b(msg_to_send.data(), REQUEST_NORMAL_LENGTH);

    // Moving checksum to QByteArray
    stream << checksum;

    /*
    std::cout << "Sending NORMAL message:" << std::endl;
    for (int i = 0; i < REQUEST_NORMAL_LENGTH; ++i) {
        std::cout << "|N" << i << "=" << unsigned(msg_to_send[i]) << std::endl;
    }
    */

    newPort->write(msg_to_send, REQUEST_NORMAL_LENGTH);
}

void Server::sendMessageDirect() {
    std::cout << "Server::sendMessageDirect()" << std::endl;
    // TODO this is actually doesn't realised on STM side, will do
    /*
    for (int i = 0; i < REQUEST_DIRECT_LENGTH; ++i) {
        msg_to_send[i] = 0x00;
    }
    msg_to_send[0] = 0xFF;

    msg_to_send[REQUEST_DIRECT_TYPE] = REQUEST_DIRECT_CODE;

    msg_to_send[REQUEST_DIRECT_1] = settings->motors[0].speed;
    msg_to_send[REQUEST_DIRECT_2] = settings->motors[1].speed;
    msg_to_send[REQUEST_DIRECT_3] = settings->motors[2].speed;
    msg_to_send[REQUEST_DIRECT_4] = settings->motors[3].speed;
    msg_to_send[REQUEST_DIRECT_5] = settings->motors[4].speed;
    msg_to_send[REQUEST_DIRECT_6] = settings->motors[5].speed;
    msg_to_send[REQUEST_DIRECT_7] = settings->motors[6].speed;
    msg_to_send[REQUEST_DIRECT_8] = settings->motors[7].speed;

    addCheckSumm16b(msg_to_send, REQUEST_DIRECT_LENGTH);

    //std::cout << "Sending DIRECT message:" << std::endl;
    for (int i = 0; i < REQUEST_DIRECT_LENGTH; ++i) {
        std::cout << "|N" << i << "=" << unsigned(msg_to_send[i]) << std::endl;
    }

    newPort->write((char*)msg_to_send, REQUEST_DIRECT_LENGTH);

    emit imSleeping();
    QTest::qSleep (settings->connection->pause_after_sent);
    receiveMessage();
    */
}

void Server::sendMessageConfig() {
    std::cout << "Server::sendMessageConfig()" << std::endl;
    j->update();

    msg_to_send.clear();
    QDataStream stream(&msg_to_send, QIODevice::Append);

    ConfigRequest_s req;

    req.type = REQUEST_CONFIG_CODE;
    req.contour = static_cast<uint8_t>(settings->CS);

    req.march = j->march;
    req.lag = j->lag;
    req.depth = j->depth;
    req.roll = j->roll;
    req.pitch = j->pitch;
    req.yaw = j->yaw;

    req.pJoyUnitCast = settings->stabContour[settings->CS].stabConstants.pJoyUnitCast;
    req.pSpeedDyn = settings->stabContour[settings->CS].stabConstants.pSpeedDyn;
    req.pErrGain = settings->stabContour[settings->CS].stabConstants.pErrGain;

    req.posFilterT = settings->stabContour[settings->CS].stabConstants.aFilter[POS_FILTER].T;
    req.posFilterK = settings->stabContour[settings->CS].stabConstants.aFilter[POS_FILTER].K;
    req.speedFilterT = settings->stabContour[settings->CS].stabConstants.aFilter[SPEED_FILTER].T;
    req.speedFilterK = settings->stabContour[settings->CS].stabConstants.aFilter[SPEED_FILTER].K;

    req.pid_pGain = settings->stabContour[settings->CS].stabConstants.pid.pGain;
    req.pid_iGain = settings->stabContour[settings->CS].stabConstants.pid.iGain;
    req.pid_iMax = settings->stabContour[settings->CS].stabConstants.pid.iMax;
    req.pid_iMin = settings->stabContour[settings->CS].stabConstants.pid.iMin;

    req.pThrustersCast = settings->stabContour[settings->CS].stabConstants.pThrustersCast;
    req.pThrustersMin = settings->stabContour[settings->CS].stabConstants.pThrustersMin;
    req.pThrustersMax = settings->stabContour[settings->CS].stabConstants.pThrustersMax;

    stream << req;

    // Calculating checksum
    addCheckSumm16b(msg_to_send.data(), REQUEST_CONFIG_LENGTH);

    newPort->write(msg_to_send.data(), REQUEST_CONFIG_LENGTH);

    settings->oldCS = settings->CS;

    emit imSleeping();
    QTest::qSleep (settings->connection->pause_after_sent);
    receiveMessage();
}

void Server::receiveMessage() {
    std::cout << "Server::receiveMessage()" << std::endl;
    long long buffer_size = 0;
    newPort->waitForReadyRead(25);
    buffer_size = newPort->bytesAvailable();

    std::cout << "In input buffer there are " << buffer_size << " bytes availible" << std::endl;

    if (buffer_size == 0) {
        std::cout << "No message to read. Buffer is empty" << std::endl;
        msg_lost_counter++;
    } else {
        int counter = 0;
        while (true) {
            long long bytesAvailible = newPort->bytesAvailable();
            if (bytesAvailible > 0) {
                msg_in.clear();
                msg_in.push_back(newPort->read(1));
                switch(static_cast<uint8_t>(msg_in[0])) {
                    case REQUEST_NORMAL_CODE:
                        receiveNormalMessage();
                        break;
                    case REQUEST_CONFIG_CODE:
                        receiveConfigMessage();
                        break;
                    default:
                       newPort->clear();
                }
                break;
            }
            else if (counter > 250000) {
                newPort->clear();
            }
            else {
                counter++;
            }
        }
    }

    //QTest::qSleep (settings->connection->pause_after_received);
    msg_lost_percent = static_cast<float>(msg_lost_counter) / (static_cast<float>(msg_received_counter) + static_cast<float>(msg_lost_counter));
    msg_lost_percent *= 100;

    std::cout << "RECEIVED = " << msg_received_counter << std::endl;
    std::cout << "LOST = " << msg_lost_counter << std::endl;
    std::cout << "LOST PERCENT = " << msg_lost_percent << std::endl;
}

void Server::receiveNormalMessage()
{
    msg_in = newPort->readAll();
    QDataStream stream(&msg_in, QIODevice::ReadOnly);;

    // For PLOTS  __________________________________________________________________

    //key1 = QTime::currentTime()/1000;
    static QTime time(QTime::currentTime());
    key1 = time.elapsed()/1000.0;

    imu_roll_d = imu_roll;
    imu_pitch_d = imu_pitch;
    imu_yaw_d = imu_yaw;

    imu_roll_speed_d = imu_roll_speed;
    imu_pitch_speed_d = imu_pitch_speed;
    imu_yaw_speed_d = imu_yaw_speed;

    std::cout << "Got response. First symbol: " << msg_in[0] << std::endl;
    std::cout << "Checksum...";

    if (isCheckSumm16bCorrect(msg_in.data(), RESPONSE_LENGTH)) {
        std::cout << "OK" << std::endl;
        msg_received_counter++;
    } else {
        std::cout << "INCORRECT!" << std::endl;
        msg_lost_counter++;
        return;
    }

    // Defining response data structure
    struct Response_s resp;

    // Moving QByteArray to structure
    stream >> resp;

    // Moving data from resp structure to application
    imu_roll = resp.roll;
    imu_pitch = resp.pitch;
    imu_yaw = resp.yaw;

    imu_roll_speed = resp.rollSpeed;
    imu_pitch_speed = resp.pitchSpeed;
    imu_yaw_speed = resp.yawSpeed;

    imu_pressure = resp.pressure;

    acoustic_state = resp.dev_state;
    leak_sensor = resp.leak_data;
    in_pressure = resp.in_pressure;

    current_HLB = resp.vma_current_hlb;
    current_HLF = resp.vma_current_hlf;
    current_HRB = resp.vma_current_hrb;
    current_HRF = resp.vma_current_hrf;
    current_VB = resp.vma_current_vb;
    current_VF = resp.vma_current_vf;
    current_VL = resp.vma_current_vl;
    current_VR = resp.vma_current_vr;

    current_light = resp.dev_current_light;
    current_tilt = resp.dev_current_tilt;
    current_grab = resp.dev_current_grab;
    current_grab_rotate = resp.dev_current_grab_rotate;
    current_bottom_light = resp.dev_current_dev1;
    current_agar = resp.dev_current_dev2;

    err_vma = resp.vma_errors;
    err_dev = resp.dev_errors;
    err_pc = resp.pc_errors;
}

void Server::receiveConfigMessage()
{
    msg_in.push_back(newPort->readAll());
    QDataStream stream(&msg_in, QIODevice::ReadOnly);;

    std::cout << "Got response. First symbol: " << msg_in[0] << std::endl;
    std::cout << "Checksum...";

    if (isCheckSumm16bCorrect(msg_in.data(), RESPONSE_CONFIG_LENGTH)) {
        std::cout << "OK" << std::endl;
        msg_received_counter++;
    } else {
        std::cout << "INCORRECT!" << std::endl;
        msg_lost_counter++;
        return;
    }

    // Defining response data structure
    struct ConfigResponse_s resp;

    // Moving QByteArray to structure
    stream >> resp;

    imu_roll = resp.roll;
    imu_pitch = resp.pitch;
    imu_yaw = resp.yaw;

    imu_roll_speed = resp.rollSpeed;
    imu_pitch_speed = resp.pitchSpeed;
    imu_yaw_speed = resp.yawSpeed;

    imu_pressure = resp.pressure;
    in_pressure = resp.in_pressure;

    settings->stabContour[settings->CS].stabState.inputSignal = resp.inputSignal;
    settings->stabContour[settings->CS].stabState.speedSignal = resp.speedSignal;
    settings->stabContour[settings->CS].stabState.posSignal = resp.posSignal;

    settings->stabContour[settings->CS].stabState.oldSpeed = resp.oldSpeed;
    settings->stabContour[settings->CS].stabState.oldPos = resp.oldPos;

    settings->stabContour[settings->CS].stabState.joyUnitCasted = resp.joyUnitCasted;
    settings->stabContour[settings->CS].stabState.joy_iValue = resp.joy_iValue;
    settings->stabContour[settings->CS].stabState.posError = resp.posError;
    settings->stabContour[settings->CS].stabState.speedError = resp.speedError;
    settings->stabContour[settings->CS].stabState.dynSummator = resp.dynSummator;
    settings->stabContour[settings->CS].stabState.pidValue = resp.pidValue;
    settings->stabContour[settings->CS].stabState.posErrorAmp = resp.posErrorAmp;
    settings->stabContour[settings->CS].stabState.speedFiltered = resp.speedFiltered;
    settings->stabContour[settings->CS].stabState.posFiltered = resp.posFiltered;

    settings->stabContour[settings->CS].stabState.LastTick = resp.LastTick;

    emit updateCsView();
}

/* CRC16-CCITT algorithm */
uint16_t Server::getCheckSumm16b(char *pcBlock, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    //int crc_fix = reinterpret_cast<int*>(&crc);
    uint8_t i;
    len = len-2;

    while (len--) {
        crc ^= *pcBlock++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

uint8_t Server::isCheckSumm16bCorrect(char *pcBlock, uint16_t len)
{
    uint16_t crc_calculated = getCheckSumm16b(pcBlock, len);

    uint16_t *crc_pointer = reinterpret_cast<uint16_t*>(&pcBlock[len-2]);
    uint16_t crc_got = *crc_pointer;

    if(crc_got == crc_calculated) {
        return true;
    }
    else {
        return false;
    }
}

void Server::addCheckSumm16b(char *pcBlock, uint16_t len)
{
    uint16_t crc = getCheckSumm16b(pcBlock, len);
    uint16_t *crc_pointer = reinterpret_cast<uint16_t*>(&pcBlock[len-2]);
    *crc_pointer = crc;
}

void Server::addFloat(uint8_t * msg, int position, float value) {
    memcpy(msg + position, (unsigned char*) (&value), 4);
}

float Server::getFloat(QByteArray msg, int position) {
    float value = 0;
    memcpy(&value, msg.data()+position, 4);
    return value;
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

void writeCSV(QTextStream stream, uint8_t * msg, uint16_t length) {
    stream << QTime::currentTime().toString();
    for(int i=0; i < length; i++){
        stream << "," << msg[i];
    }
    stream << "\n";
}

Server::~Server() {
    std::cout << "Server shutting down..." << std::endl;
    if (newPort && newPort->isOpen()) {
        std::cout << "Closing port" << std::endl;
        newPort->close();
        newPort->deleteLater();
    }
    std::cout << "Goodbye!" << std::endl;
}

