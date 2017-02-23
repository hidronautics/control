#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTableWidget>
#include <iostream>
#include "server.h"
#include "joystick.h"
#include "settings.h"
//added
#include "NewWindow.h"

#define REQUEST_TABLE_ROW_COUNT         REQUEST_CONFIG_LENGTH
#define RESPONSE_TABLE_ROW_COUNT        RESPONSE_LENGTH



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init();

    //Motor *motors;
    Server *server;
    Joystick *joystick;
    Settings *settings;

public slots:
//    void info(QString s);
//    void serverIsSleeping();
      void openNewWindow();

private slots:

//    void on_checkBox_Calibration_Control_toggled(bool checked);

//    void on_pushButton_Test_Motor_released();

    void on_pushButton_Connect_released();

//    void on_pushButton_Stop_Test_released();

//    void on_pushButton_Save_released();

//    void on_spinBox_Motor_Slot_valueChanged(int arg1);

    void on_pushButton_Disconnect_released();

//    void on_pushButton_load_config_released();

//    void on_pushButton_send_config_released();

    void on_pushButton_released();

//    void on_pushButton_2_released();

//    void on_pushButton_3_released();

//    void on_pushButton_save_config_released();

//    void on_pushButton_4_released();

    void on_radioButton_Joystick_released();

    void on_radioButton_Keyboard_released();

    void on_radioButton_XBox_released();

    void on_pushButton_Bluetooth_clicked();

signals:
    void newValues(int* vals);\

//    void changeMessageType(uint8_t type);

    void tryConnect();
    void disconnect();
    void connect_fake();
    void changeMotorSetting(int slot, QString motorID, bool inverse);

private:
    Ui::MainWindow *ui;
    NewWindow *bluetooth_menu;

    QTableWidgetItem *requestQTableWidgetItemsHEX[REQUEST_TABLE_ROW_COUNT];
    QTableWidgetItem *requestQTableWidgetItemsDEC[REQUEST_TABLE_ROW_COUNT];
    QTableWidgetItem *responseQTableWidgetItems[RESPONSE_TABLE_ROW_COUNT];
};

#endif // MAINWINDOW_H
