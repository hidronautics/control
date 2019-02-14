#include <QApplication>
#include <QObject>
#include "server.h"
#include "mainwindow.h"
#include "settings.h"
#include "logging_categories.h"

double X[2000][2];


const int MAX_JOYSTICK_ID = 5;

MainWindow* mainWindow;
Server* server;
Settings* settings;
Joystick* joystick;

QScopedPointer<QFile>   m_logFile;

void initMainWindow();
void initServer();
void initSettings();
void initJoystick();

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    m_logFile.reset(new QFile("log.txt"));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);

    mainWindow = new MainWindow();
    server = new Server();
    settings = new Settings();
    joystick = new Joystick();

    qInfo(logInfo()) << "Objects loaded";

    initMainWindow();
    initServer();
    initSettings();
    initJoystick();

    qInfo(logInfo()) << "Objects inited";

    mainWindow->init(); // Дублирование кода

    QObject::connect(server, SIGNAL(imSleeping()), mainWindow, SLOT(serverIsSleeping()));
    QObject::connect(server, SIGNAL(info(QString s)), mainWindow, SLOT(info(QString s)));
    QObject::connect(mainWindow, SIGNAL(connect_fake()), server, SLOT(connect_fake()));
    QObject::connect(mainWindow, SIGNAL(tryConnect()), server, SLOT(connect_com()));
    QObject::connect(mainWindow, SIGNAL(disconnect()), server, SLOT(disconnect_com()));
    QObject::connect(server, &Server::updateCsView, mainWindow, &MainWindow::update_csView);

    mainWindow->show();


    return a.exec();
}

void initMainWindow() {
    mainWindow->joystick = joystick;
    mainWindow->server = server; // Не должно быть
    mainWindow->settings = settings;
    mainWindow->init();
}

void initServer() {
    server->settings = settings;
    server->j = joystick;
}

void initSettings() {
    settings->oldCS = 0;
    settings->CS = STAB_YAW;
}

void initJoystick() {
    qInfo(logInfo()) << "Connecting joystick";
    bool flag = false;
    for (int i = 0; i < MAX_JOYSTICK_ID; ++i) {
        if (joystick->init(i)) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        qWarning(logInfo()) << "No joystick found";
        std::cout << "No joystick found among ids from 0 to " << MAX_JOYSTICK_ID-1 << std::endl;
    } else {
        std::cout << "Joystick connected" << std::endl;
        joystick->update();
    }
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }
    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": "
        << msg << endl;
    out.flush();    // Очищаем буферизированные данные
}
