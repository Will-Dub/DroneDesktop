#include "dronebackend.h"

DroneBackend::DroneBackend(QObject *parent) :
    QObject(parent),
    m_status("Disconnected")
{
    // Init sdl3
    if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) < 0) {
        qCritical() << "SDL_Init Error: " << SDL_GetError();
    }

    // Usb worker
    m_usbWorker = new DroneWorker();
    m_usbThread = new QThread(this);
    m_usbWorker->moveToThread(m_usbThread);

    // Connect start and finish
    connect(m_usbThread, &QThread::finished, m_usbWorker, &QObject::deleteLater);

    connect(m_usbThread, &QThread::started, m_usbWorker, [this]() {
        qDebug() << "Worker thread started";
    });

    // Backend to worker
    connect(this, &DroneBackend::doConnect,
            m_usbWorker, &DroneWorker::connectToDrone);

    connect(this, &DroneBackend::doDisconnect,
            m_usbWorker, &DroneWorker::disconnectDrone);

    connect(this, &DroneBackend::doWriteData,
            m_usbWorker, &DroneWorker::writeData);

    // Worker to backend
    connect(m_usbWorker, &DroneWorker::connectionStatusChanged,
            this, &DroneBackend::onConnectionStatusChanged);

    connect(m_usbWorker, &DroneWorker::newPacketReceived,
            this, &DroneBackend::onNewPacketReceived);

    m_usbThread->start();

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

    // GamepadWorker to backend
    connect(m_gamepadWorker, &GamepadWorker::gamepadConnectionStatusChanged,
            this, &DroneBackend::onGamepadConnectionStatusChanged);

    m_gamepadThread->start();

    // Update the devices
    updateUsbDevices();
    updateGamepadDevices();
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
    return m_isConnected;
}

bool DroneBackend::isGamepadConnected() const
{
    return m_isGamepadConnected;
}

void DroneBackend::setIsConnected(bool is_connected)
{
    if (m_isConnected != is_connected) {
        m_isConnected = is_connected;
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

bool DroneBackend::connectToDrone(const QString& portName)
{
    if (!m_isConnected) {
        emit doConnect(portName);
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

void DroneBackend::disconnectDrone()
{
    if(m_isConnected){
        emit doDisconnect();
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
    if(!m_isConnected){
        qDebug() << "DroneBackend: Send data got called but the connection is closed";
        return;
    }

    DataPacket test{1,1,DataPacketType::START,{}};
    emit doWriteData(test);
}

void DroneBackend::onConnectionStatusChanged(const bool isConnected)
{
    if(isConnected){
        DataPacket startPacket{1,1,DataPacketType::START,{}};
        emit doWriteData(startPacket);

        DataPacket statusPacket{1,1,DataPacketType::STATUS,{}};
        emit doWriteData(statusPacket);
    }else{
        DataPacket startPacket{1,1,DataPacketType::STOP,{}};
        emit doWriteData(startPacket);
    }

    qDebug() << "DroneBackend: Connection status changed to " << isConnected;
    setIsConnected(isConnected);
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
    qDebug() << "_____________________________";
    qDebug() << "Data packet received";
    qDebug() << "Data size: " << dataPacket.m_header.dataSize;
    qDebug() << "Packet id: " << dataPacket.m_header.packetId;
    qDebug() << "Drone id: " << dataPacket.m_header.droneId;
    qDebug() << "Type: " << dataPacket.m_header.type;
    qDebug() << "Data: " << dataPacket.m_data;
    qDebug() << "_____________________________";
}

void DroneBackend::onRefreshUsbDevices()
{
    updateUsbDevices();
}

void DroneBackend::onRefreshGamepadDevices()
{
    updateGamepadDevices();
}

void DroneBackend::onGamepadConnectionStatusChanged(bool isConnected)
{
    m_isGamepadConnected = isConnected;
    emit gamepadConnectedChanged();
}

void DroneBackend::updateUsbDevices()
{
    QVariantList newDevices;

    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        newDevices.append(mapDeviceInfo(portInfo));
    }

    if (newDevices != m_usbDevices) {
        m_usbDevices = newDevices;
        emit usbDevicesChanged();
        qDebug() << "USB devices updated. Found" << m_usbDevices.size() << "devices";
    }
}

void DroneBackend::updateGamepadDevices()
{
    QVariantList newGamepads;
    int count = SDL_NumJoysticks();
    for (int i = 0; i < count; ++i) {
        if (SDL_IsGameController(i)) {
            newGamepads.append(mapGamepadDeviceInfo(i));
        }
    }
    if (newGamepads != m_gamepadDevices) {
        m_gamepadDevices = newGamepads;
        emit gamepadDevicesChanged();
        qDebug() << "Gamepad devices updated. Found" << m_gamepadDevices.size() << "devices";
    }
}

QVariantMap DroneBackend::mapGamepadDeviceInfo(SDL_JoystickID joystickId)
{
    QVariantMap deviceInfo;
    deviceInfo["id"] = static_cast<int>(joystickId);
    deviceInfo["name"] = QString(SDL_GameControllerNameForIndex(joystickId));
    deviceInfo["type"] = gamepadTypeToString(SDL_GameControllerTypeForIndex(joystickId));
    return deviceInfo;
}

QString DroneBackend::gamepadTypeToString(SDL_GameControllerType type) {
    switch (type) {
    case SDL_CONTROLLER_TYPE_XBOX360:
        return "Xbox 360";
    case SDL_CONTROLLER_TYPE_XBOXONE:
        return "Xbox One";
    case SDL_CONTROLLER_TYPE_PS3:
        return "PlayStation 3";
    case SDL_CONTROLLER_TYPE_PS4:
        return "PlayStation 4";
    case SDL_CONTROLLER_TYPE_PS5:
        return "PlayStation 5";
    case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
        return "Nintendo Switch Pro";
    case SDL_CONTROLLER_TYPE_UNKNOWN:
    default:
        return "Unknown";
    }
}

QVariantMap DroneBackend::mapDeviceInfo(const QSerialPortInfo &portInfo)
{
    QVariantMap deviceInfo;

    deviceInfo["portName"] = portInfo.portName();
    deviceInfo["systemLocation"] = portInfo.systemLocation();
    deviceInfo["manufacturer"] = portInfo.manufacturer();
    deviceInfo["serialNumber"] = portInfo.serialNumber();

    return deviceInfo;
}
