#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QTableWidget>
#include <iostream>
#include <QTimer>
#include <QTime>
#include <QPushButton>
#include <QLabel>
#include "server.h"
#include "joystick.h"
#include "settings.h"
#include "qmainwindow.h"

#define REQUEST_TABLE_ROW_COUNT         REQUEST_CONFIG_LENGTH
#define RESPONSE_TABLE_ROW_COUNT        RESPONSE_LENGTH

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

    //Motor *motors;
    Server *server;
    Joystick *joystick;
    Settings *settings;

    void updateCsView();
    void saveCsView();
    void updateCsLabels();
public slots:
    void info(QString s);
    void serverIsSleeping();
    void update_csView();

private slots:

    void on_checkBox_Calibration_Control_toggled(bool checked);

    void on_pushButton_Test_Motor_released();

    void on_pushButton_Connect_released();

    void on_pushButton_Stop_Test_released();

    void on_pushButton_Save_released();

    void on_spinBox_Motor_Slot_valueChanged(int arg1);

    void on_pushButton_Disconnect_released();

    void on_pushButton_CS_loadConfig_released();

    void on_pushButton_send_config_released();

    void on_pushButton_released();

    void on_pushButton_2_released();

    void on_pushButton_3_released();

    void on_pushButton_CS_saveConfig_released();

    void on_pushButton_4_released();

    void on_radioButton_Joystick_released();

    void on_radioButton_Keyboard_released();

    void on_radioButton_XBox_released();

    void on_comboBox_Parity_currentTextChanged(const QString &arg1);

    void on_comboBox_StopBits_currentTextChanged(const QString &arg1);

    void on_comboBox_FlowControl_currentTextChanged(const QString &arg1);

    void on_spinBox_COM_valueChanged(int arg1);

    void on_comboBox_DataBits_currentTextChanged(const QString &arg1);

    void on_comboBox_BaudRate_currentTextChanged(const QString &arg1);

    void on_checkBox_toggled(bool checked);

    void on_spinBox_PauseAfterSent_valueChanged(int arg1);

    void on_spinBox_PauseAfterReceived_valueChanged(int arg1);

    void on_checkBox_2_toggled(bool checked);

    void on_checkBox_SDepth_toggled(bool checked);

    void on_checkBox_SRoll_toggled(bool checked);

    void on_checkBox_SYaw_toggled(bool checked);

    void on_StartPauseButton_clicked();

    void on_ClearButton_clicked();

    void on_NextButton_clicked();

    void updateTime();

    void on_radioButton_CS_YawSelect_released();

    void on_radioButton_CS_DepthSelect_released();

    void on_radioButton_CS_RollSelect_released();

    void on_radioButton_CS_PitchSelect_released();

signals:
    void newValues(int* vals);

    //void changeMessageType(uint8_t type);

    void tryConnect();
    void disconnect();
    void connect_fake();
    void changeMotorSetting(int slot, QString motorID, bool inverse);

private:
    Ui::MainWindow *ui;
    //NewWindow *bluetooth_menu;

    QTableWidgetItem *requestQTableWidgetItemsHEX[REQUEST_TABLE_ROW_COUNT];
    QTableWidgetItem *requestQTableWidgetItemsDEC[REQUEST_TABLE_ROW_COUNT];
    QTableWidgetItem *requestQTableWidgetItemsBinary[REQUEST_TABLE_ROW_COUNT];

    QTableWidgetItem *responseQTableWidgetItemsHEX[RESPONSE_TABLE_ROW_COUNT];
    QTableWidgetItem *responseQTableWidgetItemsDEC[RESPONSE_TABLE_ROW_COUNT];
    QTableWidgetItem *responseQTableWidgetItemsBinary[RESPONSE_TABLE_ROW_COUNT];

    QPushButton *startPauseBtn;
    QLabel *commonTime;
    QLabel *currentMissionTime;
    QLabel *missionNumber;
    QTime *time;
    int bufferTimeMain;
    int bufferTimeMission;
    int x;
    QTimer *timer;
    QString getWatchTime(int time);

    int imu_pitch_max;
    int imu_roll_max;
    int imu_pitch_speed_max;
    int imu_roll_speed_max;
    int imu_yaw_max;
    int imu_yaw_speed_max;

    int imu_pitch_min;
    int imu_roll_min;
    int imu_pitch_speed_min;
    int imu_roll_speed_min;
    int imu_yaw_min;
    int imu_yaw_speed_min;

};

#endif // MAINWINDOW_H

