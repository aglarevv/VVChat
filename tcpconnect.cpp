#include "tcpconnect.h"

TcpConnect *TcpConnect::instance()
{
    static TcpConnect conn;
    return &conn;
}

bool TcpConnect::connect()
{
    QString configPath = "/home/aglarevv/work/VVChat/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    QString ip = settings.value("ip").toString();
    int port = settings.value("port").toInt();
    qDebug() << ip << " " << port ;
    m_socket->connectToHost(ip,port);
    if (!m_socket->waitForConnected()) {
        qDebug() << "Error: " << m_socket->errorString();
        return false;
    }
    return true;
}

QTcpSocket *TcpConnect::getSocket()
{
    return m_socket;
}

TcpConnect::TcpConnect()
{
    m_socket = new QTcpSocket;
}


