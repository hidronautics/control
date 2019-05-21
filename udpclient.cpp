#include "udpclient.h"

UDPClient::UDPClient(QObject *parent) :
    QObject(parent)
{
    _socket_p = new QUdpSocket(this);
    connect(_socket_p, &QUdpSocket::readyRead,
            this, &UDPClient::response_handler);
}

UDPClient::~UDPClient()
{
    delete _socket_p;
}

void UDPClient::set_address(const QHostAddress &host, qint64 port)
{
    _host = host;
    _port = port;
}


void UDPClient::set_address(const QString &host, const QString &port)
{
    set_address(QHostAddress(host), port.toInt());
}


void UDPClient::request(const QByteArray &package)
{
    _socket_p->writeDatagram(package, _host, _port);
}

void UDPClient::response_handler()
{
    QByteArray buf;
    buf.resize(_socket_p->pendingDatagramSize());

    _socket_p->readDatagram(buf.data(), buf.size());

    qDebug() << "received datagram - " << buf.toHex().toUpper();

    emit received_package(buf);
}
