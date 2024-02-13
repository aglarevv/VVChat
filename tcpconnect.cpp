#include "tcpconnect.h"

TcpConnect::TcpConnect()
{

}

bool TcpConnect::connect()
{
    QString configPath = "./config.ini";
    QSettings settings(configPath, QSettings::IniFormat);
    QString ip = settings.value("ip").toString();
    int port = settings.value("port").toInt();
    m_socket->connectToHost(ip,port);
    if (!m_socket->waitForConnected()) {
        qDebug() << "Error: " << m_socket->errorString();
        return false;
    }
    return true;
}
