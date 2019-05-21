#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class UDPClient : public QObject
{
    Q_OBJECT
public:
    explicit UDPClient(QObject *parent = 0);
    virtual ~UDPClient();

    void set_address(const QHostAddress &host, qint64 port);
    void set_address(const QString &host, const QString &port);
    void request(const QByteArray &package);

private:
    QUdpSocket *_socket_p;
    QHostAddress _host;
    qint64 _port;

signals:
    void received_package(const QByteArray &package);

private slots:
    void response_handler();
};

#endif // UDPCLIENT_H
