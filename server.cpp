#include "server.h"

const int MAX_COM_ID = 20;

Server::Server(QObject *parent) :
    QObject(parent),
    sendTimer(new QTimer(this)),
    _udp(new UDPClient(this))
{
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

    if (isOpened ) {
        std::cout << " successfully opened!" << std::endl;
        _com_enable = true;
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
        message_type = REQUEST_NORMAL_CODE;
        sendMessageNormal();
        break;
    case REQUEST_CONFIG_CODE:
        message_type = REQUEST_CONFIG_CODE;
        sendMessageConfig();
        break;
    case REQUEST_DIRECT_CODE:
        message_type = REQUEST_DIRECT_CODE;
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

void Server::start_qualification_task()
{
    start_qualification = true;
}


void Server::flash_thruster()
{
    flash_thruster_settings = true;
}


void Server::reset_IMU()
{
    reset_imu = true;
}


void Server::set_stabilize_roll(bool enabled)
{
    stabilize_roll = enabled;
}


void Server::set_stabilize_pitch(bool enabled)
{
    stabilize_pitch = enabled;
}


void Server::set_stabilize_yaw(bool enabled)
{
    stabilize_yaw = enabled;
}


void Server::set_stabilize_depth(bool enabled)
{
    stabilize_depth = enabled;
}


bool Server::pick_bit(uint8_t &input, uint8_t bit)
{
    return static_cast<bool>((input << (8 - bit)) >> 8);
}


void Server::set_bit(uint8_t &byte, uint8_t bit, bool state)
{
    uint8_t value = 1;
    if(state) {
        byte = byte | (value << bit);
    }
    else {
        byte = byte & ~(value << bit);
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

    req.march = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->march));
    req.lag = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->lag));
    req.depth = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->depth));

    req.roll = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->roll));
    req.pitch = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->pitch));
    req.yaw = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->yaw));

    req.light = j->light;
    req.grab = j->grab;
    req.tilt = j->tilt;
    req.grab_rotate = static_cast<int8_t>(j->grab_rotate);
    req.dev1 = 0;
    req.dev2 = 0;

    req.dev_flags = 0;
    req.stabilize_flags = 0;

    if (flash_thruster_settings) {
        set_bit(req.stabilize_flags, SHORE_STABILIZE_SAVE_BIT, true);
        flash_thruster_settings = false;
    }

    if (reset_imu) {
        set_bit(req.stabilize_flags, SHORE_STABILIZE_RESET_IMU_BIT, true);
        reset_imu = false;
    }

    if (start_qualification) {
        set_bit(req.stabilize_flags, SHORE_START_QUALIFICATION_BIT, true);
        start_qualification = false;
    }

    set_bit(req.stabilize_flags, SHORE_STABILIZE_ROLL_BIT, stabilize_roll);
    set_bit(req.stabilize_flags, SHORE_STABILIZE_PITCH_BIT, stabilize_pitch);
    set_bit(req.stabilize_flags, SHORE_STABILIZE_YAW_BIT, stabilize_yaw);
    set_bit(req.stabilize_flags, SHORE_STABILIZE_DEPTH_BIT, stabilize_depth);

    req.cameras = j->camera;
    req.pc_reset = settings->pcreset;

    // Moving structure to QByteArray
    stream << req;

    // Calculating checksum
    uint16_t checksum = getCheckSumm16b(msg_to_send.data(), REQUEST_NORMAL_LENGTH);

    // Moving checksum to QByteArray
    stream << checksum;

    if (_udp_enable) {
        _udp->request(msg_to_send);
    }
    if (_com_enable) {
        newPort->write(msg_to_send, REQUEST_NORMAL_LENGTH);
    }
}

void Server::sendMessageDirect() {
    std::cout << "Server::sendMessageDirect()" << std::endl;

    msg_to_send.clear();
    QDataStream stream(&msg_to_send, QIODevice::Append);

    // Filling request structure
    RequestDirect_s req;
    req.type = REQUEST_DIRECT_CODE;
    req.number = settings->current_thrusters_numb;
    req.id = settings->thrusters_configs[req.number].id;
    req.velocity = settings->thrusters_configs[req.number].velocity;
    req.reverse = settings->thrusters_configs[req.number].reverse;
    req.kForward = settings->thrusters_configs[req.number].kForward;
    req.kBackward = settings->thrusters_configs[req.number].kBackward;
    req.forward_saturation = settings->thrusters_configs[req.number].forward_saturation;
    req.backward_saturation = settings->thrusters_configs[req.number].backward_saturation;

    // Moving structure to QByteArray
    stream << req;

    // Calculating checksum
    uint16_t checksum = getCheckSumm16b(msg_to_send.data(), REQUEST_DIRECT_LENGTH);

    // Moving checksum to QByteArray
    stream << checksum;

    if (_udp_enable) {
        _udp->request(msg_to_send);
    }
    if (_com_enable) {
        newPort->write(msg_to_send, REQUEST_DIRECT_LENGTH);
    }
}


void Server::sendMessageConfig() {
    std::cout << "Server::sendMessageConfig()" << std::endl;
    j->update();

    msg_to_send.clear();
    QDataStream stream(&msg_to_send, QIODevice::Append);

    ConfigRequest_s req;

    req.type = REQUEST_CONFIG_CODE;
    req.contour = static_cast<uint8_t>(settings->CS);

    req.march = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->march));
    req.lag = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->lag));
    req.depth = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->depth));
    req.roll = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->roll));
    req.pitch = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->pitch));
    req.yaw = static_cast<int16_t>(settings->controlMultiplier*static_cast<float>(j->yaw));

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

    if (_udp_enable) {
        _udp->request(msg_to_send);
    }
    if (_com_enable) {
        newPort->write(msg_to_send.data(), REQUEST_CONFIG_LENGTH);
    }

    settings->oldCS = settings->CS;

    emit imSleeping();
    QTest::qSleep (settings->connection->pause_after_sent);
    receiveMessage();
}


void Server::connect_udp(const QString &host, const QString &port)
{
    _udp->set_address(host, port);
    _udp_enable = true;
    sendTimer->start(REQUEST_DELAY);
}


void Server::disconnect_udp()
{
    _udp_enable = false;
    if (!_udp_enable) {
        sendTimer->stop();
    }
}


void Server::receiveMessage() {
    if (!_udp_enable) {
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
                    switch(message_type) {
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
}

void Server::receiveNormalMessage()
{
    if (_com_enable) {
        msg_in.clear();
        msg_in.push_back(newPort->readAll());
        QDataStream stream(&msg_in, QIODevice::ReadOnly);

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
}

void Server::receiveConfigMessage()
{
    if (_com_enable) {
        msg_in.clear();
        msg_in.push_back(newPort->readAll());
        QDataStream stream(&msg_in, QIODevice::ReadOnly);;

    //    std::cout << "Got response. First symbol: " << msg_in[0] << std::endl;
    //    std::cout << "Checksum...";

        if (isCheckSumm16bCorrect(msg_in.data(), RESPONSE_CONFIG_LENGTH)) {
    //        std::cout << "OK" << std::endl;
            msg_received_counter++;
        } else {
    //        std::cout << "INCORRECT!" << std::endl;
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
        imu_raw_yaw = resp.raw_yaw;

        imu_roll_speed = resp.rollSpeed;
        imu_pitch_speed = resp.pitchSpeed;
        imu_yaw_speed = resp.yawSpeed;

        imu_pressure = resp.pressure;
        in_pressure = resp.in_pressure;

        settings->stabContour[settings->CS].stabState.inputSignal = resp.inputSignal;
        settings->stabContour[settings->CS].stabState.speedSignal = resp.speedSignal;
        settings->stabContour[settings->CS].stabState.posSignal = resp.posSignal;

        settings->stabContour[settings->CS].stabState.joyUnitCasted = resp.joyUnitCasted;
        settings->stabContour[settings->CS].stabState.joy_iValue = resp.joy_iValue;
        settings->stabContour[settings->CS].stabState.posError = resp.posError;
        settings->stabContour[settings->CS].stabState.speedError = resp.speedError;
        settings->stabContour[settings->CS].stabState.dynSummator = resp.dynSummator;
        settings->stabContour[settings->CS].stabState.pidValue = resp.pidValue;
        settings->stabContour[settings->CS].stabState.posErrorAmp = resp.posErrorAmp;
        settings->stabContour[settings->CS].stabState.speedFiltered = resp.speedFiltered;
        settings->stabContour[settings->CS].stabState.posFiltered = resp.posFiltered;

        settings->stabContour[settings->CS].stabState.pid_iValue = resp.pid_iValue;

        emit updateCsView();
    }
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
    _com_enable = false;
    if (!_udp_enable) {
        sendTimer->stop();
    }
    newPort->disconnect();
    newPort->close();
    newPort->deleteLater();
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
    delete sendTimer;
    delete _udp;
}

