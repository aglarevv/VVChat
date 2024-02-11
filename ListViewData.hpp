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
    void setPixmapList(QList<QPixmap> pixmapList){
        m_headList = pixmapList;
    }
    void addName(QString name){
        m_nameList.push_back(name);
    }
    void addPixmap(QPixmap pixmap){
        m_headList.push_back(pixmap);
    }
    QStringList getNameList(){
        return m_nameList;
    }
    QList<QPixmap> getHeadList(){
        return m_headList;
    }
    QString getName(){
        return m_nameList.front();
    }
    QPixmap getHead(){
        return m_headList.front();
    }
private:
    QStringList m_nameList;
    QList<QPixmap> m_headList;

};

#endif // LISTVIEWDATA_H
