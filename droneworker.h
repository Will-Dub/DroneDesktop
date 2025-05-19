#ifndef DRONEWORKER_H
#define DRONEWORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>

class DroneWorker : public QObject
{
    Q_OBJECT
public:
    explicit DroneWorker(QObject* parent = nullptr);
    ~DroneWorker();

    void stopWorking();

public slots:
    void connectToDrone();
    void disconnectDrone();

signals:
    void connectionStatusChanged(bool connected);
    void statusUpdated(const QString &status);

private:
    bool m_connected;
};

#endif // DRONEWORKER_H
