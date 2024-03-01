#include "flushinfo.h"
#include "signin.h"
#include "mainwindow.h"
extern TcpConnect* tcp;
FlushInfo::FlushInfo()
{

}

FlushInfo::FlushInfo(json js,QObject* parent):QThread(parent)
{

    this->js = js;
    connect(tcp->getSocket(),&QTcpSocket::readyRead,this,&FlushInfo::run);
    //
//    connect(&timer, &QTimer::timeout, this, &FlushInfo::run);
//    timer.start(1000);
}

void FlushInfo::run() {
    json receivedJson;
    //MainWindow w(receivedJson);
    QByteArray data;
    data = tcp->getSocket()->readAll();
    QString jsonString = QString::fromUtf8(data);
    receivedJson = json::parse(jsonString.toStdString());
    if(receivedJson["msgType"] == GETINFO_ACK){
        std::cout << "updata info : " << receivedJson;
        //connect(this,&FlushInfo::dataReady,&w,std::bind(&MainWindow::getInfo,this,receivedJson));
        emit dataReady(receivedJson);
    }else if(receivedJson["msgType"] == ONE_CHAT){

    }



}

void FlushInfo::timerout(){
    json data;
    data["msgType"] = GETINFO;
    data["account"] = js["account"];
    std::string request = data.dump();
    if((tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1)) != -1){
        //connect(tcp->getSocket(),&QTcpSocket::readyRead,this,&FlushInfo::fetchDataFromServer);
        connect(tcp->getSocket(),&QTcpSocket::readyRead,this,[=](){
            QObject::disconnect(tcp->getSocket(), &QTcpSocket::readyRead, this, nullptr);
            json receivedJson;
            QByteArray data;
            data = tcp->getSocket()->readAll();
            QString jsonString = QString::fromUtf8(data);

            receivedJson = json::parse(jsonString.toStdString());
            if(receivedJson["msgType"] != GETINFO_ACK && receivedJson["errorNo"] != 0){
                return;
            }

            emit dataReady(receivedJson);
        });
    }
}

void FlushInfo::fetchDataFromServer() {
    QObject::disconnect(tcp->getSocket(), &QTcpSocket::readyRead, this, &FlushInfo::fetchDataFromServer);
    json receivedJson;
    QByteArray data;
    data = tcp->getSocket()->readAll();
    QString jsonString = QString::fromUtf8(data);

    receivedJson = json::parse(jsonString.toStdString());
    if(receivedJson["msgType"] != GETINFO_ACK && receivedJson["errorNo"] != 0){
        return;
    }

    emit dataReady(receivedJson);

}

