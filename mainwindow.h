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


#include <QMainWindow>
#include <QStackedWidget>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
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


    void w3();

private:
    Ui::MainWindow *ui;
    QWidget *m_mainWidget;
    QHBoxLayout *m_mainLayout;
    QListView *m_view;
    QtMaterialDrawer *m_drawer;

signals:
    void returnToSignIn();
};
#endif // MAINWINDOW_H
