#include "signin.h"
#include "ui_signin.h"

#include <QDebug>

SignIn::SignIn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);

    m_stackWidget = new QStackedWidget(this);
    //QVBoxLayout *stackWidgetLayout = new QVBoxLayout;
    //m_stackWidget->setLayout(stackWidgetLayout);
    m_mainWidget = new QWidget;
    m_mainWidget->setStyleSheet("QWidget { background: white; }");
    QVBoxLayout * mainLayout = new QVBoxLayout(m_mainWidget);
    //标题
    QLabel *Toptitle = new QLabel("VVChat");
    Toptitle->setStyleSheet("QLabel {\
                         font-family: \"Microsoft YaHei\";\
                         font-size: 24px;\
                         color: black;\
                         font-style: normal;\
                         font-weight: bold;}");
    Toptitle->setMaximumHeight(100);
    Toptitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mainLayout->addWidget(Toptitle);

    //账号和密码
    QWidget *w2_w3Widget = new QWidget(m_mainWidget);
    QVBoxLayout *w2_w3Layout = new QVBoxLayout(w2_w3Widget);
    mainLayout->addWidget(w2_w3Widget);

    QWidget *w2 = new QWidget(m_mainWidget);
    QHBoxLayout * w2Layout = new QHBoxLayout(w2);
    m_account = new QLabel("账号:",w2);
    m_account->setStyleSheet("QLabel {\
                             font-family: \"Microsoft YaHei\";\
                             font-size: 18px;\
                             color: black;\
                             font-style: normal;\
                             font-weight: bold;}");
    m_account->setFixedHeight(25);
    m_acc = new QLineEdit(w2);
    w2Layout->addWidget(m_account);
    w2Layout->addWidget(m_acc);
    w2_w3Layout->addWidget(w2);

    QSpacerItem *spacer = new QSpacerItem(5, 1);
    mainLayout->addItem(spacer);

    QWidget *w3 = new QWidget(m_mainWidget);
    QHBoxLayout * w3Layout = new QHBoxLayout(w3);
    m_password = new QLabel("密码:",w3);
    m_password->setStyleSheet("QLabel {\
                             font-family: \"Microsoft YaHei\";\
                             font-size: 18px;\
                             color: black;\
                             font-style: normal;\
                             font-weight: bold;}");
    m_password->setFixedHeight(25);
    m_pwd = new QLineEdit(w3);
    m_pwd->setEchoMode(QLineEdit::Password);
    w3Layout->addWidget(m_password);
    w3Layout->addWidget(m_pwd);
    w2_w3Layout->addWidget(w3);
    //登录和注册
    QWidget *w4 = new QWidget(m_mainWidget);
    QHBoxLayout *w4Layout = new QHBoxLayout(w4);
    m_signIn = new QtMaterialRaisedButton(m_mainWidget);
    m_signIn->setRole(Material::Role::Primary);
    m_signIn->setOverlayStyle(Material::OverlayStyle::TintedOverlay);
    m_signIn->setText("登录");
    w4Layout->addWidget(m_signIn);
    m_signIn->setMaximumHeight(30);

    m_signUp = new QtMaterialRaisedButton(m_mainWidget);
    m_signUp->setRole(Material::Role::Primary);
    m_signUp->setOverlayStyle(Material::OverlayStyle::TintedOverlay);
    m_signUp->setText("注册");
    m_signUp->setMaximumHeight(30);
    w4Layout->addWidget(m_signUp);
    mainLayout->addWidget(w4);
    //页面切换
    m_signUpWindow = new SignUp;
    m_stackWidget->addWidget(m_mainWidget);
    m_stackWidget->addWidget(m_signUpWindow);
    m_stackWidget->setFixedSize(440,300);

    //信号
    connect(m_signIn,&QPushButton::clicked,this,&SignIn::loginConfirm);
    connect(m_signUp,&QPushButton::clicked,this,&SignIn::signUpConfirm);
    connect(m_signUpWindow,&SignUp::switchSignIn,this,[=](){
        m_stackWidget->setCurrentWidget(m_mainWidget);
    });


}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::loginConfirm()
{



    //login logic
    QString account;
    account = m_acc->text();
    QString password;
    password = m_pwd->text();

    json js;
    js["msgType"] = LOGIN;
    js["account"] = account.toUtf8();
    js["password"] = password.toUtf8();
    std::string request = js.dump();


    TcpConnect* tcp = TcpConnect::instance();

        tcp->getSocket()->flush();
        if(tcp->getSocket()->waitForConnected())
        {
            if((tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1)) != -1){
                connect(tcp->getSocket(),&QTcpSocket::readyRead,this,&SignIn::handleReadyRead);
                return;
            }
            else{
                QMessageBox::information(this, "waring", "login error",QMessageBox::Ok);
            }

        }



}

void SignIn::signUpConfirm()
{
    m_stackWidget->removeWidget(m_signUpWindow);
    if (m_signUpWindow != nullptr) {
        delete m_signUpWindow;
        m_signUpWindow = nullptr;
    }
    m_signUpWindow = new SignUp;
    m_stackWidget->addWidget(m_signUpWindow);
    connect(m_signUpWindow,&SignUp::switchSignIn,this,[=](){
        m_stackWidget->setCurrentWidget(m_mainWidget);
    });
    m_stackWidget->setCurrentWidget(m_signUpWindow);
}

void SignIn::handleReadyRead()
{
    TcpConnect* tcp = TcpConnect::instance();
    json receivedJson;
    receivedJson.clear();

    QByteArray data;
    data.clear();
    data = tcp->getSocket()->readAll();
    QString jsonString = QString::fromUtf8(data);
    try {
        receivedJson = json::parse(jsonString.toStdString());
    } catch(const std::exception& e) {
        std::cout << receivedJson << '\n';
        std::cout << e.what() << '\n';
        return;
    }
    if(receivedJson["msgType"] == LOGIN_ACK && receivedJson["errorNo"] == 0)
    {
        m_mainWindow = new MainWindow(receivedJson,this);
        connect(m_mainWindow,&MainWindow::returnToSignIn,this,[=](){
            m_mainWindow->hide();
            this->show();
        });
        this->hide();
        m_mainWindow->show();
    }
    else
    {
        QMessageBox::information(this, "waring", "Account or Password error",QMessageBox::Ok);
        connect(m_signIn,&QPushButton::clicked,this,&SignIn::loginConfirm);
    }

}

