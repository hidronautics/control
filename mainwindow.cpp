#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // send to global view control buttons signals
    connect(ui->pushButtonJetsonOnOff, &QPushButton::toggled,
            this, &MainWindow::send_jetson_btn_signal);
    connect(ui->pushButtonResetIMU, &QPushButton::pressed,
            this, &MainWindow::send_reset_IMU_signal);

    // save to flash thruster configuration
    connect(ui->pushButtonFlash, &QPushButton::pressed,
            this, &MainWindow::send_thruster_flashing_signal);

    connect(ui->checkBoxStabilizeRoll, &QCheckBox::toggled,
            this, &MainWindow::send_stabilize_roll_signal);
    connect(ui->checkBoxStabilizePitch, &QCheckBox::toggled,
            this, &MainWindow::send_stabilize_pitch_signal);
    connect(ui->checkBoxStabilizeYaw, &QCheckBox::toggled,
            this, &MainWindow::send_stabilize_yaw_signal);
    connect(ui->checkBoxStabilizeDepth, &QCheckBox::toggled,
            this, &MainWindow::send_stabilize_depth_signal);

    // send start qualification address
    connect(ui->pushButtonStartQualification, &QPushButton::pressed,
            this, &MainWindow::send_start_qualification_signal);

    ui->tableWidgetRequest->setColumnCount(3);
    ui->tableWidgetRequest->setRowCount(REQUEST_TABLE_ROW_COUNT);

    QAbstractItemModel *model_request = ui->tableWidgetRequest->model();
    QAbstractItemModel *model_response = ui->tableWidgetResponse->model();
    QStringList labels_request, labels_response;
    QStringList headerLabels;
    headerLabels << QString("HEX") << QString("DEX") << QString("Binary");

    labels_response <<  "0:roll"<<
                        "1:"<<
                        "2:pitch"<<
                        "3:"<<
                        "4:yaw"<<
                        "5:"<<
                        "6:roll_speed"<<
                        "7:"<<
                        "8:pitch_speed"<<
                        "9:"<<
                        "10:yaw_speed"<<
                        "11:"<<
                        "12:pressure"<<
                        "13:"<<
                        "14:Bluetooth"<<
                        "15:"<<
                        "16:"<<
                        "17:"<<
                        "18:"<<
                        "19:"<<
                        "20:"<<
                        "21:"<<
                        "22:current_HLB"<<
                        "23:"<<
                        "24:current_HLF"<<
                        "25:"<<
                        "26:current_HRB"<<
                        "27:"<<
                        "28:current_HRF"<<
                        "29:"<<
                        "30:current_VB"<<
                        "31:"<<
                        "32:current_VF"<<
                        "33:"<<
                        "34:current_VL"<<
                        "35:"<<
                        "36:current_VR"<<
                        "37:"<<
                        "38:velocity_HLB"<<
                        "39:velocity_HLF"<<
                        "40:velocity_HRB"<<
                        "41:velocity_HRF"<<
                        "42:velocity_VB"<<
                        "43:velocity_VF"<<
                        "44:velocity_VL"<<
                        "45:velocity_VR"<<
                        "46:current_light"<<
                        "47:"<<
                        "48:current_bottom_light"<<
                        "49:"<<
                        "50:current_arar"<<
                        "51:"<<
                        "52:current_grab"<<
                        "53:"<<
                        "54:current_grab_rotate"<<
                        "55:"<<
                        "56:current_tilt"<<
                        "57:"<<
                        "58:"<<
                        "59:"<<
                        "60:"<<
                        "61:checksum"<<
                        "62:";

        labels_request << "0:code"<<
                          "1:"<<
                          "2:march"<<
                          "3:"<<
                          "4:lag"<<
                          "5:"<<
                          "6:depth"<<
                          "7:"<<
                          "8:roll"<<
                          "9:"<<
                          "10:pitch"<<
                          "11:"<<
                          "12:yaw"<<
                          "13:"<<
                          "14:light"<<
                          "15:grab"<<
                          "16:tilt"<<
                          "17:rotate"<<
                          "18:bt"<<
                          "19:bottom_light"<<
                          "20:stabilize_depth"<<
                          "21:stabilize_roll"<<
                          "22:stabilize_pitch"<<
                          "23:stabilize_yaw"<<
                          "24:reset_IMU"<<
                          "25:checksum"<<
                          "26:";


    for (int i = 0; i < REQUEST_TABLE_ROW_COUNT; ++i) {
        requestQTableWidgetItemsHEX[i] = new QTableWidgetItem(tr("no msg yet"));
        requestQTableWidgetItemsHEX[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetRequest->setItem(i, 0, requestQTableWidgetItemsHEX[i]);

        QVariant data = model_request->headerData(i, Qt::Vertical);

        requestQTableWidgetItemsDEC[i] = new QTableWidgetItem(tr("no msg yet"));
        requestQTableWidgetItemsDEC[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetRequest->setItem(i, 1, requestQTableWidgetItemsDEC[i]);

        requestQTableWidgetItemsBinary[i] = new QTableWidgetItem(tr("no msg yet"));
        requestQTableWidgetItemsBinary[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetRequest->setItem(i, 2, requestQTableWidgetItemsBinary[i]);
    }

    ui->tableWidgetRequest->setVerticalHeaderLabels(labels_request);
    ui->tableWidgetRequest->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidgetRequest->setColumnWidth(0,35);
    ui->tableWidgetRequest->setColumnWidth(1,35);
    ui->tableWidgetRequest->setColumnWidth(2,80);

    ui->tableWidgetResponse->setColumnCount(3);
    ui->tableWidgetResponse->setRowCount(RESPONSE_TABLE_ROW_COUNT);
    ui->tableWidgetResponse->setColumnWidth(0,35);
    ui->tableWidgetResponse->setColumnWidth(1,35);
    ui->tableWidgetResponse->setColumnWidth(2,80);


    for (int i = 0; i < RESPONSE_TABLE_ROW_COUNT; ++i) {
        responseQTableWidgetItemsHEX[i] = new QTableWidgetItem(tr("no msg yet"));
        responseQTableWidgetItemsHEX[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetResponse->setItem(i, 0, responseQTableWidgetItemsHEX[i]);

        QVariant data = model_response->headerData(i, Qt::Vertical);
        //labels_response << QString("%1").arg(data.toInt() - 1); ЧИСЛА ВМЕСТО НАЗВАНИЙ ПЕРЕМЕННЫХ

        responseQTableWidgetItemsDEC[i] = new QTableWidgetItem(tr("no msg yet"));
        responseQTableWidgetItemsDEC[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetResponse->setItem(i, 1, responseQTableWidgetItemsDEC[i]);

        responseQTableWidgetItemsBinary[i] = new QTableWidgetItem(tr("no msg yet"));
        responseQTableWidgetItemsBinary[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetResponse->setItem(i, 2, responseQTableWidgetItemsBinary[i]);
    }

    ui->tableWidgetResponse->setVerticalHeaderLabels(labels_response);
    ui->tableWidgetResponse->setHorizontalHeaderLabels(headerLabels);

    K_Protocol = new Qkx_coeffs("protocols.conf", "ki");
    X_Protocol = new x_protocol("protocols.conf", "xi", X);

    // Direct tab
    btn_gr_thruster_numb = new QButtonGroup(this);
    btn_gr_thruster_numb->addButton(ui->pushButtonHLB, 0);
    btn_gr_thruster_numb->addButton(ui->pushButtonHLF, 1);
    btn_gr_thruster_numb->addButton(ui->pushButtonHRB, 2);
    btn_gr_thruster_numb->addButton(ui->pushButtonHRF, 3);
    btn_gr_thruster_numb->addButton(ui->pushButtonVB, 4);
    btn_gr_thruster_numb->addButton(ui->pushButtonVF, 5);
    btn_gr_thruster_numb->addButton(ui->pushButtonVL, 6);
    btn_gr_thruster_numb->addButton(ui->pushButtonVR, 7);

    connect(btn_gr_thruster_numb,
            static_cast<void(QButtonGroup::*)(int)>(
                &QButtonGroup::buttonClicked), this,
            &MainWindow::thruster_number_changed);

    connect(ui->spinBoxThrusterId,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::thruster_id_changed);
    connect(ui->checkBoxReverse, &QCheckBox::clicked,
            this, &MainWindow::thruster_reverse_clicked);

    connect(ui->verticalSliderVelocity, &QSlider::valueChanged,
            this, &MainWindow::thruster_velocity_changed);
    connect(ui->verticalSliderForwardK, &QSlider::valueChanged,
            this, &MainWindow::thruster_k_forward_changed);
    connect(ui->verticalSliderBackwardK, &QSlider::valueChanged,
            this, &MainWindow::thruster_k_backward_changed);
    connect(ui->verticalSliderForwardSaturation, &QSlider::valueChanged,
            this, &MainWindow::thruster_forward_saturation_changed);
    connect(ui->verticalSliderBarkwardSaturation, &QSlider::valueChanged,
            this, &MainWindow::thruster_backward_saturation_changed);
}


void MainWindow::send_start_qualification_signal()
{
    emit start_qualification_btn_clicked();
}


void MainWindow::thruster_number_changed(int number)
{
    settings->current_thrusters_numb = static_cast<uint8_t>(number);

    ui->spinBoxThrusterId->setValue(settings->thrusters_configs[settings->current_thrusters_numb].id);
    ui->checkBoxReverse->setChecked(settings->thrusters_configs[settings->current_thrusters_numb].reverse);
    ui->verticalSliderVelocity->setValue(settings->thrusters_configs[settings->current_thrusters_numb].velocity);
    ui->verticalSliderForwardK->setValue(static_cast<int>(settings->thrusters_configs[settings->current_thrusters_numb].kForward*ui->verticalSliderForwardK->maximum()));
    ui->verticalSliderBackwardK->setValue(static_cast<int>(settings->thrusters_configs[settings->current_thrusters_numb].kBackward*ui->verticalSliderBackwardK->maximum()));
    ui->verticalSliderForwardSaturation->setValue(settings->thrusters_configs[settings->current_thrusters_numb].forward_saturation);
    ui->verticalSliderBarkwardSaturation->setValue(settings->thrusters_configs[settings->current_thrusters_numb].backward_saturation);
}


void MainWindow::thruster_id_changed(int id)
{
    settings->thrusters_configs[settings->current_thrusters_numb].id = static_cast<uint8_t>(id);
}


void MainWindow::thruster_reverse_clicked(bool checked)
{
    settings->thrusters_configs[settings->current_thrusters_numb].reverse = checked;
}


void MainWindow::thruster_velocity_changed(int value)
{
    settings->thrusters_configs[settings->current_thrusters_numb].velocity = static_cast<int8_t>(value);
    ui->spinBoxVelocity->setValue(value);
}


void MainWindow::thruster_k_forward_changed(int value)
{
    float f_val = static_cast<double>(value) / ui->verticalSliderForwardK->maximum();
    settings->thrusters_configs[settings->current_thrusters_numb].kForward = f_val;
    ui->spinBoxForwardK->setValue(f_val);
}


void MainWindow::thruster_k_backward_changed(int value)
{
    float f_val = static_cast<double>(value) / ui->verticalSliderBackwardK->maximum();
    settings->thrusters_configs[settings->current_thrusters_numb].kBackward = f_val;
    ui->spinBoxBackwardK->setValue(f_val);
}


void MainWindow::thruster_forward_saturation_changed(int value)
{
    settings->thrusters_configs[settings->current_thrusters_numb].forward_saturation = static_cast<int8_t>(value);
    ui->spinBoxForwardSaturation->setValue(value);
}


void MainWindow::thruster_backward_saturation_changed(int value)
{
    settings->thrusters_configs[settings->current_thrusters_numb].backward_saturation = static_cast<int8_t>(value);
    ui->spinBoxBarkwardSaturation->setValue(value);
}


void MainWindow::send_thruster_flashing_signal()
{
    emit flash_thruster_btn_clicked();
}


void MainWindow::send_jetson_btn_signal(bool checked)
{
    emit jetson_on_off_btn_toggled(checked);
}


void MainWindow::send_reset_IMU_signal()
{
    emit reset_IMU_btn_clicked();
    std::cout << "RESET IMU";
}


void MainWindow::send_stabilize_roll_signal(bool checked)
{
    emit  stabilize_roll_toggled(checked);
}


void MainWindow::send_stabilize_pitch_signal(bool checked)
{
    emit stabilize_pitch_toggled(checked);
}


void MainWindow::send_stabilize_yaw_signal(bool checked)
{
    emit stabilize_yaw_toggled(checked);
}


void MainWindow::send_stabilize_depth_signal(bool checked)
{
    emit stabilize_depth_toggled(checked);
}


void MainWindow::serverIsSleeping() {
    std::cout << "Server is sleeping, putting it's data to gui..." << std::endl;
    int request_length = 0;

    ui->label_current_message_type->setText(QString::number(server->currentMessageType, 16));
    ui->label_next_message_type->setText(QString::number(server->nextMessageType, 16));

    switch (server->currentMessageType) {
    case REQUEST_NORMAL_CODE:
        request_length = REQUEST_NORMAL_LENGTH;
        break;
    case REQUEST_CONFIG_CODE:
        request_length = REQUEST_CONFIG_LENGTH;
        break;
    case REQUEST_DIRECT_CODE:
        request_length = REQUEST_DIRECT_LENGTH;
        break;
    default:
        std::cout << "Unknown current message code!" << std::endl;
    }

    for (int i = 0; i < request_length; ++i) {
        //requestQTableWidgetItems[i]->setText(QString("%1").arg(server->msg_to_send[i]));
        requestQTableWidgetItemsHEX[i]->setText(QString::number(server->msg_to_send.data()[i], 16));
        requestQTableWidgetItemsDEC[i]->setText(QString::number(server->msg_to_send.data()[i], 10));
        requestQTableWidgetItemsBinary[i]->setText(QString::number(server->msg_to_send.data()[i], 2));
    }

    for (int i = request_length; i < REQUEST_TABLE_ROW_COUNT; ++i) {
        requestQTableWidgetItemsHEX[i]->setText(QString("NO"));
        requestQTableWidgetItemsDEC[i]->setText(QString("NO"));
        requestQTableWidgetItemsBinary[i]->setText(QString("NO"));
    }

    for (int i = 0; i < RESPONSE_LENGTH; ++i) {
        responseQTableWidgetItemsHEX[i]->setText(QString::number(server->msg_in.data()[i], 16));
        responseQTableWidgetItemsDEC[i]->setText(QString::number(server->msg_in.data()[i], 10));
        responseQTableWidgetItemsBinary[i]->setText(QString::number(server->msg_in.data()[i], 2));
    }

    for (int i = RESPONSE_LENGTH; i < RESPONSE_TABLE_ROW_COUNT; ++i) {
        responseQTableWidgetItemsHEX[i]->setText(QString("NO"));
        responseQTableWidgetItemsDEC[i]->setText(QString("NO"));
        responseQTableWidgetItemsBinary[i]->setText(QString("NO"));
    }

    std::cout << "STABILIZE DEPTH = " << joystick->stabilize_depth << std::endl;
    std::cout << "STABILIZE ROLL  = " << joystick->stabilize_roll  << std::endl;
    std::cout << "STABILIZE PITCH = " << joystick->stabilize_yaw << std::endl;
}


void MainWindow::info(QString s)
{
 //   ui->terminal->append(s);
    ui->statusbar->showMessage(s);
}


void MainWindow::init()
{
    //on_pushButton_CS_loadConfig_released(); // Зло во плоти (в коде)

    //settings->connection->num = ui->spinBox_COM->value();
    //settings->connection->baudRate = settings->connection->getBaudRate(ui->comboBox_BaudRate->currentText().toInt());
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_checkBox_Calibration_Control_toggled(bool checked)
{
    //emit changeControl(checked);
}


void MainWindow::on_pushButton_Connect_released()
{
    emit tryConnect();
}


void MainWindow::on_pushButton_Disconnect_released()
{
    emit disconnect();
}


void MainWindow::on_pushButton_CS_loadConfig_released()
{
    if (settings->loadFromJSONFile()) {
        updateCsView();

        ui->spinBox_COM->setValue(settings->connection->num);
        std::cout << QString::number(settings->connection->setBaudRate(settings->connection->baudRate)).toStdString() << std::endl;
        std::cout << QString::number(settings->connection->setDataBits(settings->connection->dataBits)).toStdString() << std::endl;
        ui->comboBox_BaudRate->setCurrentText(QString::number(settings->connection->setBaudRate(settings->connection->baudRate)));
        ui->comboBox_DataBits->setCurrentText(QString::number(settings->connection->setDataBits(settings->connection->dataBits)));
        ui->comboBox_Parity->setCurrentText(settings->connection->setParity(settings->connection->parity));
        ui->comboBox_StopBits->setCurrentText(settings->connection->setStopBits(settings->connection->stopBits));
        ui->comboBox_FlowControl->setCurrentText(settings->connection->setFlowControl(settings->connection->flowControl));

        ui->spinBox_PauseAfterSent->setValue(settings->connection->pause_after_sent);
        ui->spinBox_PauseAfterReceived->setValue(settings->connection->pause_after_received);

        ui->statusbar->showMessage("Settings successfully loaded", 5000);
    } else {
        ui->statusbar->showMessage("Cannot load settings. Invalid file or file does not exist.", 5000);
    }
}


void MainWindow::on_pushButton_send_config_released()
{
    server->nextMessageType = REQUEST_CONFIG_CODE;
}


void MainWindow::on_pushButton_released()
{
    ui->checkBox->setChecked(true);
    emit connect_fake();
}


void MainWindow::on_pushButton_2_released()
{
    server->nextMessageType = REQUEST_DIRECT_CODE;
}


void MainWindow::on_pushButton_3_released()
{
    server->nextMessageType = REQUEST_NORMAL_CODE;
}


void MainWindow::on_pushButton_CS_saveConfig_released()
{
    saveCsView();

    settings->connection->pause_after_sent = ui->spinBox_PauseAfterSent->value();
    settings->connection->pause_after_received = ui->spinBox_PauseAfterReceived->value();

    settings->saveToJSONFIle();
}


void MainWindow::on_pushButton_4_released()
{
    settings->loadFromJSONFile();
}


void MainWindow::on_radioButton_Joystick_released()
{
    joystick->isControlJoystick = true;
    joystick->isControlKeyboard = false;
    joystick->isControlXbox = false;

    ui->radioButton_Joystick->setChecked(true);
    ui->radioButton_Keyboard->setChecked(false);
    ui->radioButton_XBox->setChecked(false);
}


void MainWindow::on_radioButton_Keyboard_released()
{
    joystick->isControlJoystick = false;
    joystick->isControlKeyboard = true;
    joystick->isControlXbox = false;

    ui->radioButton_Joystick->setChecked(false);
    ui->radioButton_Keyboard->setChecked(true);
    ui->radioButton_XBox->setChecked(false);
}


void MainWindow::on_radioButton_XBox_released()
{
    joystick->isControlJoystick = false;
    joystick->isControlKeyboard = false;
    joystick->isControlXbox = true;

    ui->radioButton_Joystick->setChecked(false);
    ui->radioButton_Keyboard->setChecked(false);
    ui->radioButton_XBox->setChecked(true);
}


void MainWindow::on_comboBox_Parity_currentTextChanged(const QString &arg1)
{
    settings->connection->parity = settings->connection->getParity(arg1);
}


void MainWindow::on_comboBox_StopBits_currentTextChanged(const QString &arg1)
{
    settings->connection->stopBits = settings->connection->getStopBits(arg1);
}


void MainWindow::on_comboBox_FlowControl_currentTextChanged(const QString &arg1)
{
    settings->connection->flowControl = settings->connection->getFlowControl(arg1);
}


void MainWindow::on_spinBox_COM_valueChanged(int arg1)
{
    settings->connection->num = arg1;
}


void MainWindow::on_comboBox_DataBits_currentTextChanged(const QString &arg1)
{
    settings->connection->dataBits = settings->connection->getDataBits(arg1.toInt());
}


void MainWindow::on_comboBox_BaudRate_currentTextChanged(const QString &arg1)
{
    settings->connection->baudRate = settings->connection->getBaudRate(arg1.toInt());
}


void MainWindow::on_checkBox_toggled(bool checked)
{
    server->emulation_mode = checked;
}


void MainWindow::on_spinBox_PauseAfterSent_valueChanged(int arg1)
{
    settings->connection->pause_after_sent = arg1;
}


void MainWindow::on_spinBox_PauseAfterReceived_valueChanged(int arg1)
{
    settings->connection->pause_after_received = arg1;
}


void MainWindow::on_checkBox_2_toggled(bool checked)
{
    joystick->inverseDepth = checked;
}

void MainWindow::on_checkBox_SDepth_toggled(bool checked)
{
    joystick->stabilize_depth = checked;
}

void MainWindow::on_checkBox_SRoll_toggled(bool checked)
{
    joystick->stabilize_roll = checked;
}

void MainWindow::on_checkBox_SYaw_toggled(bool checked)
{
    joystick->stabilize_yaw = checked;
}

void MainWindow::on_radioButton_CS_YawSelect_released()
{
    settings->CS = STAB_YAW;
    updateCsView();
}

void MainWindow::on_radioButton_CS_DepthSelect_released()
{
    settings->CS = STAB_DEPTH;
    updateCsView();
}

void MainWindow::on_radioButton_CS_RollSelect_released()
{
    settings->CS = STAB_ROLL;
    updateCsView();
}

void MainWindow::on_radioButton_CS_PitchSelect_released()
{
    settings->CS = STAB_PITCH;
    updateCsView();
}

// Stopwatch
void MainWindow::on_StartPauseButton_clicked()
{
    if (startPauseBtn->text() == "Start"){
        startPauseBtn->setText("Pause");
        time->start();
        timer->start(1000);
    }else {
        startPauseBtn->setText("Start");
        bufferTimeMission += time->elapsed();
        time = new QTime();
    }
}

void MainWindow::on_ClearButton_clicked()
{
    time = new QTime();
    bufferTimeMain = 0;
    bufferTimeMission = 0;
    startPauseBtn->setText("Start");
    missionNumber->setText("1");
}

void MainWindow::on_NextButton_clicked()
{
    if(missionNumber->text() == "3"){
        on_ClearButton_clicked();
    }else{
        missionNumber->setText(QString::number(missionNumber->text().toInt()+1));
        bufferTimeMain += bufferTimeMission + time->elapsed();
        bufferTimeMission = 0;
        time = new QTime();
    }
    startPauseBtn->setText("Start");
}

QString MainWindow::getWatchTime(int time){
    QString result = "";
    time /= 1000;
    QString seconds = QString::number(time % 60);
    QString minutes = QString::number(time / 60);
    result = (minutes.toInt() > 9 ? minutes : "0" + minutes) + ":" + (seconds.toInt() > 9 ? seconds : "0" + seconds);
    return result;
}

void MainWindow::updateTime(){
    x = time->elapsed();
    x += bufferTimeMission;
    currentMissionTime->setText(getWatchTime(x));
    x += bufferTimeMain;
    commonTime->setText(getWatchTime(x));
}

void MainWindow::updateCsView()
{
    ui->doubleSpinBox_CS_pJoyUnitCast->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pJoyUnitCast));
    ui->doubleSpinBox_CS_pSpeedDyn->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pSpeedDyn));
    ui->doubleSpinBox_CS_pErrGain->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pErrGain));
    ui->doubleSpinBox_CS_pid_pGain->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pid.pGain));
    ui->doubleSpinBox_CS_pid_iGain->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pid.iGain));
    ui->doubleSpinBox_CS_pid_iMax->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pid.iMax));
    ui->doubleSpinBox_CS_pid_iMin->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pid.iMin));
    ui->doubleSpinBox_CS_pThrustersCast->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pThrustersCast));
    ui->doubleSpinBox_CS_pThrustersMin->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pThrustersMin));
    ui->doubleSpinBox_CS_pThrustersMax->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.pThrustersMax));

    ui->doubleSpinBox_CS_posFilterK->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.aFilter[POS_FILTER].K));
    ui->doubleSpinBox_CS_posFilterT->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.aFilter[POS_FILTER].T));
    ui->doubleSpinBox_CS_speedFilterK->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.aFilter[SPEED_FILTER].K));
    ui->doubleSpinBox_CS_speedFilterT->setValue(static_cast<double>(settings->stabContour[settings->CS].stabConstants.aFilter[SPEED_FILTER].T));
}

void MainWindow::saveCsView()
{
    settings->stabContour[settings->CS].stabConstants.pJoyUnitCast = static_cast<float>(ui->doubleSpinBox_CS_pJoyUnitCast->value());
    settings->stabContour[settings->CS].stabConstants.pSpeedDyn = static_cast<float>(ui->doubleSpinBox_CS_pSpeedDyn->value());
    settings->stabContour[settings->CS].stabConstants.pErrGain = static_cast<float>(ui->doubleSpinBox_CS_pErrGain->value());
    settings->stabContour[settings->CS].stabConstants.pid.pGain = static_cast<float>(ui->doubleSpinBox_CS_pid_pGain->value());
    settings->stabContour[settings->CS].stabConstants.pid.iGain = static_cast<float>(ui->doubleSpinBox_CS_pid_iGain->value());
    settings->stabContour[settings->CS].stabConstants.pid.iMax = static_cast<float>(ui->doubleSpinBox_CS_pid_iMax->value());
    settings->stabContour[settings->CS].stabConstants.pid.iMin = static_cast<float>(ui->doubleSpinBox_CS_pid_iMin->value());
    settings->stabContour[settings->CS].stabConstants.pThrustersCast = static_cast<float>(ui->doubleSpinBox_CS_pThrustersCast->value());
    settings->stabContour[settings->CS].stabConstants.pThrustersMin = static_cast<float>(ui->doubleSpinBox_CS_pThrustersMin->value());
    settings->stabContour[settings->CS].stabConstants.pThrustersMax = static_cast<float>(ui->doubleSpinBox_CS_pThrustersMax->value());

    settings->stabContour[settings->CS].stabConstants.aFilter[POS_FILTER].K = static_cast<float>(ui->doubleSpinBox_CS_posFilterK->value());
    settings->stabContour[settings->CS].stabConstants.aFilter[POS_FILTER].T = static_cast<float>(ui->doubleSpinBox_CS_posFilterT->value());
    settings->stabContour[settings->CS].stabConstants.aFilter[SPEED_FILTER].K = static_cast<float>(ui->doubleSpinBox_CS_speedFilterK->value());
    settings->stabContour[settings->CS].stabConstants.aFilter[SPEED_FILTER].T = static_cast<float>(ui->doubleSpinBox_CS_speedFilterT->value());

    settings->controlMultiplier = static_cast<float>(ui->doubleSpinBox_sensitivity->value());
}

void MainWindow::updateCsLabels()
{
    ui->lineEdit_CS_inputSignal->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.inputSignal), 'f', 2));
    ui->lineEdit_CS_joyUnitCasted->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.joyUnitCasted), 'f', 2));
    ui->lineEdit_CS_joy_iValue->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.joy_iValue), 'f', 2));
    ui->lineEdit_CS_posError->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.posError), 'f', 2));
    ui->lineEdit_CS_posSignal->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.posSignal), 'f', 2));
    ui->lineEdit_CS_posFiltered->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.posFiltered), 'f', 2));
    ui->lineEdit_CS_pidValue->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.pidValue), 'f', 2));
    ui->lineEdit_CS_dynSummator->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.dynSummator), 'f', 2));
    ui->lineEdit_CS_speedError->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.speedError), 'f', 2));
    ui->lineEdit_CS_speedSignal->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.speedSignal), 'f', 2));
    ui->lineEdit_CS_speedFiltered->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.speedFiltered), 'f', 2));
    ui->lineEdit_CS_pid_iValue->setText(QString::number(static_cast<double>(settings->stabContour[settings->CS].stabState.pid_iValue), 'f', 2));
}

void MainWindow::update_csView()
{
    updateCsLabels();

    X[0][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.inputSignal);
    X[1][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.speedSignal);
    X[2][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.posSignal);

    X[3][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.oldSpeed);
    X[4][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.oldPos);

    X[5][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.joyUnitCasted);
    X[6][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.joy_iValue);
    X[7][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.posError);
    X[8][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.speedError);
    X[9][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.dynSummator);
    X[10][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.pidValue);
    X[11][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.posErrorAmp);
    X[12][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.speedFiltered);
    X[13][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.posFiltered);

    X[14][0] = static_cast<double>(settings->stabContour[settings->CS].stabState.pid_iValue);
    X[15][0] = static_cast<double>(server->imu_raw_yaw);
}

