#include "itemdelegate.h"


#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>


ItemDelegate::ItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

ItemDelegate::~ItemDelegate()
{

}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{


    if(index.isValid())
    {
        painter->save();
        State status = (State)(index.data(Qt::UserRole).toInt());
        QVariant variant = index.data(Qt::UserRole+1);
        User data = variant.value<User>();

        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth( option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        //QPainterPath画圆角矩形
        const qreal radius = 12;
        QPainterPath path;
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));

        if(option.state.testFlag(QStyle::State_Selected))
        {
            painter->setPen(QPen(Qt::blue));
            painter->setBrush(QColor(229, 241, 255));
            painter->drawPath(path);
        }
        else{
            painter->setPen(QPen(Qt::gray));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }

        //绘制数据位置
        QRect NameRect = QRect(rect.left() +10, rect.top()+10, rect.width()-30, 20);
        QRect circle = QRect(NameRect.right(), rect.top()+10, 10, 10);
        QRect telRect = QRect(rect.left() +10, rect.bottom()-25, rect.width()-10, 20);

//        if(data.state == "online"){
//            painter->setBrush(Qt::green);
//            painter->setPen(QPen(Qt::green));
//        }else{
//            painter->setBrush(Qt::gray);
//            painter->setPen(QPen(Qt::gray));
//        }
        switch (status) {
        case ONLINE:
            painter->setBrush(Qt::green);
            painter->setPen(QPen(Qt::green));
            break;
        case OFFLINE:
            painter->setBrush(Qt::gray);
            painter->setPen(QPen(Qt::gray));
            break;
        }

        if(data.flag){
            painter->drawEllipse(circle);     //画圆圈

            painter->setPen(QPen(Qt::black));
            painter->setFont(QFont("Times", 12, QFont::Bold));
            painter->drawText(NameRect,Qt::AlignLeft,QString::fromStdString(data.name)); //绘制名字
            painter->setPen(QPen(Qt::gray));
            painter->setFont(QFont("Times", 10));
            painter->drawText(telRect,Qt::AlignLeft,"AC:" + QString::fromStdString(data.account)); //绘制账号
        }else{
            painter->setPen(QPen(Qt::black));
            painter->setFont(QFont("Times", 12, QFont::Bold));
            painter->drawText(NameRect,Qt::AlignLeft, QString::fromStdString(data.groupName)); //绘制名字
            painter->setPen(QPen(Qt::gray));
            painter->setFont(QFont("Times", 10));
            painter->drawText(telRect,Qt::AlignLeft,"AC:" + QString::fromStdString(data.groupAccount)); //绘制账号
        }




        painter->restore();
    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(160, 60);
}
