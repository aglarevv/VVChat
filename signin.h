#ifndef SIGNIN_H
#define SIGNIN_H


#include "mainwindow.h"
#include "qtmaterialraisedbutton.h"
#include "tcpconnect.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QString>



/*
    登录界面类
*/

namespace Ui {
class SignIn;
}

class SignIn : public QWidget
{
    Q_OBJECT

public:
    explicit SignIn(QWidget *parent = nullptr);
    ~SignIn();

    MainWindow *m_mainWindow;
    QWidget * m_mainWidget;
    Ui::SignIn *ui;
    QLabel * m_account;
    QLabel * m_password;
    QtMaterialRaisedButton * m_signIn;
    QtMaterialRaisedButton * m_signUp;
    QLineEdit * m_acc;
    QLineEdit * m_pwd;
    bool state = false;
    json data;
signals:
    void swichMainWindow();


public slots:
    void loginConfirm();
    void handleReadyRead();
    json getSendData();
    void setSendData(json receivedJson);
};

#endif // SIGNIN_H
