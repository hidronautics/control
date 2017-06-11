#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //this->setStyleSheet("background-color: black;");

    ui->tableWidgetRequest->setColumnCount(2);
    ui->tableWidgetRequest->setRowCount(REQUEST_TABLE_ROW_COUNT);

    QAbstractItemModel *model_request = ui->tableWidgetRequest->model();
    QAbstractItemModel *model_response = ui->tableWidgetResponse->model();
    QStringList labels_request, labels_response;
    QStringList headerLabels;
    headerLabels << QString("Values");



    labels_response <<   "0:roll"<<
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


        //labels_request << QString("%1").arg(data.toInt() - 1); ЧИСЛА ВМЕСТО НАЗВАНИЙ ПЕРЕМЕННЫХ


        requestQTableWidgetItemsDEC[i] = new QTableWidgetItem(tr("no msg yet"));
        requestQTableWidgetItemsDEC[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetRequest->setItem(i, 1, requestQTableWidgetItemsDEC[i]);
    }

    ui->tableWidgetRequest->setVerticalHeaderLabels(labels_request);
    ui->tableWidgetRequest->setHorizontalHeaderLabels(headerLabels);


    ui->tableWidgetResponse->setColumnCount(2);
    ui->tableWidgetResponse->setRowCount(RESPONSE_TABLE_ROW_COUNT);

    for (int i = 0; i < RESPONSE_TABLE_ROW_COUNT; ++i) {
        responseQTableWidgetItemsHEX[i] = new QTableWidgetItem(tr("no msg yet"));
        responseQTableWidgetItemsHEX[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetResponse->setItem(i, 0, responseQTableWidgetItemsHEX[i]);

        QVariant data = model_response->headerData(i, Qt::Vertical);
        //labels_response << QString("%1").arg(data.toInt() - 1); ЧИСЛА ВМЕСТО НАЗВАНИЙ ПЕРЕМЕННЫХ

        responseQTableWidgetItemsDEC[i] = new QTableWidgetItem(tr("no msg yet"));
        responseQTableWidgetItemsDEC[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetResponse->setItem(i, 1, responseQTableWidgetItemsDEC[i]);
    }

    ui->tableWidgetResponse->setVerticalHeaderLabels(labels_response);
    ui->tableWidgetResponse->setHorizontalHeaderLabels(headerLabels);

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
        requestQTableWidgetItemsHEX[i]->setText(QString::number(server->msg_to_send[i], 16));
        requestQTableWidgetItemsDEC[i]->setText(QString::number(server->msg_to_send[i], 10));
    }

    for (int i = request_length; i < REQUEST_TABLE_ROW_COUNT; ++i) {
        requestQTableWidgetItemsHEX[i]->setText(QString("not presented"));
        requestQTableWidgetItemsDEC[i]->setText(QString("not presented"));
    }

    for (int i = 0; i < RESPONSE_LENGTH; ++i) {
        responseQTableWidgetItemsHEX[i]->setText(QString::number(server->msg_in[i], 16));
        responseQTableWidgetItemsDEC[i]->setText(QString::number(server->msg_in[i], 10));
    }

    for (int i = RESPONSE_LENGTH; i < RESPONSE_TABLE_ROW_COUNT; ++i) {
        responseQTableWidgetItemsHEX[i]->setText(QString("not presented"));
        responseQTableWidgetItemsDEC[i]->setText(QString("not presented"));
    }

    ui->graphicsPFD_2->setRoll(server->imu_roll);
    ui->graphicsPFD_2->setPitch(server->imu_pitch);
    ui->graphicsPFD_2->setTurnRate(server->imu_roll_speed);
    ui->graphicsPFD_2->update();
}

void MainWindow::info(QString s) {
 //   ui->terminal->append(s);
    ui->statusbar->showMessage(s);
}

void MainWindow::init() {
    on_spinBox_Motor_Slot_valueChanged(0);
    on_pushButton_load_config_released(); // Зло во плоти (в коде)

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

void MainWindow::on_pushButton_Test_Motor_released()
{
    for (int i = 0; i < 8; ++i)
        settings->motors[i].speed = 0;
    int currentSlot = ui->spinBox_Motor_Slot->text().toInt();
    int currentSpeed = ui->spinBox_Motor_Speed->text().toInt();
    settings->motors[currentSlot].speed = currentSpeed;
}


void MainWindow::on_pushButton_Connect_released()
{
    emit tryConnect();
}


void MainWindow::on_pushButton_Stop_Test_released()
{
    for (int i = 0; i < 8; ++i)
        settings->motors[i].speed = 0;
}

void MainWindow::on_pushButton_Save_released()
{
    int code = ui->spinBox_Motor_Slot->value();
    settings->motors[code].enabled = ui->checkBox_Enabled->isChecked();
    settings->motors[code].inverse = ui->checkBox_Inverse->isChecked();
    settings->motors[code].k_backward = ui->doubleSpinBox_back->value();
    settings->motors[code].k_forward = ui->doubleSpinBox_forw->value();
    settings->motors[code].setCode(ui->comboBox_Tested_Motor->currentText());

    // TODO Refactor method
    /*emit changeMotorSetting(ui->spinBox_Motor_Slot->value(),
                            ui->comboBox_Tested_Motor->currentText(),
                            ui->checkBox_Inverse->isChecked());*/
}

void MainWindow::on_spinBox_Motor_Slot_valueChanged(int code)
{
    ui->checkBox_Enabled->setChecked(settings->motors[code].enabled);
    ui->checkBox_Inverse->setChecked(settings->motors[code].inverse);

    ui->doubleSpinBox_back->setValue(settings->motors[code].k_backward);
    ui->doubleSpinBox_forw->setValue(settings->motors[code].k_forward);

    ui->comboBox_Tested_Motor->setCurrentText(settings->motors[code].getCode());
}

void MainWindow::on_pushButton_Disconnect_released()
{
    emit disconnect();
}

void MainWindow::on_pushButton_load_config_released()
{
    if (settings->loadFromJSONFile()) {
        ui->doubleSpinBox_depth_k1->setValue(settings->depth.k1);
        ui->doubleSpinBox_depth_k2->setValue(settings->depth.k2);
        ui->doubleSpinBox_depth_start->setValue(settings->depth.start);
        ui->doubleSpinBox_depth_gain->setValue(settings->depth.gain);
        ui->checkBox_const_time_depth->setChecked(settings->depth.const_time);

        ui->doubleSpinBox_roll_k1->setValue(settings->roll.k1);
        ui->doubleSpinBox_roll_k2->setValue(settings->roll.k2);
        ui->doubleSpinBox_roll_start->setValue(settings->roll.start);
        ui->doubleSpinBox_roll_gain->setValue(settings->roll.gain);
        ui->checkBox_const_time_roll->setChecked(settings->roll.const_time);

        ui->doubleSpinBox_pitch_k1->setValue(settings->pitch.k1);
        ui->doubleSpinBox_pitch_k2->setValue(settings->pitch.k2);
        ui->doubleSpinBox_pitch_start->setValue(settings->pitch.start);
        ui->doubleSpinBox_pitch_gain->setValue(settings->pitch.gain);
        ui->checkBox_const_time_pitch->setChecked(settings->pitch.const_time);

        ui->doubleSpinBox_yaw_k1->setValue(settings->yaw.k1);
        ui->doubleSpinBox_yaw_k2->setValue(settings->yaw.k2);
        ui->doubleSpinBox_yaw_start->setValue(settings->yaw.start);
        ui->doubleSpinBox_yaw_gain->setValue(settings->yaw.gain);
        ui->checkBox_const_time_yaw->setChecked(settings->yaw.const_time);

        ui->spinBox_COM->setValue(settings->connection->num);
        std::cout << QString::number(settings->connection->setBaudRate(settings->connection->baudRate)).toStdString() << std::endl;
        std::cout << QString::number(settings->connection->setDataBits(settings->connection->dataBits)).toStdString() << std::endl;
        ui->comboBox_BaudRate->setCurrentText(QString::number(settings->connection->setBaudRate(settings->connection->baudRate)));
        ui->comboBox_DataBits->setCurrentText(QString::number(settings->connection->setDataBits(settings->connection->dataBits)));
        ui->comboBox_Parity->setCurrentText(settings->connection->setParity(settings->connection->parity));
        ui->comboBox_StopBits->setCurrentText(settings->connection->setStopBits(settings->connection->stopBits));
        ui->comboBox_FlowControl->setCurrentText(settings->connection->setFlowControl(settings->connection->flowControl));

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

void MainWindow::on_pushButton_save_config_released()
{
    settings->depth.k1 = ui->doubleSpinBox_depth_k1->value();
    settings->depth.k2 = ui->doubleSpinBox_depth_k2->value();
    settings->depth.gain = ui->doubleSpinBox_depth_gain->value();
    settings->depth.start = ui->doubleSpinBox_depth_start->value();
    settings->depth.const_time = ui->checkBox_const_time_depth->isChecked();

    settings->roll.k1 = ui->doubleSpinBox_roll_k1->value();
    settings->roll.k2 = ui->doubleSpinBox_roll_k2->value();
    settings->roll.gain = ui->doubleSpinBox_roll_gain->value();
    settings->roll.start = ui->doubleSpinBox_roll_start->value();
    settings->roll.const_time = ui->checkBox_const_time_roll->isChecked();

    settings->pitch.k1 = ui->doubleSpinBox_pitch_k1->value();
    settings->pitch.k2 = ui->doubleSpinBox_pitch_k2->value();
    settings->pitch.gain = ui->doubleSpinBox_pitch_gain->value();
    settings->pitch.start = ui->doubleSpinBox_pitch_start->value();
    settings->pitch.const_time = ui->checkBox_const_time_pitch->isChecked();

    settings->yaw.k1 = ui->doubleSpinBox_yaw_k1->value();
    settings->yaw.k2 = ui->doubleSpinBox_yaw_k2->value();
    settings->yaw.gain = ui->doubleSpinBox_yaw_gain->value();
    settings->yaw.start = ui->doubleSpinBox_yaw_start->value();
    settings->yaw.const_time = ui->checkBox_const_time_yaw->isChecked();

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

