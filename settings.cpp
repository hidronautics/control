#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
    connection = new Connection();
    initialize();
}

void Settings::initialize() {
    std::cout << "Setting default config" << std::endl;

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
    // Thrusters configuration
    QJsonObject thrusters_json = json["thrusters"].toObject();
    QJsonObject thruster_json[THRUSTERS_AMOUNT];

    thruster_json[HLB] = thrusters_json["HLB"].toObject();
    thruster_json[HLF] = thrusters_json["HLF"].toObject();
    thruster_json[HRB] = thrusters_json["HRB"].toObject();
    thruster_json[HRF] = thrusters_json["HRF"].toObject();
    thruster_json[VB] = thrusters_json["VB"].toObject();
    thruster_json[VF] = thrusters_json["VF"].toObject();
    thruster_json[VL] = thrusters_json["VL"].toObject();
    thruster_json[VR] = thrusters_json["VR"].toObject();

    for (uint i = 0; i < THRUSTERS_AMOUNT; ++i) {
        thrusters_configs[i].id = static_cast<uint8_t>(thruster_json[i]["id"].toInt());
        thrusters_configs[i].kForward = thruster_json[i]["kForward"].toInt();
        thrusters_configs[i].kBackward = thruster_json[i]["kBackward"].toInt();
        thrusters_configs[i].forward_saturation = static_cast<int16_t>(thruster_json[i]["forward_saturation"].toInt());
        thrusters_configs[i].backward_saturation = static_cast<int16_t>(thruster_json[i]["backward_saturation"].toInt());
    }


    // Stabilization configuration
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
    // Thrusters configuration
    QJsonObject thruster_json[THRUSTERS_AMOUNT];

    for (uint i = 0; i < THRUSTERS_AMOUNT; ++i) {
        thruster_json[i]["id"] = thrusters_configs[i].id;
        thruster_json[i]["kForward"] = thrusters_configs[i].id;
        thruster_json[i]["kBackward"] = thrusters_configs[i].id;
        thruster_json[i]["forward_saturation"] = thrusters_configs[i].id;
        thruster_json[i]["backward_saturation"] = thrusters_configs[i].id;
    }

    QJsonObject thrusters_json;
    thrusters_json["HLB"] = thruster_json[HLB];
    thrusters_json["HLF"] = thruster_json[HLF];
    thrusters_json["HRB"] = thruster_json[HRB];
    thrusters_json["HRF"] = thruster_json[HRF];
    thrusters_json["VB"] = thruster_json[VB];
    thrusters_json["VF"] = thruster_json[VF];
    thrusters_json["VL"] = thruster_json[VL];
    thrusters_json["VR"] = thruster_json[VR];

    json["thrusters"] = thrusters_json;

    // Stabilization configuration
    QJsonObject cs_json[STABILIZATION_AMOUNT];

    for(int i = 0; i < STABILIZATION_AMOUNT; i++) {
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
