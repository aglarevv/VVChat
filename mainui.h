#ifndef MAINUI_H
#define MAINUI_H

#include "signup.h"
#include "signin.h"
#include "mainwindow.h"

#include <QWidget>
#include <QStackedWidget>

namespace Ui {
class MainUi;
}

class MainUi : public QWidget
{
    Q_OBJECT

public:
    explicit MainUi(QWidget *parent = nullptr);
    ~MainUi();

    void login();
    void regist();
    void showMain();

private:
    Ui::MainUi *ui;
    QStackedWidget *m_stackWidget;
    SignUp* signup;
    SignIn * signin;
    MainWindow* m_mainWindow;
};



#endif // MAINUI_H
