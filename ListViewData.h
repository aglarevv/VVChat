#ifndef LISTVIEWDATA_H
#define LISTVIEWDATA_H

#include <QStringList>
#include <QPixmap>
#include <QList>

namespace myData {
    class ListViewData;
}

class ListViewData
{
public:
    ListViewData();
    ~ListViewData();
    void setNameList(QStringList nameList);
    void setPixmapList(QList<QPixmap> pixmapList);
    void addName(QString name);
    void addPixmap(QPixmap pixmap);
    QStringList getNameList();
    QList<QPixmap> getHeadList();
    QString getName();
    QPixmap getHead();
private:
    QStringList m_nameList;
    QList<QPixmap> m_headList;

};

#endif // LISTVIEWDATA_H
