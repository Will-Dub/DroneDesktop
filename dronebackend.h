#ifndef DRONEBACKEND_H
#define DRONEBACKEND_H

#include <QObject>
#include "usbworker.h"
#include "gamepadworker.h"
#include <QThread>
#include <QDebug>
#include <QVariantList>
#include <QSettings>
#include <QAbstractListModel>
#include "loglistmodel.h"
#include "componentlistmodel.h"

class DroneBackend : public QObject
{
    const int DEADZONE = 8000;

    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(bool gamepadConnected READ isGamepadConnected NOTIFY gamepadConnectedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QVariantList usbDevices READ usbDevices NOTIFY usbDevicesChanged)
    Q_PROPERTY(QVariantList gamepadDevices READ gamepadDevices NOTIFY gamepadDevicesChanged)
    Q_PROPERTY(QVariantList gamepadAxisInput READ gamepadAxisInput NOTIFY gamepadAxisInputChanged)
    Q_PROPERTY(QVariantList gamepadButtonInput READ gamepadButtonInput NOTIFY gamepadButtonInputChanged)
    Q_PROPERTY(LogListModel* logs READ logs NOTIFY logsChanged)
    Q_PROPERTY(ComponentListModel* components READ components NOTIFY componentsChanged)
    Q_PROPERTY(bool isUsbAutoConnect READ isUsbAutoConnect WRITE setIsUsbAutoConnect NOTIFY isUsbAutoConnectChanged)
    Q_PROPERTY(bool isGamepadAutoConnect READ isGamepadAutoConnect WRITE setIsGamepadAutoConnect NOTIFY isGamepadAutoConnectChanged)
    Q_PROPERTY(int maxGamepadAxisPercentage READ maxGamepadAxisPercentage WRITE setMaxGamepadAxisPercentage NOTIFY maxGamepadAxisPercentageChanged)

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
    QVariantList gamepadAxisInput() const;
    QVariantList gamepadButtonInput() const;
    LogListModel* logs();
    ComponentListModel* components();
    bool isUsbAutoConnect() const;
    bool isGamepadAutoConnect() const;
    int maxGamepadAxisPercentage() const;

    // Methods for QML
    Q_INVOKABLE bool connectToUsb(const QString &portName);
    Q_INVOKABLE void connectToGamepad(int joystickId);
    Q_INVOKABLE void disconnectUsb();
    Q_INVOKABLE void disconnectGamepad();
    Q_INVOKABLE void toggleComponentStatus(int index);
    Q_INVOKABLE void refreshComponentStatus();

    // Setting change
    void setIsUsbAutoConnect(bool isUsbAutoConnect);
    void setIsGamepadAutoConnect(bool isGamepadAutoConnect);
    void setMaxGamepadAxisPercentage(int maxGamepadAxisPercentage);

signals:
    // Signals for property change
    void connectedChanged();
    void gamepadConnectedChanged();
    void statusChanged();
    void usbDevicesChanged();
    void gamepadDevicesChanged();
    void gamepadAxisInputChanged();
    void gamepadButtonInputChanged();
    void logsChanged();
    void componentsChanged();
    void isUsbAutoConnectChanged();
    void isGamepadAutoConnectChanged();
    void maxGamepadAxisPercentageChanged();

    // Signals to usb worker
    void doUsbConnect(const QString& portName);
    void doUsbDisconnect();
    void doUsbWriteData(const DataPacket &dataPacket);
    void doUsbRefreshList();
    void doUsbAutoconnect();

    // Signals to gamepad worker
    void doGamepadConnect(int deviceIndex);
    void doGamepadDisconnect();
    void doGamepadRefreshList();
    void doGamepadAutoconnect();

public slots:
    // Slots received from worker
    void onConnectionStatusChanged(bool isConnected);
    void onUsbListChanged(const QList<UsbInfo>& usbs);
    void onStatusUpdate(const QString &newStatus);
    void onNewPacketReceived(const DataPacket &dataPacket);
    void onRefreshUsbDevices();

    // Slots received from gamepad worker
    void onGamepadConnectionStatusChanged(bool isConnected);
    void onGamepadListChanged(const QList<GamepadInfo>& gamepads);
    void onButtonPressed(int button);
    void onButtonReleased(int button);
    void onAxisChanged(int axis, int value);
    void onRefreshGamepadDevices();

private:
    QThread* m_usbThread = nullptr;
    UsbWorker* m_usbWorker = nullptr;
    QThread* m_gamepadThread = nullptr;
    GamepadWorker* m_gamepadWorker = nullptr;

    QVariantList m_usbDevices;
    QVariantList m_gamepadDevices;

    QVariantMap mapUsbDeviceInfo(const UsbInfo& portInfo);
    QVariantMap mapGamepadDeviceInfo(const GamepadInfo& gamepadInfo);

    LogListModel m_logs{};
    ComponentListModel m_components{};

    QList<int> m_gamepadAxisInput;
    QList<bool> m_gamepadButtonInput;

    QString m_status;
    bool m_isUsbConnected = false;
    bool m_isGamepadConnected = false;
    int m_maxGamepadAxisPercentage = 100;

    static QString gamepadTypeToString(SDL_GameControllerType type);
};

#endif // DRONEBACKEND_H
