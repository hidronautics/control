#include "server.h"

const int MAX_COM_ID = 20;

Server::Server(QObject *parent) : QObject(parent)
{
    sendTimer = new QTimer(this);
    connect(sendTimer, SIGNAL(timeout()), this, SLOT( sendMessage() ));
    newPort = 0;
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
    req.grab_rotate = 1.27*(float)j->grab_rotate;
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

    //path_csv_request = log_folder_path + "REQUEST_" + QDateTime::currentDateTime().toString() + ".csv";

    path_csv_request = log_folder_path + "REQUEST.csv";
    QFile file_csv_request(path_csv_request);

    /*
    if(file_csv_request.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream_request(&file_csv_request);
        stream_request << QTime::currentTime().toString() << ":" << QTime::currentTime().msec();
        stream_request << " ;" << j->roll;
        stream_request << " ;" << j->pitch;
        stream_request << " ;" << j->yaw;
        stream_request << " ;" << j->depth;
        stream_request << '\n';
        std::cout << "Request file opened at " << path_csv_request.toStdString() << std::endl;
    } else {
        std::cout << "Unable to open file: " << path_csv_request.toStdString() << std::endl;
    }
    */

    //QFile file_csv_request(path_csv_request);
    if(file_csv_request.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream_request(&file_csv_request);
        //writeCSV(stream_request, msg_to_send, REQUEST_NORMAL_LENGTH);
        stream_request << QTime::currentTime().toString();
        for(int i=0; i < REQUEST_NORMAL_LENGTH; i++){
            stream_request << ";" << msg_to_send[i];
        }
        stream_request << "\n";
    }

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

    QDataStream stream(&msg_to_send, QIODevice::Append);

    // Defining request structure
    ConfigRequest_s req;

    // Filling request structure
    req.type = REQUEST_CONFIG_CODE;

    req.depth_k1            = settings->depth.k1;
    req.depth_k2            = settings->depth.k2;
    req.depth_k3            = settings->depth.k3;
    req.depth_k4            = settings->depth.k4;
    req.depth_iborders      = settings->depth.iborders;
    req.depth_pgain         = settings->depth.pgain;
    req.depth_igain         = settings->depth.igain;

    req.roll_k1             = settings->roll.k1;
    req.roll_k2             = settings->roll.k2;
    req.roll_k3             = settings->roll.k3;
    req.roll_k4             = settings->roll.k4;
    req.roll_iborders       = settings->roll.iborders;
    req.roll_pgain          = settings->roll.pgain;
    req.roll_igain          = settings->roll.igain;

    req.pitch_k1            = settings->pitch.k1;
    req.pitch_k2            = settings->pitch.k2;
    req.pitch_k3            = settings->pitch.k3;
    req.pitch_k4            = settings->pitch.k4;
    req.pitch_iborders      = settings->pitch.iborders;
    req.pitch_pgain         = settings->pitch.pgain;
    req.pitch_igain         = settings->pitch.igain;

    req.yaw_k1              = settings->yaw.k1;
    req.yaw_k2              = settings->yaw.k2;
    req.yaw_k3              = settings->yaw.k3;
    req.yaw_k4              = settings->yaw.k4;
    req.yaw_iborders        = settings->yaw.iborders;
    req.yaw_pgain           = settings->yaw.pgain;
    req.yaw_igain           = settings->yaw.igain;

    // TODO what was this? warning
    //settings->motors;
/*
    for (int i = 0; i < 8; ++i) {
        switch (settings->motors[i].code) {
        case settings->motors[i].HLB:
            req.position_hlb = i;
            req.setting_hlb = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                req.kbackward_hlb = settings->motors[i].k_backward;
                req.kforward_hlb = settings->motors[i].k_forward;
            } else {
                req.kbackward_hlb = 0;
                req.kforward_hlb = 0;
            }
            break;
        case settings->motors[i].HLF:
            req.position_hlf = i;
            req.setting_hlf = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                req.kbackward_hlf = settings->motors[i].k_backward;
                req.kforward_hlf = settings->motors[i].k_forward;
            } else {
                req.kbackward_hlf = 0;
                req.kforward_hlf = 0;
            }
            break;
        case settings->motors[i].HRB:
            req.position_hrb = i;
            req.setting_hrb = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                req.kbackward_hrb = settings->motors[i].k_backward;
                req.kforward_hrb = settings->motors[i].k_forward;
            } else {
                req.kbackward_hrb = 0;
                req.kforward_hrb = 0;
            }
            break;
        case settings->motors[i].HRF:
            req.position_hrf = i;
            req.setting_hrf = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                req.kbackward_hrf = settings->motors[i].k_backward;
                req.kforward_hrf = settings->motors[i].k_forward;
            } else {
                req.kbackward_hrf = 0;
                req.kforward_hrf = 0;
            }
            break;
        case settings->motors[i].VB:
            req.position_vb = i;
            req.setting_vb = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                req.kbackward_vb = settings->motors[i].k_backward;
                req.kforward_vb = settings->motors[i].k_forward;
            } else {
                req.kbackward_vb = 0;
                req.kforward_vb = 0;
            }
            break;
        case settings->motors[i].VF:
            req.position_vf = i;
            req.setting_vf = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                req.kbackward_vf = settings->motors[i].k_backward;
                req.kforward_vf = settings->motors[i].k_forward;
            } else {
                req.kbackward_vf = 0;
                req.kforward_vf = 0;
            }
            break;
        case settings->motors[i].VL:
            req.position_vl = i;
            req.setting_vl = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                req.kbackward_vl = settings->motors[i].k_backward;
                req.kforward_vl = settings->motors[i].k_forward;
            } else {
                req.kbackward_vl = 0;
                req.kforward_vl = 0;
            }
            break;
        case settings->motors[i].VR:
            req.position_vr = i;
            req.setting_vr = settings->motors[i].inverse;
            if (settings->motors[i].enabled) {
                req.kbackward_vr = settings->motors[i].k_backward;
                req.kforward_vr = settings->motors[i].k_forward;
            } else {
                req.kbackward_vr = 0;
                req.kforward_vr = 0;
            }
            break;
        default:
            std::cout << "ERROR: unknown motor code!" << std::endl;
        }

    }*/

    // Moving structure to QByteArray
    stream << req;

    // Calculating checksum
    uint16_t checksum = getCheckSumm16b(&msg_to_send, REQUEST_NORMAL_LENGTH);

    // Moving checksum to QByteArray
    stream << checksum;

    /*
    std::cout << "Sending CONFIG message:" << std::endl;
    for (int i = 0; i < REQUEST_CONFIG_LENGTH; ++i) {
        std::cout << "|N" << i << "=" << unsigned(msg_to_send[i]) << std::endl;
    }
    */

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

    emit imSleeping();
    QTest::qSleep (settings->connection->pause_after_sent);
    receiveMessage();
}

void Server::receiveMessage() {
    std::cout << "Server::receiveMessage()" << std::endl;
    int buffer_size = 0;
    if(!emulation_mode) {
        newPort->waitForReadyRead(25);
        buffer_size = newPort->bytesAvailable();
    } else {
        std::cout << "WARNING: Emulation mode" << std::endl;
    }
    std::cout << "In input buffer there are " << buffer_size << " bytes availible" << std::endl;

    if (emulation_mode) {
        imu_roll = imu_roll + j->roll/10000;
        imu_pitch = imu_pitch + j->pitch/10000;
        imu_yaw = imu_yaw + j->yaw/10000;

        imu_roll_speed = j->roll/10000;
        imu_pitch_speed = j->pitch/10000;
        imu_yaw_speed = j->yaw/10000;

        imu_pressure = imu_pressure + j->depth/10000;

        current_HLB = 1;
        current_HLF = 11;
        current_HRB = 111;
        current_HRF = 1111;
        current_VB = 11111;
        current_VF = 234;
        current_VL = 4567;
        current_VR = 7808;

        velocity_HLB = 87;
        velocity_HLF = 89;
        velocity_HRB = 90;
        velocity_HRF = 35;
        velocity_VB = 76;
        velocity_VF = 88;
        velocity_VL = 99;
        velocity_VR = 101; // value 999 was obviously incorrect here

        current_light = 5;
        current_bottom_light = 10;
        current_agar = 15;
        current_grab = 20;
        current_grab_rotate = 25;
        current_tilt = 30;

        bt_data = "testMSG";


    } else if (buffer_size == 0) {
        std::cout << "No message to read. Buffer is empty" << std::endl;
        msg_lost_counter++;
    } else {
        int counter = 0;
        while (true) {
            int bytesAvailible = newPort->bytesAvailable();
            if (bytesAvailible % RESPONSE_LENGTH == 0) {
                break;
            } else if (counter > 250000) {
                newPort->clear();
            } else {
                counter++;
            }
        }

        msg_in = newPort->readAll();
        QDataStream stream(&msg_in, QIODevice::ReadOnly);;

        QFile file_csv_response(path_csv_response);
        if(file_csv_response.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream stream_response(&file_csv_response);
            stream_response << QTime::currentTime().toString();
            for(int i=0; i < REQUEST_CONFIG_LENGTH; i++){
                stream_response << ";" << msg_in[i];
            }
            stream_response << "\n";
        }

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
        //______________________________________________________________________________

        //path_csv_response = log_folder_path + "RESPONSE_" + QDateTime::currentDateTime().toString() + ".csv";

        path_csv_response = log_folder_path + "RESPONSE.csv";

        /*
        std::cout << "Opening log file" << std::endl;
        QFile file_csv_response(path_csv_response);
        if(file_csv_response.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream stream_response(&file_csv_response);
            stream_response << QTime::currentTime().toString() << ":" << QTime::currentTime().msec();
            stream_response << " ;" << imu_roll;
            stream_response << " ;" << imu_pitch;
            stream_response << " ;" << imu_roll_speed;
            stream_response << " ;" << imu_pitch_speed;
            stream_response << '\n';
            std::cout << "Response file opened at " << path_csv_response.toStdString() << std::endl;
        }
        */

        std::cout << "Got response. First symbol: " << msg_in[0] << std::endl;
        std::cout << "Checksum...";

        if (isCheckSumm16bCorrect((uint8_t*) msg_in.data(), RESPONSE_LENGTH)) {
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

        imu_pressure = resp.pressure; // Pressure вместо depth

        wf_data_type = resp.wf_type;
        wf_tick_rate = resp.wf_tickrate;
        wf_voltage = resp.wf_voltage;

        switch(wf_data_type){
        case 0:
            wf_x_angle_float = resp.wf_x;
            wf_y_angle_float = resp.wf_y;
            break;
        case 1:
            //wf_x_angle_int32_t = (int32_t) (((msg_in[RESPONSE_WF_X_ANGLE]) << 8 | msg_in[RESPONSE_WF_X_ANGLE+1]) << 8 | msg_in[RESPONSE_WF_X_ANGLE+2])<< 8 | msg_in[RESPONSE_WF_X_ANGLE+3];
            //wf_x_angle_int32_t = (int32_t) (((msg_in[RESPONSE_WF_Y_ANGLE]) << 8 | msg_in[RESPONSE_WF_Y_ANGLE+1]) << 8 | msg_in[RESPONSE_WF_Y_ANGLE+2])<< 8 | msg_in[RESPONSE_WF_Y_ANGLE+3];
            break;
        default:
            std::cout << "Wifi data type ERROR" << std::endl;
        }

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

        velocity_HLB = resp.vma_velocity_hlb;
        velocity_HLF = resp.vma_velocity_hlf;
        velocity_HRB = resp.vma_velocity_hrb;
        velocity_HRF = resp.vma_velocity_hrf;
        velocity_VB = resp.vma_velocity_vb;
        velocity_VF = resp.vma_velocity_vf;
        velocity_VL = resp.vma_velocity_vl;
        velocity_VR = resp.vma_velocity_vr;

        current_light = resp.dev_current_light;
        current_tilt = resp.dev_current_tilt;
        current_grab = resp.dev_current_grab;
        current_grab_rotate = resp.dev_current_grab_rotate;
        // TODO there are no more stuff like that
        current_bottom_light = resp.dev_current_dev1;
        current_agar = resp.dev_current_dev2;

        err_vma = resp.vma_errors;
        err_dev = resp.dev_errors;
        err_pc = resp.pc_errors;
    }
    //QTest::qSleep (settings->connection->pause_after_received);
    msg_lost_percent = (float) msg_lost_counter / ((float)msg_received_counter+ (float)msg_lost_counter);
    msg_lost_percent *= 100;

    std::cout << "RECEIVED = " << msg_received_counter << std::endl;
    std::cout << "LOST = " << msg_lost_counter << std::endl;
    std::cout << "LOST PERCENT = " << msg_lost_percent << std::endl;
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

