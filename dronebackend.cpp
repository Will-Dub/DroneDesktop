#include "dronebackend.h"

DroneBackend::DroneBackend(QObject *parent) :
    QObject(parent),
    m_status("Disconnected"),
    m_gamepadAxisInput(6), // 0-5
    m_gamepadButtonInput(16) // 0-15
{
    // Usb worker
    m_usbWorker = new UsbWorker();
    m_usbThread = new QThread(this);
    m_usbWorker->moveToThread(m_usbThread);

    // Connect start and finish
    connect(m_usbThread, &QThread::finished, m_usbWorker, &QObject::deleteLater);

    connect(m_usbThread, &QThread::started, m_usbWorker, [this]() {
        qDebug() << "Worker thread started";
    });

    // Backend to worker
    connect(this, &DroneBackend::doUsbConnect,
            m_usbWorker, &UsbWorker::connectToUsb);

    connect(this, &DroneBackend::doUsbDisconnect,
            m_usbWorker, &UsbWorker::disconnectUsb);

    connect(this, &DroneBackend::doUsbWriteData,
            m_usbWorker, &UsbWorker::writeData);

    connect(this, &DroneBackend::doUsbRefreshList,
            m_usbWorker, &UsbWorker::usbRefreshList);

    connect(this, &DroneBackend::doUsbAutoconnect,
            m_usbWorker, &UsbWorker::usbAutoconnect);

    // Worker to backend
    connect(m_usbWorker, &UsbWorker::connectionStatusChanged,
            this, &DroneBackend::onConnectionStatusChanged);

    connect(m_usbWorker, &UsbWorker::usbListChanged,
            this, &DroneBackend::onUsbListChanged);

    connect(m_usbWorker, &UsbWorker::newPacketReceived,
            this, &DroneBackend::onNewPacketReceived);

    m_usbThread->start();
    emit doUsbRefreshList();

    // Gamepad worker
    m_gamepadWorker = new GamepadWorker();
    m_gamepadThread = new QThread(this);
    m_gamepadWorker->moveToThread(m_gamepadThread);

    // Connect start and finish
    connect(m_gamepadThread, &QThread::finished, m_gamepadWorker, &QObject::deleteLater);
    connect(m_gamepadThread, &QThread::started, m_gamepadWorker, [this]() {
        qDebug() << "Gamepad worker thread started";
    });

    // Backend to gamepadWorker
    connect(this, &DroneBackend::doGamepadConnect,
            m_gamepadWorker, &GamepadWorker::connectToGamepad);

    connect(this, &DroneBackend::doGamepadDisconnect,
            m_gamepadWorker, &GamepadWorker::disconnectGamepad);

    connect(this, &DroneBackend::doGamepadRefreshList,
            m_gamepadWorker, &GamepadWorker::gamepadRefreshList);

    connect(this, &DroneBackend::doGamepadAutoconnect,
            m_gamepadWorker, &GamepadWorker::gamepadAutoconnect);

    // GamepadWorker to backend
    connect(m_gamepadWorker, &GamepadWorker::gamepadConnectionStatusChanged,
            this, &DroneBackend::onGamepadConnectionStatusChanged);

    connect(m_gamepadWorker, &GamepadWorker::gamepadListChanged,
            this, &DroneBackend::onGamepadListChanged);

    connect(m_gamepadWorker, &GamepadWorker::buttonPressed,
            this, &DroneBackend::onButtonPressed);

    connect(m_gamepadWorker, &GamepadWorker::buttonReleased,
            this, &DroneBackend::onButtonReleased);

    connect(m_gamepadWorker, &GamepadWorker::axisChanged,
            this, &DroneBackend::onAxisChanged);

    m_gamepadThread->start();
    emit doGamepadRefreshList();

    // Set previous configuration
    QSettings settings;

    bool isUsbAutoConnect = settings.value("isUsbAutoConnect", false).toBool();
    bool isGamepadAutoConnect = settings.value("isGamepadAutoConnect", false).toBool();
    m_maxGamepadAxisPercentage = settings.value("maxGamepadAxisPercentage", 100).toInt();

    if(isUsbAutoConnect){
        emit doUsbAutoconnect();
    }

    if(isGamepadAutoConnect){
        emit doGamepadAutoconnect();
    }
}

DroneBackend::~DroneBackend(){
    if (m_usbWorker && m_usbThread) {
        m_usbWorker->stopWorking();

        // tell to quit and wait
        m_usbThread->quit();

        if (!m_usbThread->wait(3000)) {
            qWarning() << "DroneBackend: Worker thread failed to terminate";
            m_usbThread->terminate();
            m_usbThread->wait();
        }

        m_usbThread = nullptr;
        m_usbWorker = nullptr;
    }

    if(m_gamepadWorker && m_gamepadThread){
        // tell to quit and wait
        m_gamepadThread->quit();

        if (!m_gamepadThread->wait(3000)) {
            qWarning() << "DroneBackend: Gamepad worker thread failed to terminate";
            m_gamepadThread->terminate();
            m_gamepadThread->wait(1000);
        }

        m_gamepadThread = nullptr;
        m_gamepadWorker = nullptr;
    }
}

bool DroneBackend::isConnected() const
{
    return m_isUsbConnected;
}

bool DroneBackend::isGamepadConnected() const
{
    return m_isGamepadConnected;
}

void DroneBackend::setIsConnected(bool is_connected)
{
    if (m_isUsbConnected != is_connected) {
        m_isUsbConnected = is_connected;
        emit connectedChanged();
    }
}

QString DroneBackend::status() const
{
    return m_status;
}

QVariantList DroneBackend::usbDevices() const
{
    return m_usbDevices;
}

QVariantList DroneBackend::gamepadDevices() const
{
    return m_gamepadDevices;
}

QVariantList DroneBackend::gamepadAxisInput() const
{
    QVariantList variantList;
    for (int value : m_gamepadAxisInput)
        variantList.append(value);

    return variantList;
}

QVariantList DroneBackend::gamepadButtonInput() const
{
    QVariantList variantList;
    for (bool value : m_gamepadButtonInput)
        variantList.append(value);

    return variantList;
}

bool DroneBackend::isUsbAutoConnect() const
{
    QSettings settings;

    return settings.value("isUsbAutoConnect", false).toBool();
}

bool DroneBackend::isGamepadAutoConnect() const
{
    QSettings settings;

    return settings.value("isGamepadAutoConnect", false).toBool();
}

int DroneBackend::maxGamepadAxisPercentage() const
{
    return m_maxGamepadAxisPercentage;
}

bool DroneBackend::connectToUsb(const QString& portName)
{
    if (!m_isUsbConnected) {
        emit doUsbConnect(portName);
        return true;
    }
    return false;
}

void DroneBackend::connectToGamepad(const int joystickId)
{
    if(m_isGamepadConnected){
        qWarning() << "Gamepad already connected";
        return;
    }

    emit doGamepadConnect(joystickId);
}

void DroneBackend::disconnectUsb()
{
    DataPacket endPacket{1,1,DataPacketType::STOP,{}};
    emit doUsbWriteData(endPacket);

    if(m_isUsbConnected){
        emit doUsbDisconnect();
    }
}

void DroneBackend::disconnectGamepad()
{
    if (!m_isGamepadConnected) {
        qWarning() << "No gamepad connected";
        return;
    }

    emit doGamepadDisconnect();
}

void DroneBackend::sendDataTest()
{
    if(!m_isUsbConnected){
        qDebug() << "DroneBackend: Send data got called but the connection is closed";
        return;
    }

    DataPacket test{1,1,DataPacketType::START,{}};
    emit doUsbWriteData(test);
}

void DroneBackend::setIsUsbAutoConnect(bool isUsbAutoConnect)
{
    QSettings settings;

    settings.setValue("isUsbAutoconnect", isUsbAutoConnect);

    emit isUsbAutoConnectChanged();
}

void DroneBackend::setIsGamepadAutoConnect(bool isGamepadAutoConnect)
{
    QSettings settings;

    settings.setValue("isGamepadAutoconnect", isGamepadAutoConnect);

    emit isGamepadAutoConnectChanged();
}

void DroneBackend::setMaxGamepadAxisPercentage(int maxGamepadAxisPercentage)
{
    if (maxGamepadAxisPercentage < 0 || maxGamepadAxisPercentage > 100) {
        return;
    }

    m_maxGamepadAxisPercentage = maxGamepadAxisPercentage;

    QSettings settings;

    settings.setValue("maxGamepadAxisPercentage", m_maxGamepadAxisPercentage);

    emit maxGamepadAxisPercentageChanged();
}

void DroneBackend::onConnectionStatusChanged(const bool isConnected)
{
    if(isConnected){
        DataPacket startPacket{1,1,DataPacketType::START,{}};
        emit doUsbWriteData(startPacket);

        DataPacket statusPacket{1,2,DataPacketType::STATUS,{}};
        emit doUsbWriteData(statusPacket);
    }

    qDebug() << "DroneBackend: Connection status changed to " << isConnected;
    setIsConnected(isConnected);
    emit doUsbRefreshList();
}

void DroneBackend::onUsbListChanged(const QList<UsbInfo> &usbs)
{
    QVariantList newUsbsList;
    foreach (const UsbInfo &usbInfo, usbs) {
        newUsbsList.append(mapUsbDeviceInfo(usbInfo));
    }

    m_usbDevices = newUsbsList;
    emit usbDevicesChanged();
    qDebug() << "Drone backend: Usb devices updated";
}

void DroneBackend::onStatusUpdate(const QString &newStatus)
{
    if (m_status != newStatus) {
        m_status = newStatus;
        emit statusChanged();
    }
}

void DroneBackend::onNewPacketReceived(const DataPacket &dataPacket)
{
    switch (dataPacket.m_header.type) {
    case DataPacketType::STATUS: {
        StatusData statusData;

        if(!statusData.deserialize(dataPacket.m_data)){
            qCritical() << "Drone Backend: Deserializing the status packet failed";
            return;
        }

        qDebug() << "Drone Backend: Status data received";

        break;
    }
    default:
        qCritical() << "Drone Backend: Unhandled data packet type";

        qDebug() << "_____________________________";
        qDebug() << "Data packet received";
        qDebug() << "Data size: " << dataPacket.m_header.dataSize;
        qDebug() << "Packet id: " << dataPacket.m_header.packetId;
        qDebug() << "Drone id: " << dataPacket.m_header.droneId;
        qDebug() << "Type: " << dataPacket.m_header.type;
        qDebug() << "Data: " << dataPacket.m_data;
        qDebug() << "_____________________________";

        break;
    }
}

void DroneBackend::onRefreshUsbDevices()
{
    emit doUsbRefreshList();
}

void DroneBackend::onRefreshGamepadDevices()
{
    emit doGamepadRefreshList();
}

void DroneBackend::onGamepadConnectionStatusChanged(bool isConnected)
{
    m_isGamepadConnected = isConnected;
    emit gamepadConnectedChanged();
}

void DroneBackend::onGamepadListChanged(const QList<GamepadInfo> &gamepads)
{
    QVariantList newGamepadsList;
    foreach (const GamepadInfo &gamepadInfo, gamepads) {
        newGamepadsList.append(mapGamepadDeviceInfo(gamepadInfo));
    }

    m_gamepadDevices = newGamepadsList;
    emit gamepadDevicesChanged();
    qDebug() << "Drone backend: Gamepad devices updated";
}

void DroneBackend::onButtonPressed(int button)
{
    if(button >= m_gamepadButtonInput.size()){
        qCritical() << "Drone backend: Button id " << button << " outside the allocated range";
        return;
    }

    m_gamepadButtonInput[button] = true;
    emit gamepadButtonInputChanged();
}

void DroneBackend::onButtonReleased(int button)
{
    if(button >= m_gamepadButtonInput.size()){
        qCritical() << "Drone backend: Button id " << button << " outside the allocated range";
        return;
    }

    m_gamepadButtonInput[button] = false;
    emit gamepadButtonInputChanged();
}

void DroneBackend::onAxisChanged(int axis, int value)
{
    float processedValue = 0;

    if(axis >= m_gamepadAxisInput.size()){
        qCritical() << "Drone backend: Axis id outside the allocated range";
        return;
    }

    // Outside the deadzone
    if (abs(value) >= DEADZONE) {
        if (value > 0) {
            processedValue = (value - DEADZONE) / (32767.0f - DEADZONE);
        } else {
            processedValue = (value + DEADZONE) / (32767.0f - DEADZONE);
        }
    }

    processedValue = processedValue * 100.0f;
    processedValue = qMin(processedValue, static_cast<float>(m_maxGamepadAxisPercentage));
    processedValue = qMax(processedValue, static_cast<float>(-m_maxGamepadAxisPercentage));

    m_gamepadAxisInput[axis] = processedValue;
    emit gamepadAxisInputChanged();
}

QVariantMap DroneBackend::mapGamepadDeviceInfo(const GamepadInfo& gamepadInfo)
{
    QVariantMap info;

    info["id"] = static_cast<int>(gamepadInfo.index);
    info["name"] = gamepadInfo.name;
    info["type"] = gamepadInfo.type;
    info["guid"] = gamepadInfo.guid;
    info["isConnected"] = gamepadInfo.isConnected;
    return info;
}

QVariantMap DroneBackend::mapUsbDeviceInfo(const UsbInfo &portInfo)
{
    QVariantMap info;

    info["portName"] = portInfo.portName;
    info["systemLocation"] = portInfo.systemLocation;
    info["manufacturer"] = portInfo.manufacturer;
    info["serialNumber"] = portInfo.serialNumber;

    return info;
}
