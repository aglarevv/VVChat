#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(json js,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_mainWidget(new QWidget)
    , m_mainLayout(new QHBoxLayout)
    , m_view(new QListView)
    , m_drawer(new QtMaterialDrawer)
{
    ui->setupUi(this);

    TcpConnect* tcp = TcpConnect::instance();
    this->setContentsMargins(0,0,0,0);
    //移动窗口到屏幕中央
    int x = (QApplication::desktop()->width() - width()) / 2.5;
    int y = (QApplication::desktop()->height() - height()) / 2.5;
    move(x, y);
    //主窗口设置
    m_mainWidget->setParent(this);
    m_mainWidget->setLayout(m_mainLayout);
    m_mainWidget->setStyleSheet("QWidget { background: white; }");
    m_mainWidget->setFixedSize(800,500);
    m_mainWidget->setContentsMargins(0,0,0,0);

    this->setFixedSize(800,500);
    QWidget *leftWidget = new QWidget(m_mainWidget);
    m_leftLayout = new QVBoxLayout;
    leftWidget->setFixedSize(190,500);
    leftWidget->setLayout(m_leftLayout);
    leftWidget->setStyleSheet("QWidget { background:white;}");
    leftWidget->setContentsMargins(0,0,0,0);
    m_mainLayout->addWidget(leftWidget);


    //place
    QWidget *place = new QWidget;
    place->setFixedHeight(25);
    m_leftLayout->addWidget(place);
    //AppBar
    QtMaterialAppBar *appbar = new QtMaterialAppBar(leftWidget);
    appbar->setFixedSize(180,50);
    appbar->setBackgroundColor(QColor("pink"));
    QLabel *appbarLabel = new QLabel(QString::fromStdString(js["name"]));
    appbarLabel->setAttribute(Qt::WA_TranslucentBackground);
    appbarLabel->setForegroundRole(QPalette::WindowText);
    appbarLabel->setContentsMargins(6, 0, 0, 0);

    QPalette palette = appbarLabel->palette();
    palette.setColor(appbarLabel->foregroundRole(), Qt::white);
    appbarLabel->setPalette(palette);
    appbarLabel->setFont(QFont("Roboto", 18, QFont::Normal));

    QtMaterialFloatingActionButton *button = new QtMaterialFloatingActionButton(QIcon(":/navigation/thirdParty/material/lib/include/icons/navigation/svg/production/ic_menu_24px.svg"));
    button->setParent(leftWidget);
    button->setMini(true);
    appbar->appBarLayout()->addWidget(button);
    button->setOffset(130,6);
    button->setRole(Material::Role::Primary);
    appbar->appBarLayout()->addWidget(appbarLabel);
    appbar->appBarLayout()->addStretch(1);
    m_leftLayout->addWidget(appbar);

    //Drawer
    m_drawer->setParent(m_mainWidget);
    m_drawer->setClickOutsideToClose(true);
    m_drawer->setOverlayMode(true);
    QVBoxLayout *drawerLayout = new QVBoxLayout;
    m_drawer->setDrawerLayout(drawerLayout);
    m_drawerLabels = {"添加好友","添加群组","创建群组"};
    QVector<QString>::iterator it;
    for (it = m_drawerLabels.begin(); it != m_drawerLabels.end(); ++it) {
        QtMaterialRaisedButton *btn = new QtMaterialRaisedButton(*it);
        btn->setRole(Material::Role::Primary);
        btn->setOverlayStyle(Material::OverlayStyle::GrayOverlay);
        btn->setHaloVisible(false);
        btn->setFixedHeight(30);
        drawerLayout->addWidget(btn);
        connect(btn,&QPushButton::clicked,this,std::bind(&MainWindow::handleDrawer,this,js));
    }
    drawerLayout->addStretch(3);
    m_drawer->setContentsMargins(10, 0, 0, 0);
    QtMaterialRaisedButton *returnSignIn = new QtMaterialRaisedButton("返回登录");
    returnSignIn->setRole(Material::Role::Secondary);
    returnSignIn->setOverlayStyle(Material::OverlayStyle::TintedOverlay);
    returnSignIn->setHaloVisible(false);
    returnSignIn->setFixedHeight(30);
    drawerLayout->addWidget(returnSignIn);
    connect(returnSignIn,&QPushButton::clicked,this,[=](){
        TcpConnect* tcp = TcpConnect::instance();
        json request;
        request["msgType"] = LOGINOUT;
        request["account"] = js["account"];
        std::string str = request.dump();
        tcp->getSocket()->write(str.c_str(),strlen(str.c_str())+1);
        emit returnToSignIn();
    });
    connect(button,&QPushButton::pressed,m_drawer,&QtMaterialDrawer::openDrawer);
    connect(returnSignIn,&QPushButton::pressed,m_drawer,&QtMaterialDrawer::closeDrawer);

    //Tabs
    QtMaterialTabs *tabs = new QtMaterialTabs(leftWidget);
    tabs->addTab("friends");
    tabs->addTab("group");
    tabs->setFixedSize(180,60);
    m_leftLayout->addWidget(tabs);
    connect(tabs,&QtMaterialTabs::currentChanged,this,&MainWindow::w3);

    //QListView
    m_view->setFixedWidth(180);
    QWidget* w3 = new QWidget(leftWidget);
    QVBoxLayout *w3Layout = new QVBoxLayout;
    w3->setLayout(w3Layout);

    UserListModel userlistModel;
    if(js.contains("friends")){
        m_userList.clear();
        std::vector<std::string> vec = js["friends"];
        for(std::string& str : vec){
            json js = json::parse(str);
            User user;
            user.setName(js["name"]);
            user.setName(js["account"]);
            user.setState(js["state"]);
            m_userList.push_back(user);
        }
        userlistModel.setUserList(m_userList);

        m_view->setModel(&userlistModel);
        m_view->show();
    }else{
        m_view->setModel(&userlistModel);
        m_view->show();
    }
    m_leftLayout->addWidget(m_view);

    //rightWidget
    QWidget *rightWidget = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightWidget->setLayout(rightLayout);
    rightWidget->setFixedSize(600,500);
    rightWidget->setStyleSheet("QWidget { background:white;}");
    rightWidget->setContentsMargins(0,0,0,0);
    m_mainLayout->addWidget(rightWidget);
    //showMsg
    QWidget *msgWidget = new QWidget(rightWidget);
    QVBoxLayout *msgLayout = new QVBoxLayout;
    msgWidget->setLayout(msgLayout);
    rightLayout->addWidget(msgWidget);
    rightLayout->setContentsMargins(0,0,0,0);

    QTextBrowser *browser = new QTextBrowser(msgWidget);
    browser->setFixedSize(570,400);
    browser->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    msgLayout->addWidget(browser);

    browser->setText("<h1>Hello, World!</h1>"
                     "<p>This is a <b>rich text</b> example.</p>"
                     "<p>Click <a href=\"https://www.qt.io/\">here</a> for Qt official website.</p>"
                     "<img src=\":/path/to/your/image.png\" width=\"100\" height=\"100\">");
    //browser->setHtml(richText);

    //编辑信息

    QTextEdit *textEdit = new QTextEdit(msgWidget);
    textEdit->setFixedSize(570,100);
    textEdit->setAlignment(Qt::AlignLeft);
    msgLayout->addWidget(textEdit);

    QtMaterialRaisedButton *sendButton = new QtMaterialRaisedButton("发送");
    sendButton->setRole(Material::Role::Default);
    sendButton->setOverlayStyle(Material::OverlayStyle::TintedOverlay);
    sendButton->setHaloVisible(false);
    sendButton->setFixedSize(100,30);

    QSpacerItem *spacer = new QSpacerItem(5, 5);
    rightLayout->addItem(spacer);

    msgLayout->addWidget(sendButton);
    msgLayout->setAlignment(sendButton, Qt::AlignBottom| Qt::AlignRight);




}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::w3()
{


}

void MainWindow::handleDrawer(json js)
{
    // 获取发送信号的对象
    QObject *senderObject = QObject::sender();
    // 检查是否是 QtMaterialRaisedButton
    QtMaterialRaisedButton *clickedButton = qobject_cast<QtMaterialRaisedButton*>(senderObject);
    // 如果是 QtMaterialRaisedButton，则可以获取按钮的文本
    QString buttonText = clickedButton->text();
        //注册对话框
        QtMaterialDialog *dialog = new QtMaterialDialog;
        dialog->setParent(this);
        QWidget *dialogWidget = new QWidget;
        //ui->setupUi(dialogWidget);
        QVBoxLayout *dialogWidgetLayout = new QVBoxLayout;
        dialogWidget->setStyleSheet("QWidget { background: white; }");
        dialogWidget->setLayout(dialogWidgetLayout);
        QLabel *title = new QLabel;
        title->setText(buttonText);
        title->setStyleSheet("QLabel {\
                                font-family: \"Microsoft YaHei\";\
                                font-size: 22px;\
                                font-weight: bold;\
                                color:black;\
                                border:none;}");
        dialogWidgetLayout->addWidget(title);
        dialogWidgetLayout->setAlignment(title, Qt::AlignTop| Qt::AlignCenter);
        QtMaterialTextField* accountEdit = new QtMaterialTextField;
        QtMaterialTextField* nameEdit = new QtMaterialTextField;
        QtMaterialTextField* descEdit = new QtMaterialTextField;
        if(buttonText != "创建群组"){
            accountEdit->setLabel("账号");
            if(buttonText == "添加群组"){
                accountEdit->setPlaceholderText("群号是15位数字");
            }else{
                accountEdit->setPlaceholderText("账号是11位数字");
            }
            accountEdit->setTextColor(QColor(192,192,192));
            dialogWidgetLayout->setAlignment(accountEdit, Qt::AlignCenter);
            dialogWidgetLayout->addWidget(accountEdit);
        }else{
            nameEdit->setLabel("群名称");
            nameEdit->setPlaceholderText("最长不能超过50字");
            nameEdit->setTextColor(QColor(192,192,192));
            dialogWidgetLayout->setAlignment(nameEdit, Qt::AlignCenter);

            QSpacerItem *spacer = new QSpacerItem(5, 40);
            dialogWidgetLayout->addItem(spacer);

            descEdit->setLabel("群描述");
            descEdit->setPlaceholderText("最长不能超过200字");
            descEdit->setTextColor(QColor(192,192,192));
            dialogWidgetLayout->setAlignment(descEdit, Qt::AlignCenter);
            dialogWidgetLayout->addWidget(nameEdit);
            dialogWidgetLayout->addWidget(descEdit);
        }

        QSpacerItem *spacer = new QSpacerItem(5, 80);
        dialogWidgetLayout->addItem(spacer);
        QString btnName;
        if(buttonText != "创建群组"){
            btnName = "发送申请";
        }else{
            btnName = "确认创建";
        }
        QtMaterialRaisedButton *closeButton = new QtMaterialRaisedButton(btnName);
        dialogWidgetLayout->setAlignment(closeButton, Qt::AlignBottom | Qt::AlignCenter);
        dialogWidgetLayout->addWidget(closeButton);
        closeButton->setRole(Material::Role::Primary);
        closeButton->setOverlayStyle(Material::OverlayStyle::TintedOverlay);
        closeButton->setFixedHeight(35);
        QVBoxLayout *dialogLayout = new QVBoxLayout;
        dialog->setWindowLayout(dialogLayout);

        dialogLayout->addWidget(dialogWidget);
        dialogWidget->setMinimumHeight(250);
        dialog->show();
        //信号
        connect(clickedButton, SIGNAL(clicked()), dialog, SLOT(showDialog()));
        connect(closeButton, SIGNAL(clicked()), dialog, SLOT(hideDialog()));
        connect(closeButton,&QPushButton::pressed,this,[=](){
            std::string request;
            request.clear();
            json sendJs;
            if(buttonText == "添加好友"){
                sendJs["msgType"] = FRIEDN_ADD;
                sendJs["userAccount"] = js["account"];
                sendJs["friendAccount"] = accountEdit->text().toUtf8();
            }else if(buttonText == "添加群组"){
                sendJs["msgType"] = GROUP_ADD;
                sendJs["account"] = js["account"];
                sendJs["groupAccount"] = accountEdit->text().toUtf8();
            }else{
                sendJs["msgType"] = GROUP_CREATE;
                sendJs["account"] = js["account"];
                sendJs["groupName"] = nameEdit->text().toUtf8();
                sendJs["groupdesc"] = descEdit->text().toUtf8();
            }
            request = sendJs.dump();
            std::cout << request << "\n";
            TcpConnect* tcp = TcpConnect::instance();
            tcp->getSocket()->flush();
            if(tcp->getSocket()->waitForConnected())
            {
                if((tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1)) != -1){
                    connect(tcp->getSocket(),&QTcpSocket::readyRead,this,&MainWindow::handleDrawerJs);
                }
                else{
                    QMessageBox::information(this, "waring", "send fail",QMessageBox::Ok);
                }

            }

        });

        }

void MainWindow::handleDrawerJs()
{
    json receivedJson;
    receivedJson.clear();
    TcpConnect* tcp = TcpConnect::instance();
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
    if(receivedJson["errorNo"] == 0)
        QMessageBox::information(this, "info", "success",QMessageBox::Ok);
    else
        QMessageBox::information(this, "waring", "add fail",QMessageBox::Ok);
}


