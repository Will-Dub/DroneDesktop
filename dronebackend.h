#ifndef DRONEBACKEND_H
#define DRONEBACKEND_H

#include <QObject>
#include "droneworker.h"
#include "gamepadworker.h"
#include <QThread>
#include <QDebug>
#include <QVariantList>

class DroneBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(bool gamepadConnected READ isGamepadConnected NOTIFY gamepadConnectedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QVariantList usbDevices READ usbDevices NOTIFY usbDevicesChanged)
    Q_PROPERTY(QVariantList gamepadDevices READ gamepadDevices NOTIFY gamepadDevicesChanged)

public:
    explicit DroneBackend(QObject* parent = nullptr);
    ~DroneBackend();

    // Property accessors
    bool isConnected() const;
    bool isGamepadConnected() const;
    void setIsConnected(bool is_connected);
    QString status() const;
    QVariantList usbDevices() const;
    QVariantList gamepadDevices() const;

    // Methods for QML
    Q_INVOKABLE bool connectToDrone(const QString &portName);
    Q_INVOKABLE void connectToGamepad(int joystickId);
    Q_INVOKABLE void disconnectDrone();
    Q_INVOKABLE void disconnectGamepad();
    Q_INVOKABLE void sendDataTest();

signals:
    // Signals for property change
    void connectedChanged();
    void gamepadConnectedChanged();
    void statusChanged();
    void usbDevicesChanged();
    void gamepadDevicesChanged();

    // Signals to worker
    void doConnect(const QString& portName);
    void doDisconnect();
    void doWriteData(const DataPacket &dataPacket);

    // Signals to gamepad worker
    void doGamepadConnect(int deviceIndex);
    void doGamepadDisconnect();
    void doRefreshGamepadList();

public slots:
    // Slots received from worker
    void onConnectionStatusChanged(bool isConnected);
    void onStatusUpdate(const QString &newStatus);
    void onNewPacketReceived(const DataPacket &dataPacket);
    void onRefreshUsbDevices();
    void onRefreshGamepadDevices();

    // Slots received from gamepad worker
    void onGamepadConnectionStatusChanged(bool isConnected);
    void onGamepadListChanged(const QList<GamepadInfo>& gamepads);

private:
    QThread* m_usbThread = nullptr;
    DroneWorker* m_usbWorker = nullptr;
    QThread* m_gamepadThread = nullptr;
    GamepadWorker* m_gamepadWorker = nullptr;

    QVariantList m_usbDevices;
    QVariantList m_gamepadDevices;

    void updateUsbDevices();
    QVariantMap mapDeviceInfo(const QSerialPortInfo &portInfo);
    QVariantMap mapGamepadDeviceInfo(const GamepadInfo& gamepadInfo);

    bool m_isConnected = false;
    bool m_isGamepadConnected = false;
    QString m_status;

    static QString gamepadTypeToString(SDL_GameControllerType type);
};

#endif // DRONEBACKEND_H
