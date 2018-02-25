#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //this->setStyleSheet("background-color: black;");

    ui->tableWidgetRequest->setColumnCount(3);
    ui->tableWidgetRequest->setRowCount(REQUEST_TABLE_ROW_COUNT);

    QAbstractItemModel *model_request = ui->tableWidgetRequest->model();
    QAbstractItemModel *model_response = ui->tableWidgetResponse->model();
    QStringList labels_request, labels_response;
    QStringList headerLabels;
    headerLabels << QString("HEX") << QString("DEX") << QString("Binary");

    //PLOTS_____________________________________________________________________________________________________________________

    //For range adjustment
    imu_pitch_max = -500;
    imu_roll_max = -500;
    imu_pitch_speed_max = -500;
    imu_roll_speed_max = -500;
    imu_yaw_max = -500;
    imu_yaw_speed_max = -500;

    imu_pitch_min = 500;
    imu_roll_min = 500;
    imu_pitch_speed_min = 500;
    imu_roll_speed_min = 500;
    imu_yaw_min = 500;
    imu_yaw_speed_min = 500;

    //PITCH_____________________________________________________________________________________________________________________

    //ui->plot_window_pitch = new QCustomPlot();
    ui->plot_window_pitch->xAxis->setLabel("T(c)");
    ui->plot_window_pitch->yAxis->setLabel("PITCH");

    ui->plot_window_pitch->setInteraction(QCP::iRangeZoom,true);   // +zoom in and out
    ui->plot_window_pitch->setInteraction(QCP::iRangeDrag, true);  // +dragging
    //ui->plot_window_pitch->axisRect()->setRangeDrag(Qt::Horizontal);   // +dragging (only xAxis)
    //ui->plot_window_pitch->axisRect()->setRangeZoom(Qt::Horizontal);   // +zoom in and out (only xAxis)

    //plot_pitch = plot_window_pitch->addGraph();
    //plot_pitch->setPen(QPen(QColor(40, 110, 255)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->plot_window_pitch->xAxis->setTicker(timeTicker);
    ui->plot_window_pitch->axisRect()->setupFullAxesBox();
    ui->plot_window_pitch->yAxis->setRange(-10, 10);

    // making left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot_window_pitch->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_pitch->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot_window_pitch->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_pitch->yAxis2, SLOT(setRange(QCPRange)));

    //ROLL_____________________________________________________________________________________________________________________

    ui->plot_window_roll->xAxis->setLabel("T(c)");
    ui->plot_window_roll->yAxis->setLabel("ROLL");


    ui->plot_window_roll->setInteraction(QCP::iRangeZoom,true);   // +zoom in and out
    ui->plot_window_roll->setInteraction(QCP::iRangeDrag, true);  // +dragging


    ui->plot_window_roll->xAxis->setTicker(timeTicker);
    ui->plot_window_roll->axisRect()->setupFullAxesBox();
    ui->plot_window_roll->yAxis->setRange(-10, 10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot_window_roll->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_roll->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot_window_roll->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_roll->yAxis2, SLOT(setRange(QCPRange)));

    //YAW_____________________________________________________________________________________________________________________

    ui->plot_window_yaw->xAxis->setLabel("T(c)");
    ui->plot_window_yaw->yAxis->setLabel("YAW");


    ui->plot_window_yaw->setInteraction(QCP::iRangeZoom,true);   // +zoom in and out
    ui->plot_window_yaw->setInteraction(QCP::iRangeDrag, true);  // +dragging


    ui->plot_window_yaw->xAxis->setTicker(timeTicker);
    ui->plot_window_yaw->axisRect()->setupFullAxesBox();
    ui->plot_window_yaw->yAxis->setRange(-10, 10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot_window_yaw->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_yaw->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot_window_yaw->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_yaw->yAxis2, SLOT(setRange(QCPRange)));

    //PITCH_SPEED_____________________________________________________________________________________________________________________

    ui->plot_window_pitch_speed->xAxis->setLabel("T(c)");
    ui->plot_window_pitch_speed->yAxis->setLabel("PITCH_SPEED");


    ui->plot_window_pitch_speed->setInteraction(QCP::iRangeZoom,true);   // +zoom in and out
    ui->plot_window_pitch_speed->setInteraction(QCP::iRangeDrag, true);  // +dragging


    ui->plot_window_pitch_speed->xAxis->setTicker(timeTicker);
    ui->plot_window_pitch_speed->axisRect()->setupFullAxesBox();
    ui->plot_window_pitch_speed->yAxis->setRange(-10, 10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot_window_pitch_speed->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_pitch_speed->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot_window_pitch_speed->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_pitch_speed->yAxis2, SLOT(setRange(QCPRange)));

    //ROLL_SPEED_____________________________________________________________________________________________________________________

    ui->plot_window_roll_speed->xAxis->setLabel("T(c)");
    ui->plot_window_roll_speed->yAxis->setLabel("ROLL_SPEED");


    ui->plot_window_roll_speed->setInteraction(QCP::iRangeZoom,true);   // +zoom in and out
    ui->plot_window_roll_speed->setInteraction(QCP::iRangeDrag, true);  // +dragging


    ui->plot_window_roll_speed->xAxis->setTicker(timeTicker);
    ui->plot_window_roll_speed->axisRect()->setupFullAxesBox();
    ui->plot_window_roll_speed->yAxis->setRange(-10, 10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot_window_roll_speed->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_roll_speed->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot_window_roll_speed->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_roll_speed->yAxis2, SLOT(setRange(QCPRange)));


    //YAW_SPEED_____________________________________________________________________________________________________________________

    ui->plot_window_yaw_speed->xAxis->setLabel("T(c)");
    ui->plot_window_yaw_speed->yAxis->setLabel("YAW_SPEED");


    ui->plot_window_yaw_speed->setInteraction(QCP::iRangeZoom,true);   // +zoom in and out
    ui->plot_window_yaw_speed->setInteraction(QCP::iRangeDrag, true);  // +dragging


    ui->plot_window_yaw_speed->xAxis->setTicker(timeTicker);
    ui->plot_window_yaw_speed->axisRect()->setupFullAxesBox();
    ui->plot_window_yaw_speed->yAxis->setRange(-10, 10);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plot_window_yaw_speed->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_yaw_speed->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plot_window_yaw_speed->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plot_window_yaw_speed->yAxis2, SLOT(setRange(QCPRange)));


    ui->plot_window_pitch->addGraph();
    ui->plot_window_pitch->graph(0)->setPen(QPen(QColor(20, 110, 255)));
    ui->plot_window_pitch_speed->addGraph();
    ui->plot_window_pitch_speed->graph(0)->setPen(QPen(QColor(20, 110, 255)));

    ui->plot_window_roll->addGraph();
    ui->plot_window_roll->graph(0)->setPen(QPen(QColor(180, 1, 1)));
    ui->plot_window_roll_speed->addGraph();
    ui->plot_window_roll_speed->graph(0)->setPen(QPen(QColor(180, 1, 1)));

    ui->plot_window_yaw->addGraph();
    ui->plot_window_yaw->graph(0)->setPen(QPen(QColor(228, 110, 50)));
    ui->plot_window_yaw_speed->addGraph();
    ui->plot_window_yaw_speed->graph(0)->setPen(QPen(QColor(228, 110, 50)));

    //_________________________________________________________________________________________________________________________________

    /*labels_response <<   "0:roll"<<
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
                      "26:";*/

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

    // Stopwatch
    time = new QTime();
    bufferTimeMain = 0;
    bufferTimeMission = 0;
    timer = new QTimer();
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(500);

    startPauseBtn = ui->StartPauseButton;
    commonTime = ui->Commontime;
    currentMissionTime = ui->Missiontime;
    missionNumber = ui->Mn;

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
        requestQTableWidgetItemsBinary[i]->setText(QString::number(server->msg_to_send[i], 2));
    }

    for (int i = request_length; i < REQUEST_TABLE_ROW_COUNT; ++i) {
        requestQTableWidgetItemsHEX[i]->setText(QString("NO"));
        requestQTableWidgetItemsDEC[i]->setText(QString("NO"));
        requestQTableWidgetItemsBinary[i]->setText(QString("NO"));
    }

    for (int i = 0; i < RESPONSE_LENGTH; ++i) {
        responseQTableWidgetItemsHEX[i]->setText(QString::number(server->msg_in[i], 16));
        responseQTableWidgetItemsDEC[i]->setText(QString::number(server->msg_in[i], 10));
        responseQTableWidgetItemsBinary[i]->setText(QString::number(server->msg_in[i], 2));
    }

    for (int i = RESPONSE_LENGTH; i < RESPONSE_TABLE_ROW_COUNT; ++i) {
        responseQTableWidgetItemsHEX[i]->setText(QString("NO"));
        responseQTableWidgetItemsDEC[i]->setText(QString("NO"));
        responseQTableWidgetItemsBinary[i]->setText(QString("NO"));
    }

    ui->lcdNumber_current_HLB->display(QString::number(server->current_HLB));
    ui->lcdNumber_current_HLF->display(QString::number(server->current_HLF));
    ui->lcdNumber_current_HRB->display(QString::number(server->current_HRB));
    ui->lcdNumber_current_HRF->display(QString::number(server->current_HRF));

    ui->lcdNumber_current_VB->display(QString::number(server->current_VB));
    ui->lcdNumber_current_VF->display(QString::number(server->current_VF));
    ui->lcdNumber_current_VL->display(QString::number(server->current_VL));
    ui->lcdNumber_current_VR->display(QString::number(server->current_VR));


    ui->lcdNumber_velocity_HLB->display(QString::number(server->velocity_HLB));
    ui->lcdNumber_velocity_HLF->display(QString::number(server->velocity_HLF));
    ui->lcdNumber_velocity_HRB->display(QString::number(server->velocity_HRB));
    ui->lcdNumber_velocity_HRF->display(QString::number(server->velocity_HRF));

    ui->lcdNumber_velocity_VB->display(QString::number(server->velocity_VB));
    ui->lcdNumber_velocity_VF->display(QString::number(server->velocity_VF));
    ui->lcdNumber_velocity_VL->display(QString::number(server->velocity_VL));
    ui->lcdNumber_velocity_VR->display(QString::number(server->velocity_VR));

    ui->label_BT->setText(server->bt_data);

    ui->lcdNumber_velocity_Light->display(QString::number(server->current_light));
    ui->lcdNumber_velocity_bottomLight->display(QString::number(server->current_bottom_light));
    ui->lcdNumber_velocity_Agar->display(QString::number(server->current_agar));
    ui->lcdNumber_velocity_Grab->display(QString::number(server->current_grab));
    ui->lcdNumber_velocity_Rotate->display(QString::number(server->current_grab_rotate));
    ui->lcdNumber_velocity_Tilt->display(QString::number(server->current_tilt));

    ui->lcdNumber_Lost->display(server->msg_lost_counter);
    ui->lcdNumber_Received->display(server->msg_received_counter);
    ui->lcdNumber_Percent->display(server->msg_lost_percent);

    ui->checkBox_SDepth->setChecked(joystick->stabilize_depth);
    ui->checkBox_SRoll->setChecked(joystick->stabilize_roll);
    ui->checkBox_SPitch->setChecked(joystick->stabilize_pitch);

    std::cout << "STABILIZE DEPTH = " << joystick->stabilize_depth << std::endl;
    std::cout << "STABILIZE ROLL  = " << joystick->stabilize_roll  << std::endl;
    std::cout << "STABILIZE PITCH = " << joystick->stabilize_pitch << std::endl;

    ui->graphicsPFD_2->setRoll(server->imu_roll);
    ui->graphicsPFD_2->setPitch(server->imu_pitch);
    ui->graphicsPFD_2->setTurnRate(server->imu_roll_speed);
    ui->graphicsPFD_2->setHeading(server->imu_yaw);
    ui->graphicsPFD_2->setAltitude(server->imu_depth);

    //PLOTS_____________________________________________________________________________________________________
    std::cout << "Drawing plots..." << std::endl;

    //static QTime time(QTime::currentTime());
    //key1 =time.elapsed()/1000.0;

    static double time_d = 0;
    if ((server->key1 - time_d) > 0.001) {
        //PITCH___________________________________
        ui->plot_window_pitch->graph(0)->addData(server->key1, server->imu_pitch_d); // SET DATA
        ui->plot_window_pitch->xAxis->setRange(server->key1, 8, Qt::AlignRight);
        if ((server->imu_pitch_d)>imu_pitch_max)
        {
            imu_pitch_max = server->imu_pitch_d;
            //std::cout << "imu_pitch_max = " << imu_pitch_max << std::endl;  // can be added for checking
        }
        if ((server->imu_pitch_d)<imu_pitch_min)
        {
            imu_pitch_min = server->imu_pitch_d;
            //std::cout << "imu_pitch_min = " << imu_pitch_min << std::endl;  // can be added for checking
        }
        ui->plot_window_pitch->yAxis->setRange(imu_pitch_min-2,imu_pitch_max+2);
        ui->plot_window_pitch->replot();           // drawing
        ui->plot_window_pitch->graph(0)->rescaleValueAxis(true);

        //ROLL____________________________________

        ui->plot_window_roll->graph(0)->addData(server->key1, server->imu_roll_d); // SET DATA
        ui->plot_window_roll->xAxis->setRange(server->key1, 8, Qt::AlignRight);
        if ((server->imu_roll_d)>imu_roll_max)
        {
            imu_roll_max = server->imu_roll_d;
            //std::cout << "imu_roll_max = " << imu_roll_max << std::endl;  // can be added for checking
        }
        if ((server->imu_roll_d)<imu_roll_min)
        {
            imu_roll_min = server->imu_roll_d;
            //std::cout << "imu_roll_min = " << imu_roll_min << std::endl;  // can be added for checking
        }
        ui->plot_window_roll->yAxis->setRange(imu_roll_min-2,imu_roll_max+2);
        ui->plot_window_roll->replot();           // drawing
        ui->plot_window_roll->graph(0)->rescaleValueAxis(true);

        //YAW____________________________________

        ui->plot_window_yaw->graph(0)->addData(server->key1, server->imu_yaw_d); // SET DATA
        ui->plot_window_yaw->xAxis->setRange(server->key1, 8, Qt::AlignRight);
        if ((server->imu_yaw_d)>imu_yaw_max)
        {
            imu_yaw_max = server->imu_yaw_d;
            //std::cout << "imu_roll_max = " << imu_yaw_max << std::endl; // can be added for checking
        }
        if ((server->imu_yaw_d)<imu_yaw_min)
        {
            imu_yaw_min = server->imu_yaw_d;
            //std::cout << "imu_roll_min = " << imu_yaw_min << std::endl; // can be added for checking
        }
        ui->plot_window_yaw->yAxis->setRange(imu_yaw_min-2,imu_yaw_max+2);
        ui->plot_window_yaw->replot();           // drawing
        ui->plot_window_yaw->graph(0)->rescaleValueAxis(true);



        //PITCH_SPEED_________________________________

        ui->plot_window_pitch_speed->graph(0)->addData(server->key1, server->imu_pitch_speed_d); // SET DATA
        ui->plot_window_pitch_speed->xAxis->setRange(server->key1, 8, Qt::AlignRight);
        if (abs(server->imu_pitch_speed_d)>imu_pitch_speed_max)
        {
            imu_pitch_speed_max = abs(server->imu_pitch_speed_d);
            //std::cout << "imu_pitch_speed_max = " << imu_pitch_speed_max << std::endl;  // can be added for checking
        }
        if ((server->imu_pitch_speed_d)<imu_pitch_speed_min)
        {
            imu_pitch_speed_min = server->imu_pitch_speed_d;
            //std::cout << "imu_pitch_speed_min = " << imu_pitch_speed_min << std::endl;  // can be added for checking
        }
        ui->plot_window_pitch_speed->yAxis->setRange(imu_pitch_speed_min-2,imu_pitch_speed_max+2);
        ui->plot_window_pitch_speed->replot();           // drawing
        ui->plot_window_pitch_speed->graph(0)->rescaleValueAxis(true);

        //ROLL_SPEED_________________________________

        ui->plot_window_roll_speed->graph(0)->addData(server->key1, server->imu_roll_speed_d); // SET DATA
        ui->plot_window_roll_speed->xAxis->setRange(server->key1, 8, Qt::AlignRight);
        if ((server->imu_roll_speed_d)>imu_roll_speed_max)
        {
            imu_roll_speed_max = server->imu_roll_speed_d;
            //std::cout << "imu_roll_speed_max = " << imu_roll_speed_max << std::endl;  // can be added for checking
        }
        if ((server->imu_roll_speed_d)<imu_roll_speed_min)
        {
            imu_roll_speed_min = server->imu_roll_speed_d;
            //std::cout << "imu_roll_speed_min = " << imu_roll_speed_min << std::endl;  // can be added for checking
        }
        ui->plot_window_roll->yAxis->setRange(imu_roll_speed_min-2,imu_roll_speed_max+2);
        ui->plot_window_roll_speed->replot();           // drawing
        ui->plot_window_roll_speed->graph(0)->rescaleValueAxis(true);

        //YAW_SPEED_________________________________

        ui->plot_window_yaw_speed->graph(0)->addData(server->key1, server->imu_yaw_speed_d); // SET DATA
        ui->plot_window_yaw_speed->xAxis->setRange(server->key1, 8, Qt::AlignRight);
        if ((server->imu_yaw_speed_d)>imu_yaw_speed_max)
        {
            imu_yaw_speed_max = server->imu_yaw_speed_d;
            //std::cout << "imu_yaw_speed_max = " << imu_yaw_speed_max << std::endl;  // can be added for checking
        }
        if ((server->imu_yaw_speed_d)<imu_yaw_speed_min)
        {
            imu_yaw_speed_min = server->imu_yaw_speed_d;
            //std::cout << "imu_yaw_speed_min = " << imu_yaw_speed_min << std::endl;  // can be added for checking
        }
        ui->plot_window_yaw->yAxis->setRange(imu_yaw_speed_min-2,imu_yaw_speed_max+2);
        ui->plot_window_yaw_speed->replot();           // drawing
        ui->plot_window_yaw_speed->graph(0)->rescaleValueAxis(true);

        time_d = server->key1;
    } else {
        std::cout << "Passed due to low dT." << std::endl;
    }
    ui->graphicsPFD_2->update();
    std::cout << "Drawing done." << std::endl;
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
        ui->doubleSpinBox_depth_iborders->setValue(settings->depth.iborders);
        ui->doubleSpinBox_depth_igain->setValue(settings->depth.igain);
        ui->doubleSpinBox_depth_k1->setValue(settings->depth.k1);
        ui->doubleSpinBox_depth_k2->setValue(settings->depth.k2);
        ui->doubleSpinBox_depth_k3->setValue(settings->depth.k3);
        ui->doubleSpinBox_depth_k4->setValue(settings->depth.k4);
        ui->doubleSpinBox_depth_pgain->setValue(settings->depth.pgain);

        ui->doubleSpinBox_pitch_iborders->setValue(settings->pitch.iborders);
        ui->doubleSpinBox_pitch_igain->setValue(settings->pitch.igain);
        ui->doubleSpinBox_pitch_k1->setValue(settings->pitch.k1);
        ui->doubleSpinBox_pitch_k2->setValue(settings->pitch.k2);
        ui->doubleSpinBox_pitch_k3->setValue(settings->pitch.k3);
        ui->doubleSpinBox_pitch_k4->setValue(settings->pitch.k4);
        ui->doubleSpinBox_pitch_pgain->setValue(settings->pitch.pgain);

        ui->doubleSpinBox_roll_iborders->setValue(settings->roll.iborders);
        ui->doubleSpinBox_roll_igain->setValue(settings->roll.igain);
        ui->doubleSpinBox_roll_k1->setValue(settings->roll.k1);
        ui->doubleSpinBox_roll_k2->setValue(settings->roll.k2);
        ui->doubleSpinBox_roll_k3->setValue(settings->roll.k3);
        ui->doubleSpinBox_roll_k4->setValue(settings->roll.k4);
        ui->doubleSpinBox_roll_pgain->setValue(settings->roll.pgain);

        ui->doubleSpinBox_yaw_iborders->setValue(settings->yaw.iborders);
        ui->doubleSpinBox_yaw_igain->setValue(settings->yaw.igain);
        ui->doubleSpinBox_yaw_k1->setValue(settings->yaw.k1);
        ui->doubleSpinBox_yaw_k2->setValue(settings->yaw.k2);
        ui->doubleSpinBox_yaw_k3->setValue(settings->yaw.k3);
        ui->doubleSpinBox_yaw_k4->setValue(settings->yaw.k4);
        ui->doubleSpinBox_yaw_pgain->setValue(settings->yaw.pgain);

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

void MainWindow::on_pushButton_save_config_released()
{
    settings->depth.iborders = ui->doubleSpinBox_depth_iborders->value();
    settings->depth.igain = ui->doubleSpinBox_depth_igain->value();
    settings->depth.k1 = ui->doubleSpinBox_depth_k1->value();
    settings->depth.k2 = ui->doubleSpinBox_depth_k2->value();
    settings->depth.k3 = ui->doubleSpinBox_depth_k3->value();
    settings->depth.k4 = ui->doubleSpinBox_depth_k4->value();
    settings->depth.pgain = ui->doubleSpinBox_depth_pgain->value();

    settings->pitch.iborders = ui->doubleSpinBox_pitch_iborders->value();
    settings->pitch.igain = ui->doubleSpinBox_pitch_igain->value();
    settings->pitch.k1 = ui->doubleSpinBox_pitch_k1->value();
    settings->pitch.k2 = ui->doubleSpinBox_pitch_k2->value();
    settings->pitch.k3 = ui->doubleSpinBox_pitch_k3->value();
    settings->pitch.k4 = ui->doubleSpinBox_pitch_k4->value();
    settings->pitch.pgain = ui->doubleSpinBox_pitch_pgain->value();

    settings->roll.iborders = ui->doubleSpinBox_roll_iborders->value();
    settings->roll.igain = ui->doubleSpinBox_roll_igain->value();
    settings->roll.k1 = ui->doubleSpinBox_roll_k1->value();
    settings->roll.k2 = ui->doubleSpinBox_roll_k2->value();
    settings->roll.k3 = ui->doubleSpinBox_roll_k3->value();
    settings->roll.k4 = ui->doubleSpinBox_roll_k4->value();
    settings->roll.pgain = ui->doubleSpinBox_roll_pgain->value();

    settings->yaw.iborders = ui->doubleSpinBox_yaw_iborders->value();
    settings->yaw.igain = ui->doubleSpinBox_yaw_igain->value();
    settings->yaw.k1 = ui->doubleSpinBox_yaw_k1->value();
    settings->yaw.k2 = ui->doubleSpinBox_yaw_k2->value();
    settings->yaw.k3 = ui->doubleSpinBox_yaw_k3->value();
    settings->yaw.k4 = ui->doubleSpinBox_yaw_k4->value();
    settings->yaw.pgain = ui->doubleSpinBox_yaw_pgain->value();


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

void MainWindow::on_checkBox_SPitch_toggled(bool checked)
{
    joystick->stabilize_pitch = checked;
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
