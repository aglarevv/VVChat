#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_mainWidget(new QWidget)
    , m_mainLayout(new QVBoxLayout)
    , m_view(new QListView)
{
    ui->setupUi(this);


    m_mainWidget->setParent(this);
    m_mainWidget->setLayout(m_mainLayout);

    QWidget* appbarWidget = new QWidget(m_mainWidget);
    QVBoxLayout* appbarLayout = new QVBoxLayout;
    appbarLayout->addWidget(appbarWidget);
    QtMaterialAppBar *appbar = new QtMaterialAppBar;
    QLabel *label = new QLabel("Inbox");
    label->setAttribute(Qt::WA_TranslucentBackground);
    label->setForegroundRole(QPalette::Foreground);
    label->setContentsMargins(6, 0, 0, 0);

    QPalette palette = label->palette();
    palette.setColor(label->foregroundRole(), Qt::white);
    label->setPalette(palette);
    label->setFont(QFont("Roboto", 18, QFont::Normal));

    QtMaterialIconButton *button = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "menu"));
    button->setIconSize(QSize(24, 24));
    appbar->appBarLayout()->addWidget(button);
    appbar->appBarLayout()->addWidget(label);
    appbar->appBarLayout()->addStretch(1);
    button->setColor(Qt::white);
    button->setFixedWidth(42);


    QWidget *canvas = new QWidget;
    canvas->setStyleSheet("QWidget { background: white; }");
    appbarLayout->addWidget(canvas);

    appbarLayout->setContentsMargins(20, 20, 20, 20);

    appbarLayout = new QVBoxLayout;
    canvas->setLayout(appbarLayout);
    canvas->setMaximumHeight(300);
    appbarLayout->addWidget(appbar);
    appbarLayout->addStretch(1);
    m_mainLayout->addWidget(appbarWidget);

    //w1(":/thirdParty/material/lib/include/icons/image/svg/production/ic_audiotrack_24px.svg",m_mainWidget);
    //w2();
    //w3();

    QTextBrowser *browser = new QTextBrowser;









}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::w1(QString str,QWidget* parent,bool layoutStyle)
{
    QWidget *w1 = new QWidget(parent);
    w1->setMaximumSize(150,60);
    QBoxLayout *layout = nullptr;
    if(layoutStyle == 0)
    {
        layout = new QHBoxLayout;
    }
    else
    {
        layout = new QVBoxLayout;
    }
    w1->setLayout(layout);
    QtMaterialAvatar *avatar = new QtMaterialAvatar(QImage(str));
    QtMaterialBadge *headImg = new QtMaterialBadge;
    layout->addWidget(avatar);
    avatar->setSize(60);
    headImg->setParent(avatar);

    headImg->setText("3");
    QLabel *selfName = new QLabel("example");
    layout->addWidget(selfName);
    layout->addWidget(w1);
    m_mainLayout->addWidget(w1);
}

void MainWindow::w2()
{
    QtMaterialTabs *tabs = new QtMaterialTabs;
    tabs->addTab("friends");
    tabs->addTab("group");
    tabs->setFixedSize(220,60);
    m_mainLayout->addWidget(tabs);
    connect(tabs,&QtMaterialTabs::currentChanged,this,&MainWindow::w3);

}

void MainWindow::w3()
{

    m_view->setFixedWidth(220);
    QWidget* w3 = new QWidget(m_mainWidget);
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
    m_mainLayout->addWidget(m_view);
}

void MainWindow::updataView(QStringList str, QPixmap pixmap)
{

}

