#ifndef DRONEBACKEND_H
#define DRONEBACKEND_H

#include <QObject>
#include "droneworker.h"
#include <QThread>
#include <QDebug>

class DroneBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
public:
    explicit DroneBackend(QObject* parent = nullptr);
    ~DroneBackend();

    // Property accessors
    bool isConnected() const;
    void setIsConnected(bool is_connected);
    QString status() const;

    // Methods for QML
    Q_INVOKABLE bool connectToDrone();
    Q_INVOKABLE void disconnectDrone();

signals:
    // Signals for property change
    void connectedChanged();
    void statusChanged();

    // Signals to worker
    void doConnect(const QString& portName);
    void doDisconnect();

public slots:
    // Slots received from worker
    void onConnectionStatusChanged(bool is_connected);
    void onStatusUpdate(const QString &newStatus);

private:
    QThread* m_workerThread;
    DroneWorker* m_worker;

    bool m_is_connected;
    QString m_status;

};

#endif // DRONEBACKEND_H
