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

    QString str = "/dev/ttyUSB";
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
    uint16_t checksum = getCheckSumm16b(&msg_to_send, REQUEST_NORMAL_LENGTH);

    // Moving checksum to QByteArray
    stream << checksum;

    /*
    std::cout << "Sending NORMAL message:" << std::endl;
    for (int i = 0; i < REQUEST_NORMAL_LENGTH; ++i) {
        std::cout << "|N" << i << "=" << unsigned(msg_to_send[i]) << std::endl;
    }
    */

    newPort->write(msg_to_send, REQUEST_NORMAL_LENGTH);

    emit imSleeping();

    std::cout << "Go sleeping for " << settings->connection->pause_after_sent << " ms" << std::endl;
    QTest::qSleep (settings->connection->pause_after_sent);
    receiveMessage();
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

    msg_to_send.clear();
    QDataStream stream(&msg_to_send, QIODevice::Append);

    stream << static_cast<uint8_t>(REQUEST_CONFIG_CODE);

    stream << settings->CS;

    stream << j->march;
    stream << j->lag;
    stream << j->depth;

    stream << j->roll;
    stream << j->pitch;
    stream << j->yaw;

    stream << settings->stabContour[settings->CS].stabConstants;

    // Calculating checksum
    uint16_t checksum = getCheckSumm16b(&msg_to_send, REQUEST_NORMAL_LENGTH);

    // Moving checksum to QByteArray
    stream << checksum;

    QFile file_csv_request(path_csv_request);
    if(file_csv_request.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream_request(&file_csv_request);
        //writeCSV(stream_request, msg_to_send, REQUEST_NORMAL_LENGTH);
        stream_request << QTime::currentTime().toString();
        for(int i=0; i < REQUEST_CONFIG_LENGTH; i++){
            stream_request << ";" << msg_to_send[i];
        }
        stream_request << "\n";
    }

    newPort->write(msg_to_send, REQUEST_CONFIG_LENGTH);

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
                msg_in = newPort->read(1);
                switch(msg_in[0]) {
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

    if (isCheckSumm16bCorrect(reinterpret_cast<uint8_t*>(msg_in.data()), RESPONSE_LENGTH)) {
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
    msg_in = newPort->readAll();
    QDataStream stream(&msg_in, QIODevice::ReadOnly);;

    std::cout << "Got response. First symbol: " << msg_in[0] << std::endl;
    std::cout << "Checksum...";

    if (isCheckSumm16bCorrect(reinterpret_cast<uint8_t*>(msg_in.data()), RESPONSE_LENGTH)) {
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
    stream >> imu_roll;
    stream >> imu_pitch;
    stream >> imu_yaw;

    stream >> imu_roll_speed;
    stream >> imu_pitch_speed;
    stream >> imu_yaw_speed;

    stream >> imu_pressure;

    stream >> settings->stabContour[settings->oldCS].stabState;

    emit updateCsView();
}

/* CRC16-CCITT algorithm */
uint8_t Server::isCheckSumm16bCorrect(uint8_t * msg, uint16_t length)
{
    uint16_t crcGot, crc = 0;
    int i;

    crcGot = (uint16_t)( msg[length-1] + (msg[length-2] << 8) );

    for(i=0; i < length - 2; i++){ //i теперь не с 0, а с 1
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

void Server::addCheckSumm16b(uint8_t * msg, uint16_t length)//i теперь не с 0, а с 1
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

uint16_t Server::getCheckSumm16b(QByteArray * msg, uint16_t length)
{
    uint16_t crc = 0;
    int i = 0;

    for(i=0; i < length - 2; i++){
        crc = (uint8_t)(crc >> 8) | (crc << 8);
        crc ^= ((uint8_t*) msg->data())[i];
        crc ^= (uint8_t)(crc & 0xff) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xff) << 4) << 1;
    }

    return crc;
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

