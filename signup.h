#ifndef SIGNUP_H
#define SIGNUP_H

#include "qtmaterialdialog.h"
#include "qtmaterialflatbutton.h"
#include "qtmaterialraisedbutton.h"


#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QDialog>
#include <QTcpSocket>
#include <QSettings>

namespace Ui {
class SignUp;
}

class SignUp : public QWidget
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

private:
    Ui::SignUp *ui;
    QLabel * m_name;
    QLabel * m_password;
    QPushButton * m_signIn;
    QPushButton * m_signUp;
    QLineEdit * m_acc;
    QLineEdit * m_pwd;

signals:
    void switchSignIn();

public slots:
    void signUpResoult();


};


#endif // SIGNUP_H
