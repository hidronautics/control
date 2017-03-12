#include "mainwindow.h"
#include "ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix_back(":images/images/background.png");
    ui->label_pic->setScaledContents(true);
    ui->label_pic->setPixmap(pix_back);

    //Motors
    QPixmap pix_mot(":images/images/Motors.png");
    ui->label_motors->setScaledContents(true);
    ui->label_motors->setPixmap(pix_mot);

    //Indicator motor
    //motor1
    if (5==5)
    {
        QPixmap pix_green(":images/images/LEDgreen");
        ui->label_indicator1->setScaledContents(true);
        ui->label_indicator1->setPixmap(pix_green);
        ui->textBrowser->setText("Current");
        ui->textBrowser->append("Speed");
        ui->textBrowser->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":images/images/LEDred");
       ui->label_indicator1->setScaledContents(true);
       ui->label_indicator1->setPixmap(pix_red);
       ui->textBrowser->setText("Error");
    }

    //motor2
    if (5>5)
    {
        QPixmap pix_green(":images/images/LEDgreen");
        ui->label_indicator1_2->setScaledContents(true);
        ui->label_indicator1_2->setPixmap(pix_green);
        ui->textBrowser_2->setText("Current");
        ui->textBrowser_2->append("Speed");
        ui->textBrowser_2->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":images/images/LEDred");
       ui->label_indicator1_2->setScaledContents(true);
       ui->label_indicator1_2->setPixmap(pix_red);
       ui->textBrowser_2->setText("Error");
    }

    //motor3
    if (5>5)
    {
        QPixmap pix_green(":images/images/LEDgreen");
        ui->label_indicator1_3->setScaledContents(true);
        ui->label_indicator1_3->setPixmap(pix_green);
        ui->textBrowser_3->setText("Current");
        ui->textBrowser_3->append("Speed");
        ui->textBrowser_3->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":images/images/LEDred");
       ui->label_indicator1_3->setScaledContents(true);
       ui->label_indicator1_3->setPixmap(pix_red);
       ui->textBrowser_3->setText("Error");
    }

    //motor4
    if (5==5)
    {
        QPixmap pix_green(":images/images/LEDgreen");
        ui->label_indicator1_4->setScaledContents(true);
        ui->label_indicator1_4->setPixmap(pix_green);
        ui->textBrowser_4->setText("Current");
        ui->textBrowser_4->append("Speed");
        ui->textBrowser_4->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":images/images/LEDred");
       ui->label_indicator1_4->setScaledContents(true);
       ui->label_indicator1_4->setPixmap(pix_red);
       ui->textBrowser_4->setText("Error");
    }

    //motor5
    if (5==5)
    {
        QPixmap pix_green(":images/images/LEDgreen");
        ui->label_indicator1_5->setScaledContents(true);
        ui->label_indicator1_5->setPixmap(pix_green);
        ui->textBrowser_5->setText("Current");
        ui->textBrowser_5->append("Speed");
        ui->textBrowser_5->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":images/images/LEDred");
       ui->label_indicator1_5->setScaledContents(true);
       ui->label_indicator1_5->setPixmap(pix_red);
       ui->textBrowser_5->setText("Error");
    }

    //motor6
    if (5==5)
    {
        QPixmap pix_green(":images/images/LEDgreen");
        ui->label_indicator1_6->setScaledContents(true);
        ui->label_indicator1_6->setPixmap(pix_green);
        ui->textBrowser_6->setText("Current");
        ui->textBrowser_6->append("Speed");
        ui->textBrowser_6->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":images/images/LEDred");
       ui->label_indicator1_6->setScaledContents(true);
       ui->label_indicator1_6->setPixmap(pix_red);
       ui->textBrowser_6->setText("Error");
    }

    //motor7
    if (5>5)
    {
        QPixmap pix_green(":images/images/LEDgreen");
        ui->label_indicator1_7->setScaledContents(true);
        ui->label_indicator1_7->setPixmap(pix_green);
        ui->textBrowser_7->setText("Current");
        ui->textBrowser_7->append("Speed");
        ui->textBrowser_7->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":images/images/LEDred");
       ui->label_indicator1_7->setScaledContents(true);
       ui->label_indicator1_7->setPixmap(pix_red);
       ui->textBrowser_7->setText("Error");
    }

    //motor8
    if (5==5)
    {
        QPixmap pix_green(":images/images/LEDgreen");
        ui->label_indicator1_8->setScaledContents(true);
        ui->label_indicator1_8->setPixmap(pix_green);
        ui->textBrowser_8->setText("Current");
        ui->textBrowser_8->append("Speed");
        ui->textBrowser_8->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":images/images/LEDred");
       ui->label_indicator1_8->setScaledContents(true);
       ui->label_indicator1_8->setPixmap(pix_red);
       ui->textBrowser_8->setText("Error");
    }


    //Indicator bluetooth
    QPixmap pix_bt(":images/images/BT.png");
    ui->label->setScaledContents(true);
    ui->label->setPixmap(pix_bt);
    ui->textBrowser_Bluetooth->setText("Message");


    if (5==5) //No bluetooth signal
    {
        QPixmap pix_redB(":images/images/LED2");
        ui->label_indicator2->setScaledContents(true);
        ui->label_indicator2->setPixmap(pix_redB);
    }
    else
    {
       QPixmap pix_greenB(":images/images/LED3");
       ui->label_indicator2->setScaledContents(true);
       ui->label_indicator2->setPixmap(pix_greenB);
    }

    connect(ui->pushButton_Reference, SIGNAL(click()), this, SLOT(openNewWindow()));


    ui->tableWidgetRequest->setColumnCount(2);
    ui->tableWidgetRequest->setRowCount(REQUEST_TABLE_ROW_COUNT);

    QAbstractItemModel *model_request = ui->tableWidgetRequest->model();
    QAbstractItemModel *model_response = ui->tableWidgetResponse->model();
    QStringList labels_request, labels_response;
    QStringList headerLabels;
    headerLabels << QString("Values");

    for (int i = 0; i < REQUEST_TABLE_ROW_COUNT; ++i) {
        requestQTableWidgetItemsHEX[i] = new QTableWidgetItem(tr("no msg yet"));
        requestQTableWidgetItemsHEX[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetRequest->setItem(i, 0, requestQTableWidgetItemsHEX[i]);

        QVariant data = model_request->headerData(i, Qt::Vertical);
        labels_request << QString("%1").arg(data.toInt() - 1);


        requestQTableWidgetItemsDEC[i] = new QTableWidgetItem(tr("no msg yet"));
        requestQTableWidgetItemsDEC[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetRequest->setItem(i, 1, requestQTableWidgetItemsDEC[i]);
    }

    ui->tableWidgetRequest->setVerticalHeaderLabels(labels_request);
    ui->tableWidgetRequest->setHorizontalHeaderLabels(headerLabels);


    ui->tableWidgetResponse->setColumnCount(1);
    ui->tableWidgetResponse->setRowCount(RESPONSE_TABLE_ROW_COUNT);

    for (int i = 0; i < RESPONSE_TABLE_ROW_COUNT; ++i) {
        responseQTableWidgetItems[i] = new QTableWidgetItem(tr("no msg yet"));
        responseQTableWidgetItems[i]->setTextAlignment(Qt::AlignVCenter);
        ui->tableWidgetResponse->setItem(i, 0, responseQTableWidgetItems[i]);

        QVariant data = model_response->headerData(i, Qt::Vertical);
        labels_response << QString("%1").arg(data.toInt() - 1);
    }

    ui->tableWidgetResponse->setVerticalHeaderLabels(labels_request);
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
        responseQTableWidgetItems[i]->setText(QString::number(server->msg_in[i], 16));
    }

   // ui->doubleSpinBoxTemperature->setValue(server->temperature);
}

void MainWindow::info(QString s) {
 //   ui->terminal->append(s);
    ui->statusbar->showMessage(s);
}

void MainWindow::init() {
    on_spinBox_Motor_Slot_valueChanged(0);
    on_pushButton_load_config_released();
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

void MainWindow::openNewWindow()
{
    reference_menu = new NewWindow();

    reference_menu->show();

}


void MainWindow::on_pushButton_Reference_clicked()
{
  openNewWindow();
}



