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
    QJsonObject cs_json[STABILIZATION_AMOUNT];
    cs_json[STAB_DEPTH]  = stabilization_json["depth"].toObject();
    cs_json[STAB_ROLL]   = stabilization_json["roll"].toObject();
    cs_json[STAB_PITCH]  = stabilization_json["pitch"].toObject();
    cs_json[STAB_YAW]    = stabilization_json["yaw"].toObject();

    for(int i=0; i<STABILIZATION_AMOUNT; i++) {
        stabContour[i].stabConstants.pJoyUnitCast = static_cast<float>(cs_json[i]["pJoyUnitCast"].toDouble());
        stabContour[i].stabConstants.pSpeedDyn = static_cast<float>(cs_json[i]["pSpeedDyn"].toDouble());
        stabContour[i].stabConstants.pErrGain = static_cast<float>(cs_json[i]["pErrGain"].toDouble());
        stabContour[i].stabConstants.pid.pGain = static_cast<float>(cs_json[i]["pid.pGain"].toDouble());
        stabContour[i].stabConstants.pid.iGain = static_cast<float>(cs_json[i]["pid.iGain"].toDouble());
        stabContour[i].stabConstants.pid.iMax = static_cast<float>(cs_json[i]["pid.iMax"].toDouble());
        stabContour[i].stabConstants.pid.iMin = static_cast<float>(cs_json[i]["pid.iMin"].toDouble());
        stabContour[i].stabConstants.pThrustersCast = static_cast<float>(cs_json[i]["pThrustersCast"].toDouble());
        stabContour[i].stabConstants.pThrustersMin = static_cast<float>(cs_json[i]["pThrustersMin"].toDouble());
        stabContour[i].stabConstants.pThrustersMax = static_cast<float>(cs_json[i]["pThrustersMax"].toDouble());

        stabContour[i].stabConstants.aFilter[POS_FILTER].K = static_cast<float>(cs_json[i]["POS_FILTER.K"].toDouble());
        stabContour[i].stabConstants.aFilter[POS_FILTER].T = static_cast<float>(cs_json[i]["POS_FILTER.T"].toDouble());
        stabContour[i].stabConstants.aFilter[SPEED_FILTER].K = static_cast<float>(cs_json[i]["SPEED_FILTER.K"].toDouble());
        stabContour[i].stabConstants.aFilter[SPEED_FILTER].T = static_cast<float>(cs_json[i]["SPEED_FILTER.T"].toDouble());

        std::cout << "STABILIZATION NUMBER number: " << i << std::endl;
        std::cout << "STABILIZATION SPEEDDYN: " << stabContour[i].stabConstants.pSpeedDyn << std::endl;
    }

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

    QJsonObject cs_json[STABILIZATION_AMOUNT];

    for(int i=0; i<STABILIZATION_AMOUNT; i++) {
        cs_json[i]["pJoyUnitCast"] = static_cast<double>(stabContour[i].stabConstants.pJoyUnitCast);
        cs_json[i]["pSpeedDyn"] = static_cast<double>(stabContour[i].stabConstants.pSpeedDyn);
        cs_json[i]["pErrGain"] = static_cast<double>(stabContour[i].stabConstants.pErrGain);
        cs_json[i]["pid.pGain"] = static_cast<double>(stabContour[i].stabConstants.pid.pGain);
        cs_json[i]["pid.iGain"] = static_cast<double>(stabContour[i].stabConstants.pid.iGain);
        cs_json[i]["pid.iMax"] = static_cast<double>(stabContour[i].stabConstants.pid.iMax);
        cs_json[i]["pid.iMin"] = static_cast<double>(stabContour[i].stabConstants.pid.iMin);
        cs_json[i]["pThrustersCast"] = static_cast<double>(stabContour[i].stabConstants.pThrustersCast);
        cs_json[i]["pThrustersMin"] = static_cast<double>(stabContour[i].stabConstants.pThrustersMin);
        cs_json[i]["pThrustersMax"] = static_cast<double>(stabContour[i].stabConstants.pThrustersMax);

        cs_json[i]["POS_FILTER.K"] = static_cast<double>(stabContour[i].stabConstants.aFilter[POS_FILTER].K);
        cs_json[i]["POS_FILTER.T"] = static_cast<double>(stabContour[i].stabConstants.aFilter[POS_FILTER].T);
        cs_json[i]["SPEED_FILTER.K"] = static_cast<double>(stabContour[i].stabConstants.aFilter[SPEED_FILTER].K);
        cs_json[i]["SPEED_FILTER.T"] = static_cast<double>(stabContour[i].stabConstants.aFilter[SPEED_FILTER].T);
    }

    QJsonObject stabilization_json;
    stabilization_json["depth"] = cs_json[STAB_DEPTH];
    stabilization_json["roll"] =  cs_json[STAB_ROLL];
    stabilization_json["pitch"] = cs_json[STAB_PITCH];
    stabilization_json["yaw"] = cs_json[STAB_YAW];

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

void Settings::jetson_on_off_btn_clicked(bool checked)
{
    if (checked) {
        std::cout << "Jetson puttong pressed...";
        this->pcreset = 0xAA;
    }
    else {
        std::cout << "Jetson puttong released";
        this->pcreset = 0x00;
    }
}
