#ifndef TCPCONNECT_H
#define TCPCONNECT_H


#include <QTcpSocket>
#include <QSettings>
#include <QSettings>

class TcpConnect
{
public:
    TcpConnect();

    bool connect();

    QTcpSocket * m_socket;
};

#endif // TCPCONNECT_H
