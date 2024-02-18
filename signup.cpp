#include "signup.h"
#include "signin.h"
#include "ui_signup.h"

#include <QtDebug>

SignUp::SignUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);

    QWidget * mainWidget = new QWidget(this);
    QVBoxLayout * mainLayout = new QVBoxLayout(mainWidget);
    //setLayout(mainLayout);
    mainWidget->setStyleSheet("QWidget { background: white; }");
    mainWidget->setFixedSize(440,300);

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

    //昵称&密码
    QWidget *w2_w3Widget = new QWidget(mainWidget);
    QVBoxLayout *w2_w3Layout = new QVBoxLayout(w2_w3Widget);
    mainLayout->addWidget(w2_w3Widget);

    QWidget *w2 = new QWidget(w2_w3Widget);
    QHBoxLayout * w2Layout = new QHBoxLayout(w2);
    m_name = new QLabel("昵称:",w2);
    m_name->setStyleSheet("QLabel {\
                             font-family: \"Microsoft YaHei\";\
                             font-size: 18px;\
                             color: black;\
                             font-style: normal;\
                             font-weight: bold;}");
    m_name->setFixedHeight(25);
    m_acc = new QLineEdit(w2);
    w2Layout->addWidget(m_name);
    w2Layout->addWidget(m_acc);
    w2_w3Layout->addWidget(w2);

    QWidget *w3 = new QWidget(w2_w3Widget);
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
    //注册
    QWidget *w4 = new QWidget(mainWidget);
    QHBoxLayout *w4Layout = new QHBoxLayout(w4);
    m_signUp = new QPushButton(mainWidget);
    m_signUp->setText("确认注册");
    m_signUp->setMaximumWidth(200);
    w4Layout->addWidget(m_signUp);
    mainLayout->addWidget(w4);

    connect(m_signUp,&QPushButton::pressed,this,&SignUp::signUpResoult);


}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::signUpResoult()
{


    QString name;
    name = m_acc->text();
    QString password;
    password = m_pwd->text();
    qDebug() << name << " " << password;


    json js;
    js["msgType"] = REGISTER;
    js["name"] = name.toUtf8();
    js["password"] = password.toUtf8();
    js["state"] = 0;
    std::string request = js.dump();

    TcpConnect* tcp = TcpConnect::instance();
    tcp->getSocket()->flush();
    if(tcp->getSocket()->waitForConnected())
    {
        if((tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1)) != -1){
            connect(tcp->getSocket(),&QTcpSocket::readyRead,this,&SignUp::handleReadyRead);
            return;
        }
        else{
            QMessageBox::information(this, "waring", "register error",QMessageBox::Ok);
        }

    }


}


void SignUp::handleReadyRead()
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

    if(receivedJson["msgType"] == REGISTER_ACK && receivedJson["errorNo"] == 0){
        //注册对话框
        QtMaterialDialog *dialog = new QtMaterialDialog;
        dialog->setParent(this);
        QWidget *dialogWidget = new QWidget;
        //ui->setupUi(dialogWidget);
        QVBoxLayout *dialogWidgetLayout = new QVBoxLayout;
        dialogWidget->setStyleSheet("QWidget { background: white; }");
        dialogWidget->setLayout(dialogWidgetLayout);
        QLabel *title = new QLabel;
        title->setText("注册成功!");
        title->setStyleSheet("QLabel {\
                                font-family: \"Microsoft YaHei\";\
                                font-size: 22px;\
                                font-weight: bold;\
                                color:black;\
                                border:none;}");
        dialogWidgetLayout->addWidget(title);
        dialogWidgetLayout->setAlignment(title, Qt::AlignTop| Qt::AlignCenter);

        m_name = new QLabel(dialogWidget);

        m_name->setStyleSheet("QLabel {\
                                font-family: \"Microsoft YaHei\";\
                                font-size: 16px;\
                                font-weight: bold;\
                                color:black;\
                                border:none;}");
        QString account;
        account = "你的账号是：";
        m_name->setText(account + QString::fromStdString(receivedJson["account"]));
        dialogWidgetLayout->addWidget(m_name);


        QLabel *label = new QLabel("请保存好您的账号和密码！");
        label->setStyleSheet("QLabel {\
                             font-family: \"Microsoft YaHei\";\
                             font-size: 12px;\
        font-weight: bold;\
        color:red;\
        border:none;}");
        dialogWidgetLayout->addWidget(label);
        dialogWidgetLayout->setAlignment(label, Qt::AlignBottom| Qt::AlignHCenter);

        QtMaterialRaisedButton *closeButton = new QtMaterialRaisedButton("点我返回登录");
        dialogWidgetLayout->setAlignment(closeButton, Qt::AlignBottom | Qt::AlignCenter);
        dialogWidgetLayout->addWidget(closeButton);
        closeButton->setRole(Material::Role::Secondary);
        closeButton->setOverlayStyle(Material::OverlayStyle::TintedOverlay);

        QVBoxLayout *dialogLayout = new QVBoxLayout;
        dialog->setWindowLayout(dialogLayout);

        dialogLayout->addWidget(dialogWidget);
        dialogWidget->setMinimumHeight(220);
        dialog->show();
        //信号
        connect(m_signUp, SIGNAL(clicked()), dialog, SLOT(showDialog()));

        connect(closeButton, SIGNAL(clicked()), dialog, SLOT(hideDialog()));
        connect(closeButton,&QPushButton::clicked,this,[=](){
            emit switchSignIn();

        });
    }
    else{
        QMessageBox::information(this, "waring", "昵称或密码不能为空",QMessageBox::Ok);
    }

}
