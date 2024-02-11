#include "mainwindow.h"
#include "signin.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SignIn s;

    s.setFixedSize(440,300);  // 设置窗口大小

    // 计算屏幕中央的位置
    int screenWidth = QApplication::desktop()->width();
    int screenHeight = QApplication::desktop()->height();
    int x = (screenWidth - s.width()) / 2;
    int y = (screenHeight - s.height()) / 2;

    // 设置窗口位置
    s.move(x, y);
    s.show();


    return a.exec();
}
