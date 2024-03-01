#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtmaterialtabs.h"
#include "qtmaterialbadge.h"
#include "qtmaterialavatar.h"
#include "ListViewData.h"
#include "qtmaterialappbar.h"
#include "qtmaterialiconbutton.h"
#include "lib/qtmaterialtheme.h"
#include "qtmaterialdrawer.h"
#include "qtmaterialfab.h"
#include "tcpconnect.h"
#include "user.h"
#include "qtmaterialdialog.h"
#include "qtmaterialtextfield.h"
#include "itemdelegate.h"
#include "flushinfo.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "fontmodel.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QTextBrowser>
#include <QListView>
#include <QBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTextBrowser>
#include <QPalette>
#include <QString>
#include <vector>
#include <iostream>
#include <functional>
#include <QObjectUserData>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QThread>
#include <QDateTime>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QFileDialog>
#include <QTextCharFormat>
#include <QTextStream>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(json js,QWidget *parent = nullptr);
    ~MainWindow();

    bool writeToFile(const QString &filePath, const QString &content);
    bool readFileContent(const QString &filePath, QTextBrowser *textBrowser);
    void groupChat();
public slots:
    void handleDrawer(json js);
    void handleDrawerJs();
    void getInfo(json receivedJson);
    void updateTextBrowser(const QModelIndex &index);
    void onDataReady();
    void sendMsg();
    QString getCurTime();
    void updataInfo();
    //void processPendingDatagrams();
    void currentFormatChanged(const QTextCharFormat &format);
    QString getMessage();
    void receivedMsg();
    void showContextMenu(const QModelIndex &index);
private:
    Ui::MainWindow *ui;
    QWidget *m_mainWidget;
    QHBoxLayout *m_mainLayout;
    QVBoxLayout *dialogWidgetLayout;
    QListView *m_view;
    QtMaterialDrawer *m_drawer;
    QVBoxLayout *m_leftLayout;
    QVector<QString> m_drawerLabels;
    ItemDelegate *m_delegate;                 //委托
    QSortFilterProxyModel* m_proxyModel;
    QtMaterialTabs *tabs;
    QStandardItemModel* itemModel;
    QTextBrowser *browser;
    QLabel* msgTitle;
    QTimer *timer;
    bool state = true;
    FlushInfo* info;
    QTextEdit *textEdit;
    QtMaterialRaisedButton *sendButton;
    json js;
    std::string peerAccount;
    std::string peerName;
    std::string peerState;
    std::string peerGroupAccount;
    json receivedJson;
    QColor color;
    TcpServer *server;
    QToolButton *boldButton;
    QToolButton *ItalicsButton;
    QToolButton *UnderlineButton;
    QToolButton *ColorButton;
    QToolButton *FileButton;
    QToolButton *ClearButton;
    QFontComboBox *fontComboBox;
    QComboBox *sizeComboBox;
    QtMaterialDialog *dialog;
    QLabel *title;
    QtMaterialTextField* accountEdit;
    QtMaterialTextField* nameEdit;
    QtMaterialTextField* descEdit;
    QtMaterialRaisedButton *closeButton;
    std::vector<std::string> vec2;
    bool flag = true;
    bool exit = false;
signals:
    void returnToSignIn();
    void readMsg();
    void dataInfo(json receivedJson);
    //void currentCharFormatChanged(const QTextCharFormat &format);
    void contentRead();
    void hand();
};
#endif // MAINWINDOW_H
