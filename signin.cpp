#include "signin.h"
#include "ui_signin.h"

#include <QDebug>

SignIn::SignIn(QWidget *parent) :
    QWidget(parent),
    m_mainWindow(new MainWindow(this)),
    ui(new Ui::SignIn)
{
    ui->setupUi(this);

    m_stackWidget = new QStackedWidget(this);
    //QVBoxLayout *stackWidgetLayout = new QVBoxLayout;
    //m_stackWidget->setLayout(stackWidgetLayout);
    QWidget * mainWidget = new QWidget;
    mainWidget->setStyleSheet("QWidget { background: white; }");
    QVBoxLayout * mainLayout = new QVBoxLayout(mainWidget);
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
    QWidget *w2_w3Widget = new QWidget(mainWidget);
    QVBoxLayout *w2_w3Layout = new QVBoxLayout(w2_w3Widget);
    mainLayout->addWidget(w2_w3Widget);

    QWidget *w2 = new QWidget(mainWidget);
    QHBoxLayout * w2Layout = new QHBoxLayout(w2);
    m_account = new QLabel("账号:",w2);
    m_account->setStyleSheet("QLabel {\
                             font-family: \"Microsoft YaHei\";\
                             font-size: 18px;\
                             color: black;\
                             font-style: normal;\
                             font-weight: bold;}");
    m_account->setFixedHeight(20);
    m_acc = new QLineEdit(w2);
    w2Layout->addWidget(m_account);
    w2Layout->addWidget(m_acc);
    w2_w3Layout->addWidget(w2);

    QSpacerItem *spacer = new QSpacerItem(5, 1);
    mainLayout->addItem(spacer);

    QWidget *w3 = new QWidget(mainWidget);
    QHBoxLayout * w3Layout = new QHBoxLayout(w3);
    m_password = new QLabel("密码:",w3);
    m_password->setStyleSheet("QLabel {\
                             font-family: \"Microsoft YaHei\";\
                             font-size: 18px;\
                             color: black;\
                             font-style: normal;\
                             font-weight: bold;}");
    m_password->setFixedHeight(20);
    m_pwd = new QLineEdit(w3);
    m_pwd->setEchoMode(QLineEdit::Password);
    w3Layout->addWidget(m_password);
    w3Layout->addWidget(m_pwd);
    w2_w3Layout->addWidget(w3);
    //登录和注册
    QWidget *w4 = new QWidget(mainWidget);
    QHBoxLayout *w4Layout = new QHBoxLayout(w4);
    m_signIn = new QtMaterialRaisedButton(mainWidget);
    m_signIn->setRole(Material::Role::Primary);
    m_signIn->setOverlayStyle(Material::OverlayStyle::TintedOverlay);
    m_signIn->setText("登录");
    w4Layout->addWidget(m_signIn);
    m_signIn->setMaximumHeight(30);

    m_signUp = new QtMaterialRaisedButton(mainWidget);
    m_signUp->setRole(Material::Role::Primary);
    m_signUp->setOverlayStyle(Material::OverlayStyle::TintedOverlay);
    m_signUp->setText("注册");
    m_signUp->setMaximumHeight(30);
    w4Layout->addWidget(m_signUp);
    mainLayout->addWidget(w4);
    //页面切换
    m_signUpWindow = new SignUp;
    m_stackWidget->addWidget(mainWidget);
    m_stackWidget->addWidget(m_signUpWindow);
    m_stackWidget->setFixedSize(440,300);

    //信号
    connect(m_signIn,&QPushButton::clicked,this,&SignIn::loginConfirm);
    connect(m_signUp,&QPushButton::clicked,this,&SignIn::signUpConfirm);
    connect(m_signUpWindow,&SignUp::switchSignIn,this,[=](){
        m_stackWidget->setCurrentIndex(0);
    });
    connect(m_mainWindow,&MainWindow::returnToSignIn,this,[=](){
        m_mainWindow->hide();
        this->show();
    });

}

SignIn::~SignIn()
{
    delete ui;
}

void SignIn::loginConfirm()
{
    //login logic
    this->hide();
    m_mainWindow->show();
//    this->setFixedSize(800,500);
//    m_stackWidget->setFixedSize(800,500);
//    int x = (QApplication::desktop()->width() - width()) / 2;
//    int y = (QApplication::desktop()->height() - height()) / 2;
//    move(x, y);
//    m_stackWidget->setCurrentIndex(2);
}

void SignIn::signUpConfirm()
{
    m_stackWidget->setCurrentIndex(1);
}

