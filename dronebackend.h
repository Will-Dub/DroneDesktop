#ifndef DRONEBACKEND_H
#define DRONEBACKEND_H

#include <QObject>
#include "droneworker.h"
#include <QThread>
#include <QDebug>

class DroneBackend : public QObject
{
    Q_OBJECT
public:
    explicit DroneBackend(QObject* parent = nullptr);
    ~DroneBackend();
    Q_INVOKABLE QString processRequest(const QString &data);

signals:
    void initialize();
    void dataChanged(const QString &newData);

private:
    QThread* m_workerThread;
    DroneWorker* m_worker;

};

#endif // DRONEBACKEND_H
