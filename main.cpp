#include "mainwindow.h"
#include "signin.h"
#include "tcpconnect.h"
#include "mainui.h"
#include <QApplication>

TcpConnect * tcp = TcpConnect::instance();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    json js;
//    std::string ac = "6";
//    std::string pd = "123";
//    js["account"] = ac;
//    js["password"] = pd;
//    js["msgType"] = LOGIN;
//    std::string request = js.dump();


    std::cout << tcp->getSocket() << std::endl;
//    json receivedJson;
//    if((tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1)) != -1){
//        if (tcp->getSocket()->waitForReadyRead())
//        receivedJson.clear();

//        QByteArray data;
//        data.clear();
//        data = tcp->getSocket()->readAll();

//        QString jsonString = QString::fromUtf8(data);

//        receivedJson = json::parse(jsonString.toStdString());
//    }

    //MainWindow* s = new MainWindow(receivedJson);
    MainUi s;
    tcp->connect();
    s.setFixedSize(440,300);  // 设置窗口大小
    // 计算屏幕中央的位置
    int screenWidth = QApplication::desktop()->width();
    int screenHeight = QApplication::desktop()->height();
//    int x = (screenWidth - s->width()) / 2;
//    int y = (screenHeight - s->height()) / 2;
    int x = (screenWidth - s.width()) / 2;
    int y = (screenHeight - s.height()) / 2;
    // 设置窗口位置
//    s.move(x, y);
//    s.show();
    s.login();

    //s.login();

    s.move(x,y);
    s.show();



    return a.exec();
}
