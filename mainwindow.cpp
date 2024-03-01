#include "mainwindow.h"
#include "./ui_mainwindow.h"

extern TcpConnect* tcp;
MainWindow::MainWindow(json js,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_mainWidget(new QWidget)
    , m_mainLayout(new QHBoxLayout)
    , m_view(new QListView)
    , m_drawer(new QtMaterialDrawer)
{
    ui->setupUi(this);

    server = new TcpServer(this);
    this->js = js;
    std::cout << js<< std::endl;
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
        connect(btn,&QPushButton::pressed,this,std::bind(&MainWindow::handleDrawer,this,js));
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
        exit = true;
        json request;
        request["msgType"] = LOGINOUT;
        request["account"] = js["account"];
        std::string str = request.dump();
        disconnect(timer, &QTimer::timeout, this, &MainWindow::updataInfo);
        disconnect(tcp->getSocket(),&QTcpSocket::readyRead,this,&MainWindow::receivedMsg);
        tcp->getSocket()->write(str.c_str(),strlen(str.c_str())+1);
        emit returnToSignIn();
    });
    connect(button,&QPushButton::clicked,m_drawer,&QtMaterialDrawer::openDrawer);
    connect(returnSignIn,&QPushButton::clicked,m_drawer,&QtMaterialDrawer::closeDrawer);

    timer = new QTimer(this);
    //Tabs
    tabs = new QtMaterialTabs(leftWidget);
    tabs->addTab("friends");
    tabs->addTab("group");
    tabs->setFixedSize(180,60);
    m_leftLayout->addWidget(tabs);

    //QListView
    m_view->setFixedWidth(180);
    itemModel = new QStandardItemModel;
    if(this->js.contains("friends")){
        std::vector<std::string> vec = this->js["friends"];
        for(std::string& str : vec){
            QStandardItem *item = new QStandardItem;
            json friendJson = json::parse(str);
            User user;
            user.name = friendJson["name"];
            user.account = friendJson["account"];
            user.state = friendJson["state"];
            if(user.state == "online"){
                item->setData(ONLINE,Qt::UserRole);
            }else{
                item->setData(OFFLINE,Qt::UserRole);
            }
            user.flag = true;
            item->setData(QVariant::fromValue(user),Qt::UserRole+1);//整体存取
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            itemModel->appendRow(item);
        }
    }
        m_delegate = new ItemDelegate(this);
        m_view->setItemDelegate(m_delegate);       //为视图设置委托
        m_view->setSpacing(2);
        m_proxyModel = new QSortFilterProxyModel(m_view);
        m_proxyModel->setSourceModel(itemModel);
        m_proxyModel->setFilterRole(Qt::UserRole);
        m_proxyModel->setDynamicSortFilter(true);
        m_view->setModel(m_proxyModel);
        m_view->setDragEnabled(false);

        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


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
    msgWidget->setContentsMargins(0,0,0,0);
    msgTitle = new QLabel(this);
    msgTitle->setFixedSize(100,20);
    msgLayout->addWidget(msgTitle);

    browser = new QTextBrowser(msgWidget);
    browser->setFixedSize(580,280);
    browser->setAlignment( Qt::AlignLeft);



    msgLayout->addWidget(browser);
    rightLayout->addWidget(msgWidget);
    connect(m_view, &QListView::clicked, this, &MainWindow::updateTextBrowser);


        QFrame* window  = new QFrame(msgWidget);
        window->setFrameShape(QFrame::Box);

        // 创建水平布局
        QHBoxLayout *layout = new QHBoxLayout(window);
        layout->setContentsMargins(0,0,0,0);
        window->setFixedSize(580,25);
        // 字体设置
        fontComboBox = new QFontComboBox(window);
        layout->setContentsMargins(0,0,0,0);
        layout->addWidget(fontComboBox);


        // 字体大小
        sizeComboBox = new QComboBox();
        QStringList sizeList;
        for (int i = 8; i <= 24; i += 2) {
            sizeList << QString::number(i);
        }
        sizeComboBox->addItems(sizeList);

        layout->addWidget(sizeComboBox);

        // Tool 按钮
        boldButton = new QToolButton();
        boldButton->setText("Bold");
        boldButton->setCheckable(true);
        layout->addWidget(boldButton);
        ItalicsButton = new QToolButton();
        ItalicsButton->setText("Italics");
        ItalicsButton->setCheckable(true);
        layout->addWidget(ItalicsButton);
        UnderlineButton = new QToolButton();
        UnderlineButton->setText("Underline");
        UnderlineButton->setCheckable(true);
        layout->addWidget(UnderlineButton);
        ColorButton = new QToolButton();
        ColorButton->setText("Color");
        layout->addWidget(ColorButton);
        FileButton = new QToolButton();
        FileButton->setText("File");
        layout->addWidget(FileButton);
        ClearButton = new QToolButton();
        ClearButton->setText("Clear");
        layout->addWidget(boldButton);
        layout->addWidget(ItalicsButton);
        layout->addWidget(UnderlineButton);
        layout->addWidget(ColorButton);
        layout->addWidget(FileButton);
        layout->addWidget(ClearButton);

    msgLayout->addWidget(window);
    //编辑信息
    textEdit = new QTextEdit(msgWidget);
    textEdit->setFixedSize(580,100);
    textEdit->setAlignment(Qt::AlignLeft);
    msgLayout->addWidget(textEdit);
    textEdit->setFocus();

    sendButton = new QtMaterialRaisedButton("发送");
    sendButton->setRole(Material::Role::Default);
    sendButton->setOverlayStyle(Material::OverlayStyle::TintedOverlay);
    sendButton->setHaloVisible(false);
    sendButton->setFixedSize(100,30);
    connect(sendButton,&QPushButton::pressed,this,&MainWindow::sendMsg);

    QSpacerItem *spacer = new QSpacerItem(5, 5);

    msgLayout->addWidget(sendButton);
    msgLayout->setAlignment(sendButton, Qt::AlignBottom| Qt::AlignRight);
    msgLayout->addItem(spacer);

    connect(fontComboBox,&QFontComboBox::currentFontChanged,this,[=](const QFont &f){
           textEdit->setCurrentFont(f);
           textEdit->setFocus();
       });
    void (QComboBox:: * cbxSingal)(const QString &text) = &QComboBox::currentIndexChanged;
      connect(sizeComboBox,cbxSingal,this,[=](const QString &text){
          bool conversionOk;
          double fontSize = text.toDouble(&conversionOk);
          if (conversionOk && fontSize > 0) {
              textEdit->setFontPointSize(fontSize);
              textEdit->setFocus();
          } else {
              qDebug() << "fonts size\n";
          }
      });
      connect(boldButton,&QToolButton::clicked,this,[=](bool checked){
            if(checked)
            {
                textEdit->setFontWeight(QFont::Bold);
            }
            else
            {
               textEdit->setFontWeight(QFont::Normal);
            }
            textEdit->setFocus();
        });
      connect(ItalicsButton,&QToolButton::clicked,this,[=](bool checked){
              textEdit->setFontItalic(checked);
              textEdit->setFocus();
          });
      connect(UnderlineButton,&QToolButton::clicked,this,[=](bool checked){
              textEdit->setFontUnderline(checked);
              textEdit->setFocus();
          });
      connect(ColorButton,&QToolButton::clicked,this,[=](){
             color = QColorDialog::getColor(color,this); //color对象可以在widget.h中定义私有成员
              if(color.isValid())
              {
                  textEdit->setTextColor(color);
                  textEdit->setFocus();
              }
          });


      connect(FileButton,&QToolButton::clicked,this,&MainWindow::onDataReady);
      connect(ClearButton,&QToolButton::clicked,[=](){
              browser->clear();
              QString folderPath = "/home/aglarevv/work/VVChat/msg/" + QString::fromStdString(js["account"]);
              QString path = folderPath + QDir::separator() + QString::fromStdString(peerAccount) + ".ini";
              if (QFile::remove(path)) {
                      qDebug() << "File deleted: " << path;
                  } else {
                      qDebug() << "Error deleting file: " << path;
                  }

      });
      connect(tabs,&QtMaterialTabs::currentChanged,this,&MainWindow::updataInfo, Qt::UniqueConnection);
      connect(textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(QTextCharFormat)));
      connect(tcp->getSocket(),&QTcpSocket::readyRead,this,&MainWindow::receivedMsg);
//    info = new FlushInfo(js,this);
//    QObject::connect(info, &FlushInfo::dataReady, this, &MainWindow::getInfo,Qt::QueuedConnection);
//    info->start();
    //getInfo();
    connect(m_view, &QListView::doubleClicked, this, &MainWindow::showContextMenu);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updataInfo);
    //timer->start(3000);
      browser->clear();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::getInfo(json receivedJson)
{

//std::cout << "getInfo info : " << receivedJson;

    QItemSelection currentSelection = m_view->selectionModel()->selection();
        itemModel->clear();
        if(tabs->currentIndex() == 0){
            if(receivedJson.contains("friends")){
                std::vector<std::string> vec = receivedJson["friends"];
                for(std::string& str : vec){
                    QStandardItem *item = new QStandardItem;
                    json friendJson = json::parse(str);
                    User user;
                    user.name = friendJson["name"];
                    user.account = friendJson["account"];
                    user.state = friendJson["state"];
                    if(user.state == "online"){
                        item->setData(ONLINE,Qt::UserRole);
                    }else{
                        item->setData(OFFLINE,Qt::UserRole);
                    }
                    user.flag = true;
                    item->setData(QVariant::fromValue(user),Qt::UserRole+1);//整体存取
                    itemModel->appendRow(item);
                }
            }
        }
        else if(tabs->currentIndex() == 1){
            if(receivedJson.contains("groups")){
                std::vector<std::string> vec = receivedJson["groups"];
                for(std::string& group : vec){
                    QStandardItem *item = new QStandardItem;
                    json groupjs = json::parse(group);
                    User allGroup;
                    allGroup.flag = false;
                    allGroup.groupAccount = groupjs["groupAccount"];
                    allGroup.groupName = groupjs["groupName"];
                    allGroup.groupDesc = groupjs["groupDesc"];
                    vec2 = groupjs["users"].get<std::vector<std::string>>();
                    for(std::string& user : vec2){
                        json userjs = json::parse(user);
                        User user2;
                        user2.name = userjs["name"];
                        user2.account = userjs["account"];
                        user2.state = userjs["state"];
                        user2.role = userjs["role"];
                        item->setData(QVariant::fromValue(user2),Qt::UserRole+1);//整体存取
                        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                        //itemModel->appendRow(item);
                    }
                    item->setData(QVariant::fromValue(allGroup),Qt::UserRole+1);//整体存取
                    itemModel->appendRow(item);
                }
            }
        }
        m_view->selectionModel()->select(currentSelection, QItemSelectionModel::Select);

}

bool MainWindow::writeToFile(const QString &filePath, const QString &content)
{
    if(content.isEmpty())
        return false;
    QFile file(filePath);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out.setAutoDetectUnicode(true);
        out << content;
        file.close();
        return true;
    } else {
        std::cout << "file open error\n";
    }
    return false;
}

bool MainWindow::readFileContent(const QString &filePath, QTextBrowser *textBrowser)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString fileContent = in.readAll();
        file.close();
        textBrowser->setHtml(fileContent);
        //emit contentRead();
        return true;
    } else {
    }
    return false;
}

void MainWindow::updateTextBrowser(const QModelIndex &index)
{

    if (index.isValid()) {
        // 获取点击的项
        QModelIndex sourceIndex = m_proxyModel->mapToSource(index);
        QStandardItem *item = itemModel->itemFromIndex(sourceIndex);
        //QStandardItem *item = itemModel->itemFromIndex(index);
        if (item) {
            // 获取存储的用户数据
            QVariant userData = item->data(Qt::UserRole + 1);
            if (userData.isValid()) {
                User clickedUser = userData.value<User>();
                if((peerAccount != clickedUser.account) || (peerGroupAccount != clickedUser.groupAccount))
                    browser->clear();

                // 转换为 User 类型
                peerName = clickedUser.name;
                peerGroupAccount = clickedUser.groupAccount;
                peerAccount = clickedUser.account;
                peerState = clickedUser.state;
                QString path;
                QString folderPath = "/home/aglarevv/work/VVChat/msg/" + QString::fromStdString(js["account"]);
                QDir folderDir(folderPath);
                if (!folderDir.exists()) {
                    if (!folderDir.mkpath(".")) {
                        QMessageBox::critical(nullptr, "Error", "Failed to create folder.");
                        return;
                    }
                }
                    if(tabs->currentIndex() == 0){
                        disconnect(sendButton,&QPushButton::pressed,this,&MainWindow::groupChat);
                        msgTitle->setText(QString::fromStdString(peerName));
                        msgTitle->setStyleSheet("QLabel {\
                                                font-family: \"Microsoft YaHei\";\
                                                font-size: 14px;\
                                color: black;\
                        font-style: normal;\
                        font-weight: bold;}");
                    path = folderPath + QDir::separator() + QString::fromStdString(peerAccount) + ".ini";
                    connect(sendButton,&QPushButton::pressed,this,&MainWindow::sendMsg);
                }else{
                    disconnect(sendButton,&QPushButton::pressed,this,&MainWindow::sendMsg);
                    msgTitle->setText(QString::fromStdString(clickedUser.groupName));
                    msgTitle->setStyleSheet("QLabel {\
                                            font-family: \"Microsoft YaHei\";\
                                            font-size: 14px;\
                            color: black;\
                    font-style: normal;\
                    font-weight: bold;}");
             path = folderPath + QDir::separator() + QString::fromStdString(peerGroupAccount) + ".ini";
             connect(sendButton,&QPushButton::pressed,this,&MainWindow::groupChat);
            }



            readFileContent(path,browser);
            // 将滚动条设置到最下方
            QScrollBar *scrollBar = browser->verticalScrollBar();
            scrollBar->setValue(scrollBar->maximum());
            }
        }
    }
}


void MainWindow::showContextMenu(const QModelIndex &index){
    // 获取点击位置的项索引
    if (index.isValid()) {
        // 获取点击的项
        QModelIndex sourceIndex = m_proxyModel->mapToSource(index);
        QStandardItem *item = itemModel->itemFromIndex(sourceIndex);
            // 创建上下文菜单
            QMenu contextMenu;

            // 添加操作到菜单
            QAction *action = new QAction("Show Dialog", this);
            contextMenu.addAction(action);

            // 连接操作的槽函数，用于在点击时弹出对话框
            connect(action, &QAction::triggered, this, [=]() {
                // 获取点击的项
                QModelIndex sourceIndex = m_proxyModel->mapToSource(index);
                QStandardItem *item = itemModel->itemFromIndex(sourceIndex);


                    // 弹出对话框，你可以根据 userData 的内容定制对话框的显示
                    QMessageBox::information(this, "Item Information", "User Data: ");


            });

        }
}


void MainWindow::onDataReady()
{
    if(peerAccount != "")
    {
        server->show();
    }else{
        QMessageBox::warning(this,tr("选择用户"),tr("请先从用户列表选择要传送的用户!"),QMessageBox::Ok);
    }
}

void MainWindow::groupChat(){
    QString str = getMessage();
    json data;
    data["msgType"] = GROUP_CHAT;
    data["name"] = js["name"];
    data["account"] = js["account"];
    data["groupAccount"] = peerGroupAccount;
    data["message"] = str.toUtf8();
    data["time"] = getCurTime().toUtf8();
    std::string request = data.dump();
    if(textEdit->toPlainText() == ""){
        return;
    }
    if(tcp->getSocket() != nullptr)
    {
        // tcp->getSocket()->flush();
        if(tcp->getSocket()->waitForConnected())
        {
            if(peerGroupAccount != "")
            {
                if((tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1)) == -1){
                    std::cout << "chat error\n";
                }
            }else{
                return;
            }
        }
        QString msg;
        msg = textEdit->toHtml();

        textEdit->clear();
        textEdit->setFocus();
        QString time = getCurTime();
        std::string name = js["name"];
        browser->setTextColor(Qt::blue);
        browser->append("[" + QString::fromStdString(name)+"] " + time);
        browser->setTextColor(Qt::black);
        browser->append(msg);

        QString folderPath = "/home/aglarevv/work/VVChat/msg/" + QString::fromStdString(js["account"]);
        QDir folderDir(folderPath);
        if (!folderDir.exists()) {
            if (!folderDir.mkpath(".")) {
                QMessageBox::critical(nullptr, "Error", "Failed to create folder.");
                return;
            }
        }

        QString path = folderPath + QDir::separator() + QString::fromStdString(peerGroupAccount) + ".ini";
        writeToFile(path,browser->toHtml());
    }
}



void MainWindow::sendMsg()
{
    QString str = getMessage();
    json data;
    data["msgType"] = ONE_CHAT;
    data["name"] = js["name"];
    data["account"] = js["account"];
    data["sendToAccount"] = peerAccount;
    data["message"] = str.toUtf8();
    data["time"] = getCurTime().toUtf8();
    std::string request = data.dump();
    if(textEdit->toPlainText() == ""){
        return;
    }
    if(tcp->getSocket() != nullptr)
    {
        // tcp->getSocket()->flush();
        if(tcp->getSocket()->waitForConnected())
        {  
            if(peerAccount != "")
            {
                if((tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1)) == -1){
                    std::cout << "chat error\n";
                }
            }else{
                return;
            }
            QString msg;
            msg = textEdit->toHtml();

            textEdit->clear();
            textEdit->setFocus();
            QString time = getCurTime();
            std::string name = js["name"];
            browser->setTextColor(Qt::blue);
            browser->append("[" + QString::fromStdString(name)+"] " + time);
            browser->setTextColor(Qt::black);
            browser->append(msg);

            QString folderPath = "/home/aglarevv/work/VVChat/msg/" + QString::fromStdString(js["account"]);
            QDir folderDir(folderPath);
            if (!folderDir.exists()) {
                if (!folderDir.mkpath(".")) {
                    QMessageBox::critical(nullptr, "Error", "Failed to create folder.");
                    return;
                }
            }

            QString path = folderPath + QDir::separator() + QString::fromStdString(peerAccount) + ".ini";
             writeToFile(path,browser->toHtml());
        }
    }

}

void MainWindow::receivedMsg(){

    json receivedJson;
    QByteArray data;
    bool show = true;
    data = tcp->getSocket()->readAll();

    QString jsonString = QString::fromUtf8(data);
    if(!exit){
        receivedJson = json::parse(jsonString.toStdString());
    }
    //tcp->getSocket()->flush();
    //int type = receivedJson["msgType"];
    std::cout << "recimsg: " << receivedJson << "\n";

    if(js.contains("offlineMessage")){
        QString folderPath = "/home/aglarevv/work/VVChat/msg/" + QString::fromStdString(js["account"]);
        QDir folderDir(folderPath);
        if (!folderDir.exists()) {
            if (!folderDir.mkpath(".")) {
                QMessageBox::critical(nullptr, "Error", "Failed to create folder.");
                return;
            }
        }
        std::vector<std::string> vec = js["offlineMessage"];
        QString Msg;
        json forjs;
        for(std::string& msg : vec){
            forjs = json::parse(msg);
            if(ONE_CHAT == forjs["msgtype"]){
                QString account = QString::fromStdString(forjs["account"]);
                QString time = QString::fromStdString(forjs["time"]);
                QString name = QString::fromStdString(forjs["name"]);
                QString message = QString::fromStdString(forjs["message"]);
                Msg = "[" + name + "] " + time + "\n" + message;
                qDebug() << Msg << " :Msg\n";
            }
            else {
                //                            cout << "群消息[" << js["groupid"].get<int>() << "]: " << js["time"].get<string>()
                //                                << " [" << js["id"] << "]" << js["name"].get<string>()
                //                                << " said: " << js["msg"].get<string>() << endl;
            }
        }
        QString path = folderPath + QDir::separator() + QString::fromStdString(forjs["account"]) + ".ini";
        QFile file(path);
        if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out.setAutoDetectUnicode(true);
            out << Msg;
            file.close();
        } else {
            std::cout << "file open error\n";
        }
        if(js["account"] != peerAccount){
            show = false;
        }else{
            //readFileContent(path,browser);
        }
    }

    if(receivedJson["msgType"] == ONE_CHAT){
        std::string name = receivedJson["name"];
        std::string message = receivedJson["message"];
        std::string time = receivedJson["time"];
        if(receivedJson.contains("account")){
            if(receivedJson["account"] != peerAccount)
            {
                QString folderPath = "/home/aglarevv/work/VVChat/msg/" + QString::fromStdString(js["account"]);
                QDir folderDir(folderPath);
                if (!folderDir.exists()) {
                    if (!folderDir.mkpath(".")) {
                        QMessageBox::critical(nullptr, "Error", "Failed to create folder.");
                        return;
                    }
                }
                QString path = folderPath + QDir::separator() + QString::fromStdString(receivedJson["account"]) + ".ini";
                QString str ="[" + QString::fromStdString(name) + "] " + QString::fromStdString(time);
                str += "      " + QString::fromStdString(receivedJson["message"]) + "\n";

                QFile file(path);
                if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)) {
                    QTextStream out(&file);
                    out.setCodec("UTF-8");
                    out.setAutoDetectUnicode(true);
                    out << str;
                    file.close();
                } else {
                    std::cout << "file open error\n";
                }
                show = false;
            }
        }

        if(show){
            browser->setTextColor(Qt::blue);
            browser->setCurrentFont(QFont("Times New Roman",12));
            browser->append("[" + QString::fromStdString(name)+"] " + QString::fromStdString(time));
            browser->setTextColor(Qt::black);
            //browser->setCurrentFont(QFont("Times New Roman",12));
            browser->append(QString::fromStdString(message));
            QString folderPath = "/home/aglarevv/work/VVChat/msg/" + QString::fromStdString(js["account"]);
            QDir folderDir(folderPath);
            if (!folderDir.exists()) {
                if (!folderDir.mkpath(".")) {
                    QMessageBox::critical(nullptr, "Error", "Failed to create folder.");
                    return;
                }
            }

            QString path = folderPath + QDir::separator() + QString::fromStdString(peerAccount) + ".ini";

            writeToFile(path,browser->toHtml());
        }


    }else if(receivedJson["msgType"] == GROUP_CHAT){
        std::string name = receivedJson["name"];
        std::string message = receivedJson["message"];
        std::string time = receivedJson["time"];
        if(receivedJson.contains("account")){
            if(receivedJson["groupAccount"] != peerGroupAccount){
                QString folderPath = "/home/aglarevv/work/VVChat/msg/" + QString::fromStdString(js["account"]);
                QDir folderDir(folderPath);
                if (!folderDir.exists()) {
                    if (!folderDir.mkpath(".")) {
                        QMessageBox::critical(nullptr, "Error", "Failed to create folder.");
                        return;
                    }
                }
                QString path = folderPath + QDir::separator() + QString::fromStdString(receivedJson["groupAccount"]) + ".ini";
                QString str ="[" + QString::fromStdString(name) + "] " + QString::fromStdString(time);
                str += "      " + QString::fromStdString(receivedJson["message"]) + "\n";

                QFile file(path);
                if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append)) {
                    QTextStream out(&file);
                    out.setCodec("UTF-8");
                    out.setAutoDetectUnicode(true);
                    out << str;
                    file.close();
                } else {
                    std::cout << "file open error\n";
                }
                show = false;
            }
            if(show){
                browser->setTextColor(Qt::blue);
                browser->setCurrentFont(QFont("Times New Roman",12));
                browser->append("[" + QString::fromStdString(name)+"] " + QString::fromStdString(time));
                browser->setTextColor(Qt::black);
                //browser->setCurrentFont(QFont("Times New Roman",12));
                browser->append(QString::fromStdString(message));
                QString folderPath = "/home/aglarevv/work/VVChat/msg/" + QString::fromStdString(js["account"]);
                QDir folderDir(folderPath);
                if (!folderDir.exists()) {
                    if (!folderDir.mkpath(".")) {
                        QMessageBox::critical(nullptr, "Error", "Failed to create folder.");
                        return;
                    }
                }
                QString path = folderPath + QDir::separator() + QString::fromStdString(peerGroupAccount) + ".ini";
                writeToFile(path,browser->toHtml());
            }
        }
        }else if(receivedJson["msgType"] == GETINFO_ACK && receivedJson["errorNo"] == 0){
            connect(this,&MainWindow::dataInfo,this,std::bind(&MainWindow::getInfo,this,receivedJson));
            emit dataInfo(receivedJson);
        }else if(receivedJson["msgType"] == FRIEND_ADD_ACK || receivedJson["msgType"] == GROUP_ADD_ACK || receivedJson["msgType"] == GROUP_CREATE_ACK ){
            connect(this,&MainWindow::hand,this,&MainWindow::handleDrawerJs);
            emit hand();
        }


}

QString MainWindow::getMessage()
{
    QString msg = textEdit->toPlainText();
//    textEdit->clear();
//    textEdit->setFocus();
    return msg;
}

QString MainWindow::getCurTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // 将日期和时间格式化为字符串
    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
    return formattedDateTime;
}

void MainWindow::updataInfo()
{
    json data;
    data["msgType"] = GETINFO;
    data["account"] = this->js["account"];
    std::string request = data.dump();
    tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1);
}

void MainWindow::currentFormatChanged(const QTextCharFormat &format)
{
    fontComboBox->setCurrentFont(format.font());
    if(format.fontPointSize() < 7) {
        sizeComboBox->setCurrentIndex(3);
    }
    else {
        sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(format.fontPointSize())));
    }
    boldButton->setChecked(format.font().bold());
    ItalicsButton->setChecked(format.font().italic());
    UnderlineButton->setChecked(format.font().underline());
    color = format.foreground().color();

}



void MainWindow::handleDrawer(json js)
{
    // 获取发送信号的对象
    QObject *senderObject = QObject::sender();
    // 检查是否是 QtMaterialRaisedButton
    QtMaterialRaisedButton *clickedButton = qobject_cast<QtMaterialRaisedButton*>(senderObject);
    // 如果是 QtMaterialRaisedButton，则可以获取按钮的文本
    QString buttonText = clickedButton->text();
    if(flag){
        //注册对话框
        dialog = new QtMaterialDialog;
        dialog->setParent(this);
        QWidget *dialogWidget = new QWidget(this);
        dialogWidgetLayout = new QVBoxLayout;
        dialogWidget->setStyleSheet("QWidget { background: white; }");
        dialogWidget->setLayout(dialogWidgetLayout);
        title = new QLabel;

        title->setStyleSheet("QLabel {\
                                font-family: \"Microsoft YaHei\";\
                                font-size: 22px;\
                                font-weight: bold;\
                                color:black;\
                                border:none;}");
        dialogWidgetLayout->addWidget(title);
        dialogWidgetLayout->setAlignment(title, Qt::AlignTop| Qt::AlignCenter);
        accountEdit = new QtMaterialTextField;
        nameEdit = new QtMaterialTextField;
        descEdit = new QtMaterialTextField;

        QString btnName;
        if(buttonText != "创建群组"){
            btnName = "发送申请/关闭";
        }else{
            btnName = "确认创建/关闭";
        }
        closeButton = new QtMaterialRaisedButton(btnName);
        dialogWidgetLayout->setAlignment(closeButton, Qt::AlignBottom | Qt::AlignCenter);

        closeButton->setRole(Material::Role::Primary);
        closeButton->setOverlayStyle(Material::OverlayStyle::TintedOverlay);
        closeButton->setFixedHeight(35);
        QVBoxLayout *dialogLayout = new QVBoxLayout;
        dialog->setWindowLayout(dialogLayout);

        dialogLayout->addWidget(dialogWidget);
        dialogWidget->setMinimumHeight(250);
        flag = false;
    }
        title->setText(buttonText);
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
            dialogWidgetLayout->addWidget(closeButton);
        dialog->show();
        //信号
        connect(clickedButton, SIGNAL(clicked()), dialog, SLOT(showDialog()));
        connect(closeButton, SIGNAL(clicked()), dialog, SLOT(hideDialog()));
        connect(closeButton,&QPushButton::pressed,this,[=](){
            if((buttonText != "创建群组") && accountEdit->text() == ""){
                dialog->hideDialog();
                return;
            }
            if((buttonText == "创建群组") && nameEdit->text() == ""){
                dialog->hideDialog();
                return;
            }
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
                sendJs["groupDesc"] = descEdit->text().toUtf8();
            }
            request = sendJs.dump();


            if(tcp->getSocket()->waitForConnected())
            {
                if((tcp->getSocket()->write(request.c_str(),strlen(request.c_str())+1)) != -1){}
            }
        });

}
void MainWindow::handleDrawerJs()
{
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
    if(receivedJson["errorNo"] == 0){
        QMessageBox::information(this, "info", "success",QMessageBox::Ok);
    }else
        QMessageBox::information(this, "waring", "fail",QMessageBox::Ok);
}




