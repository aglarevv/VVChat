#ifndef USERLISTMODEL_H
#define USERLISTMODEL_H

#include "user.h"

#include <QAbstractListModel>
#include <vector>

class UserListModel : public QAbstractListModel
{
public:


    explicit UserListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        Q_UNUSED(parent);
        return m_userList.size();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || index.row() >= m_userList.size())
            return QVariant();

        const User& user = m_userList[index.row()];


    }

    void setUserList(const std::vector<User>& userList) {
        beginResetModel();
        m_userList = userList;
        endResetModel();
    }

private:
    std::vector<User> m_userList;
};

#endif // USERLISTMODEL_H
