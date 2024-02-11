#include "ListViewData.h"


ListViewData::ListViewData()
{

}

ListViewData::~ListViewData()
{

}

void ListViewData::setNameList(QStringList nameList){
    m_nameList = nameList;
}

void ListViewData::setPixmapList(QList<QPixmap> pixmapList){
    m_headList = pixmapList;
}

void ListViewData::addName(QString name){
    m_nameList.append(name);
}

void ListViewData::addPixmap(QPixmap pixmap){
    m_headList.push_back(pixmap);
}

QStringList ListViewData::getNameList(){
    return m_nameList;
}

QList<QPixmap> ListViewData::getHeadList(){
    return m_headList;
}

QString ListViewData::getName(){
    return m_nameList.front();
}

QPixmap ListViewData::getHead(){
    return m_headList.front();
}
