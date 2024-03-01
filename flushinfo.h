#ifndef FLUSHINFO_H
#define FLUSHINFO_H

#include "thirdParty/json.hpp"
using json = nlohmann::json;
#include <QThread>
#include <QObject>
#include <QTimer>
#include <QTcpSocket>

class FlushInfo : public QThread
{
    Q_OBJECT
public:
    FlushInfo();
    FlushInfo(json js,QObject* parent = nullptr);
    void timerout();
signals:
    void dataReady(const json &data);
private:
    json js;
QTimer timer;

protected:
    void run() override;


public slots:
    void fetchDataFromServer();

};

#endif // FLUSHINFO_H
