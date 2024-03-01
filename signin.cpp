#include "signin.h"
#include "ui_signin.h"

#include <QDebug>
extern TcpConnect* tcp;
SignIn::SignIn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);
    m_mainWidget = new QWidget(this);
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
    m_mainWidget->setFixedSize(440,300);

    //信号
    connect(m_signIn,&QPushButton::clicked,this,&SignIn::loginConfirm);



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

    if(tcp->getSocket() != nullptr)
    {
       // tcp->getSocket()->flush();
        if(tcp->getSocket()->waitForConnected())
        {
            if((tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1)) != -1){
                    connect(tcp->getSocket(),&QTcpSocket::readyRead,this,&SignIn::handleReadyRead);

            }
            else{
                QMessageBox::information(this, "waring", "login error",QMessageBox::Ok);
            }

        }
    }
    else{
        QMessageBox::information(this, "waring", "please check your network!",QMessageBox::Ok);

    }

}


void SignIn::handleReadyRead()
{

    disconnect(tcp->getSocket(), &QTcpSocket::readyRead, this, &SignIn::handleReadyRead);

    json receivedJson;
    receivedJson.clear();

    QByteArray data;
    data.clear();
    data = tcp->getSocket()->readAll();

    QString jsonString = QString::fromUtf8(data);

    receivedJson = json::parse(jsonString.toStdString());
    if(receivedJson["msgType"] == LOGIN_ACK && receivedJson["errorNo"] == 0)
    {
        qDebug() << "one\n";
        setSendData(receivedJson);
        state = true;
        emit swichMainWindow();
    }else{
        std::string temp = receivedJson["errorMsg"];
        QMessageBox::information(this, "waring", QString::fromStdString(temp),QMessageBox::Ok);
    }


}

json SignIn::getSendData()
{
    return data;
}

void SignIn::setSendData(json receivedJson)
{
    data = receivedJson;

}


