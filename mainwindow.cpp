#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_mainWidget(new QWidget)
    , m_mainLayout(new QHBoxLayout)
    , m_view(new QListView)
    , m_drawer(new QtMaterialDrawer)
{
    ui->setupUi(this);
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
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftWidget->setFixedSize(190,500);
    leftWidget->setLayout(leftLayout);
    leftWidget->setStyleSheet("QWidget { background:white;}");
    leftWidget->setContentsMargins(0,0,0,0);
    m_mainLayout->addWidget(leftWidget);


    //place
    QWidget *place = new QWidget;
    place->setFixedHeight(25);
    leftLayout->addWidget(place);
    //AppBar
    QtMaterialAppBar *appbar = new QtMaterialAppBar(leftWidget);
    appbar->setFixedSize(180,50);
    appbar->setBackgroundColor(QColor("pink"));
    QLabel *appbarLabel = new QLabel("Inbox");
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
    leftLayout->addWidget(appbar);

    //Drawer
    m_drawer->setParent(m_mainWidget);
    m_drawer->setClickOutsideToClose(true);
    m_drawer->setOverlayMode(true);
    QVBoxLayout *drawerLayout = new QVBoxLayout;
    m_drawer->setDrawerLayout(drawerLayout);
    QVector<QString> drawerLabels = {"返回登录"};
    QVector<QString>::iterator it;
    for (it = drawerLabels.begin(); it != drawerLabels.end(); ++it) {
        QLabel *label = new QLabel(*it);
        label->setMinimumHeight(30);
        label->setFont(QFont("Roboto", 10, QFont::Medium));
        drawerLayout->addWidget(label);
    }
    drawerLayout->addStretch(3);
    m_drawer->setContentsMargins(10, 0, 0, 0);
    QPushButton *returnSignIn = new QPushButton;
    returnSignIn->setText("返回登录");
    drawerLayout->addWidget(returnSignIn);
    connect(returnSignIn,&QPushButton::clicked,this,[=](){
        emit returnToSignIn();
    });
    connect(button,&QPushButton::pressed,m_drawer,&QtMaterialDrawer::openDrawer);
    connect(returnSignIn,&QPushButton::pressed,m_drawer,&QtMaterialDrawer::closeDrawer);

    //Tabs
    QtMaterialTabs *tabs = new QtMaterialTabs(leftWidget);
    tabs->addTab("friends");
    tabs->addTab("group");
    tabs->setFixedSize(180,60);
    leftLayout->addWidget(tabs);
    connect(tabs,&QtMaterialTabs::currentChanged,this,&MainWindow::w3);

    //QListView
    m_view->setFixedWidth(180);
    QWidget* w3 = new QWidget(leftWidget);
    QVBoxLayout *w3Layout = new QVBoxLayout;
    w3->setLayout(w3Layout);
    QStandardItemModel *model = new QStandardItemModel;
    ListViewData data;
    QStringList str = {"1","2","3"};
    data.setNameList(str);
    data.addName("4");
    data.addPixmap(QPixmap(":/thirdParty/material/lib/include/icons/image/svg/production/ic_add_a_photo_24px.svg"));
    data.addPixmap(QPixmap(":/thirdParty/material/lib/include/icons/image/svg/production/ic_add_a_photo_24px.svg"));
    data.addPixmap(QPixmap(":/thirdParty/material/lib/include/icons/image/svg/production/ic_add_a_photo_24px.svg"));
    data.addPixmap(QPixmap(":/thirdParty/material/lib/include/icons/image/svg/production/ic_add_a_photo_24px.svg"));
    for (int i = 0; i < data.getHeadList().size(); ++i) {
            QStandardItem *item = new QStandardItem(data.getNameList().at(i));
            item->setIcon(QIcon(data.getHeadList().at(i)));
            model->appendRow(item);
        }
    m_view->setModel(model);
    m_view->show();
    leftLayout->addWidget(m_view);

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

    QPushButton *sendButton = new QPushButton(msgWidget);
    sendButton->setText("发送");
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



