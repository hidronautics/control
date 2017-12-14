#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    motors = new Motor[8];
    connection = new Connection();
    initialize();
}

void Settings::initialize() {
    std::cout << "Setting default config" << std::endl;
    for (int i = 0; i < 8; ++i) {
        motors[i].code = Motor::MotorCode::VF;
        motors[i].adress = i;
        motors[i].inverse = false;
        motors[i].enabled = true;
        motors[i].speed = 0;
        motors[i].k_backward = 1.;
        motors[i].k_forward = 1.;
    }

    connection->num = 1;
    connection->baudRate = QSerialPort::BaudRate::Baud57600;
    connection->dataBits = QSerialPort::DataBits::Data8;
    connection->parity = QSerialPort::Parity::NoParity;
    connection->stopBits = QSerialPort::StopBits::OneStop;
    connection->flowControl = QSerialPort::FlowControl::NoFlowControl;
    connection->pause_after_sent = 50;
    connection->pause_after_received = 50;
}



void Settings::read(const QJsonObject &json)
{
    QJsonArray motorArray = json["motors"].toArray();
    for (int i = 0; i < motorArray.size(); ++i) {
        QJsonObject motorObject = motorArray[i].toObject();

        if (i == motorObject["slot"].toInt()) {
            std::cout << "Parsing slot " << i << " settings..." << std::endl;
            motors[i].setCode(motorObject["code"].toString());
            std::cout << "code = " << motors[i].getCode().toStdString() << std::endl;
            motors[i].inverse = motorObject["inverse"].toBool();
            std::cout << "inverse = " << motors[i].inverse << std::endl;
            motors[i].enabled = motorObject["enabled"].toBool();
            std::cout << "enabled = " << motors[i].enabled << std::endl;
            motors[i].k_backward = motorObject["k_backward"].toDouble();
            std::cout << "k_backward = " << motors[i].k_backward << std::endl;
            motors[i].k_forward = motorObject["k_forward"].toDouble();
            std::cout << "k_forward = " << motors[i].k_forward << std::endl;
            std::cout << std::endl;
        } else {
            std::cout << "Wrong slot order in JSON file" << std::endl;
            std::cout << i << " expected, " << motorObject["slot"].toInt() << " got." << std::endl;
        }

    }

    QJsonObject stabilization_json = json["stabilization"].toObject();

    QJsonObject depth_json  = stabilization_json["depth"].toObject();
    QJsonObject roll_json   = stabilization_json["roll"].toObject();
    QJsonObject pitch_json  = stabilization_json["pitch"].toObject();
    QJsonObject yaw_json    = stabilization_json["yaw"].toObject();

    depth.k1         = depth_json["k1"].toDouble();
    depth.k2         = depth_json["k2"].toDouble();
    depth.start      = depth_json["start"].toDouble();
    depth.gain       = depth_json["gain"].toDouble();
    depth.const_time = depth_json["const_time"].toBool();

    roll.k1         = roll_json["k1"].toDouble();
    roll.k2         = roll_json["k2"].toDouble();
    roll.start      = roll_json["start"].toDouble();
    roll.gain       = roll_json["gain"].toDouble();
    roll.const_time = roll_json["const_time"].toBool();

    pitch.k1         = pitch_json["k1"].toDouble();
    pitch.k2         = pitch_json["k2"].toDouble();
    pitch.start      = pitch_json["start"].toDouble();
    pitch.gain       = pitch_json["gain"].toDouble();
    pitch.const_time = pitch_json["const_time"].toBool();

    yaw.k1         = yaw_json["k1"].toDouble();
    yaw.k2         = yaw_json["k2"].toDouble();
    yaw.start      = yaw_json["start"].toDouble();
    yaw.gain       = yaw_json["gain"].toDouble();
    yaw.const_time = yaw_json["const_time"].toBool();

    QJsonObject connection_json = json["connection"].toObject();

    int n;
    QString s;

    n = connection_json["comNumber"].toInt();
    if (n != 0) connection->num = n;
    std::cout << "COM number: " << connection->num << std::endl;

    n = connection_json["baudRate"].toInt();
    if (n != 0) connection->baudRate = connection->getBaudRate(n);
    std::cout << "baudRate: " << connection->baudRate << std::endl;

    n = connection_json["dataBits"].toInt();
    if (n != 0) connection->dataBits = connection->getDataBits(n);
    std::cout << "dataBits: " << connection->dataBits << std::endl;

    s = connection_json["parity"].toString();
    if (s.compare("")) connection->parity = connection->getParity(s);
    std::cout << "parity: " << connection->parity << std::endl;

    s = connection_json["stopBits"].toString();
    if (s.compare("")) connection->stopBits = connection->getStopBits(s);
    std::cout << "stopBits: " << connection->stopBits << std::endl;

    s = connection_json["flowControl"].toString();
    if (s.compare("")) connection->flowControl = connection->getFlowControl(s);
    std::cout << "flowControl: " << connection->flowControl << std::endl;

    connection->pause_after_sent = connection_json["pauseAfterSent"].toInt();
    connection->pause_after_received = connection_json["pauseAfterReceived"].toInt();

}

void Settings::write(QJsonObject &json) const
{
    QJsonArray motorArray;

    for (int i = 0; i < 8; ++i) {
        QJsonObject motorObject;

        motorObject["slot"] = i;
        motorObject["code"] = motors[i].getCode();
        motorObject["inverse"] = motors[i].inverse;
        motorObject["enabled"] = motors[i].enabled;
        motorObject["k_backward"] = motors[i].k_backward;
        motorObject["k_forward"] = motors[i].k_forward;

        motorArray.append(motorObject);
    }

    json["motors"] = motorArray;

    QJsonObject depth_json;
    QJsonObject roll_json;
    QJsonObject pitch_json;
    QJsonObject yaw_json;


    depth_json["k1"]        = depth.k1;
    depth_json["k2"]        = depth.k2;
    depth_json["start"]     = depth.start;
    depth_json["gain"]      = depth.gain;
    depth_json["const_time"] = depth.const_time;

    roll_json["k1"]         = roll.k1;
    roll_json["k2"]         = roll.k2;
    roll_json["start"]      = roll.start;
    roll_json["gain"]       = roll.gain;
    roll_json["const_time"] = roll.const_time;

    pitch_json["k1"]        = pitch.k1;
    pitch_json["k2"]        = pitch.k2;
    pitch_json["start"]     = pitch.start;
    pitch_json["gain"]      = pitch.gain;
    pitch_json["const_time"] = pitch.const_time;

    yaw_json["k1"]          = yaw.k1;
    yaw_json["k2"]          = yaw.k2;
    yaw_json["start"]       = yaw.start;
    yaw_json["gain"]        = yaw.gain;
    yaw_json["const_time"]  = yaw.const_time;

    QJsonObject stabilization_json;

    stabilization_json["depth"] = depth_json;
    stabilization_json["roll"]  = roll_json;
    stabilization_json["pitch"] = pitch_json;
    stabilization_json["yaw"]   = yaw_json;

    json["stabilization"] = stabilization_json;

    QJsonObject connection_json;

    std::cout << "!!!" << connection->baudRate << std::endl;
    std::cout << "!!!" << connection->dataBits << std::endl;

    connection_json["comNumber"] = connection->num;
    connection_json["baudRate"] = connection->setBaudRate(connection->baudRate);
    connection_json["dataBits"] = connection->setDataBits(connection->dataBits);
    connection_json["parity"] = connection->setParity(connection->parity);
    connection_json["stopBits"] = connection->setStopBits(connection->stopBits);
    connection_json["flowControl"] = connection->setFlowControl(connection->flowControl);

    connection_json["pauseAfterSent"] = connection->pause_after_sent;
    connection_json["pauseAfterReceived"] = connection->pause_after_received;

    json["connection"] = connection_json;
}


bool Settings::loadFromJSONFile() {
    QFile loadFile("config.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    read(loadDoc.object());
    std::cout << "Settings loaded" << std::endl;
    return true;
}


bool Settings::saveToJSONFIle() const{
    QFile saveFile("config.json");

    if (!saveFile.open(QIODevice::WriteOnly)) {
        std::cout << "Couldn't open save file." << std::endl;
        return false;
    }

    QJsonObject gameObject;
    write(gameObject);
    QJsonDocument saveDoc(gameObject);
    saveFile.write(saveDoc.toJson(QJsonDocument::Indented));
    std::cout << "Settings saved" << std::endl;
    return true;
}

void Settings::changeMotorSetting(int slot, QString motorID, bool inverse) {
    saveToJSONFIle();
    loadFromJSONFile();
    std::cout << "Motor " << motorID.toStdString() << " is now binded to " << slot << " slot"
              << (inverse?" ":" not ") << "inverse" << std::endl;
}
