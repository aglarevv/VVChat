#include "mainui.h"
#include "ui_mainui.h"

extern TcpConnect* tcp;
MainUi::MainUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUi)
{

    ui->setupUi(this);
    m_stackWidget = new QStackedWidget(this);
    signin = new SignIn(m_stackWidget);
    m_stackWidget->addWidget(signin);
    signup = new SignUp;
    m_stackWidget->addWidget(signup);
    m_stackWidget->setFixedSize(440,300);

}

MainUi::~MainUi()
{
    delete ui;
}

void MainUi::login()
{
    connect(signin->m_signUp,&QPushButton::clicked,this,&MainUi::regist);
    connect(signin,&SignIn::swichMainWindow,this,&MainUi::showMain);
    m_stackWidget->setCurrentWidget(signin);
}

void MainUi::regist()
{

    m_stackWidget->removeWidget(signup);
    if (signup != nullptr) {
        delete signup;
        signup = nullptr;
    }
    signup = new SignUp;

    m_stackWidget->addWidget(signup);
    connect(signup,&SignUp::switchSignIn,this,[=](){
        m_stackWidget->setCurrentWidget(signin);

    });

    m_stackWidget->setCurrentWidget(signup);

}

void MainUi::showMain()
{

    if(signin->state == true)
    {
        m_mainWindow = new MainWindow(signin->getSendData());
        connect(m_mainWindow,&MainWindow::returnToSignIn,this,[=](){
            m_mainWindow->close();
            this->show();
        });
        this->hide();
        m_mainWindow->show();
    }
}

