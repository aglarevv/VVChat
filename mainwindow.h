#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtmaterialtabs.h"
#include "qtmaterialbadge.h"
#include "qtmaterialavatar.h"
#include "ListViewData.h"
#include "qtmaterialappbar.h"
#include "qtmaterialiconbutton.h"
#include "lib/qtmaterialtheme.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QTextBrowser>
#include <QListView>
#include <QBoxLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTextBrowser>
#include <QPalette>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void w1(QString str,QWidget* parent = nullptr,bool layoutStyle = 0);
    void w2();
    void w3();
    void updataView(QStringList str, QPixmap pixmap);
private:
    Ui::MainWindow *ui;
    QWidget *m_mainWidget;
    QVBoxLayout *m_mainLayout;
    QListView *m_view;

};
#endif // MAINWINDOW_H
