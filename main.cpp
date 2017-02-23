#include <QApplication>
#include "server.h"
#include "mainwindow.h"
#include "settings.h"

const int MAX_JOYSTICK_ID = 5;

MainWindow* mainWindow;
Server* server;
Settings* settings;
Joystick* joystick;

void initMainWindow();
void initServer();
void initSettings();
void initJoystick();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainWindow = new MainWindow();
    server = new Server();
    settings = new Settings();
    joystick = new Joystick();

    initMainWindow();
    initServer();
    initSettings();
    initJoystick();

    Q_INIT_RESOURCE(qfi);


    mainWindow->show();
    return a.exec();
}

void initMainWindow() {
    mainWindow->joystick = joystick;
    mainWindow->server = server;
    mainWindow->settings = settings;
}
void initServer() {
    server->j = joystick;
}

void initSettings() {

}

void initJoystick() {
    std::cout << "Connecting joystick..." << std::endl;
    bool flag = false;
    for (int i = 0; i < MAX_JOYSTICK_ID; ++i) {
        if (joystick->init(i)) {
            flag = true;
            break;
        }
    }
    if (!flag)
        std::cout << "No joystick found among ids from 0 to " << MAX_JOYSTICK_ID-1 << std::endl;
    else {
        std::cout << "Joystick connected" << std::endl;
        joystick->update();
    }
}

