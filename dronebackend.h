#ifndef DRONEBACKEND_H
#define DRONEBACKEND_H

#include <QObject>
#include "droneworker.h"
#include <QThread>
#include <QDebug>
#include <QVariantList>

class DroneBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QVariantList usbDevices READ usbDevices NOTIFY usbDevicesChanged)
public:
    explicit DroneBackend(QObject* parent = nullptr);
    ~DroneBackend();

    // Property accessors
    bool isConnected() const;
    void setIsConnected(bool is_connected);
    QString status() const;
    QVariantList usbDevices() const;

    // Methods for QML
    Q_INVOKABLE bool connectToDrone(const QString &portName);
    Q_INVOKABLE void disconnectDrone();
    Q_INVOKABLE void sendDataTest();

signals:
    // Signals for property change
    void connectedChanged();
    void statusChanged();
    void usbDevicesChanged();

    // Signals to worker
    void doConnect(const QString& portName);
    void doDisconnect();
    void doWriteData(const DataPacket &dataPacket);

public slots:
    // Slots received from worker
    void onConnectionStatusChanged(const bool is_connected);
    void onStatusUpdate(const QString &newStatus);
    void onNewPacketReceived(const DataPacket &dataPacket);
    void onRefreshUsbDevices();

private:
    QThread* m_workerThread;
    DroneWorker* m_worker;

    QVariantList m_usbDevices;
    void updateUsbDevices();
    QVariantMap mapDeviceInfo(const QSerialPortInfo &portInfo);

    bool m_is_connected;
    QString m_status;

};

#endif // DRONEBACKEND_H
