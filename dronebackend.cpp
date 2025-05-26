#include "dronebackend.h"

DroneBackend::DroneBackend(QObject *parent) : QObject(parent), m_is_connected(false), m_status("Disconnected") {
    // Usb worker
    m_usbWorker = new DroneWorker();

    m_usbThread = new QThread(this);
    m_usbWorker->moveToThread(m_usbThread);

    // Connect start and finish
    connect(m_usbThread, &QThread::finished, m_usbWorker, &QObject::deleteLater);

    connect(m_usbThread, &QThread::started, m_usbWorker, [this]() {
        qDebug() << "Worker thread started";
    });

    // Connect backend to worker
    connect(this, &DroneBackend::doConnect,
            m_usbWorker, &DroneWorker::connectToDrone);

    connect(this, &DroneBackend::doDisconnect,
            m_usbWorker, &DroneWorker::disconnectDrone);

    connect(this, &DroneBackend::doWriteData,
            m_usbWorker, &DroneWorker::writeData);

    // Connect worker to backend
    connect(m_usbWorker, &DroneWorker::connectionStatusChanged,
            this, &DroneBackend::onConnectionStatusChanged);

    connect(m_usbWorker, &DroneWorker::newPacketReceived,
            this, &DroneBackend::onNewPacketReceived);

    // Start the thread
    m_usbThread->start();

    // Gamepad worker
    /*m_gamepadWorker = new GamepadWorker();

    m_gamepadThread = new QThread(this);
    m_gamepadWorker->moveToThread(m_gamepadThread);

    // Connect start and finish
    connect(m_gamepadThread, &QThread::finished, m_gamepadWorker, &QObject::deleteLater);

    connect(m_gamepadThread, &QThread::started, m_gamepadWorker, [this]() {
        qDebug() << "Worker thread started";
    });

    m_gamepadThread->start();*/

    // Update the list of usb device connected
    updateUsbDevices();
}

DroneBackend::~DroneBackend(){
    if (m_usbWorker && m_usbThread) {
        m_usbWorker->stopWorking();

        // tell to quit and wait
        m_usbThread->quit();

        if (!m_usbThread->wait(3000)) {
            qWarning() << "DroneBackend: Worker thread failed to terminate, forcing termination";
            m_usbThread->terminate();
            m_usbThread->wait();
        }
    }
}

bool DroneBackend::isConnected() const
{
    return m_is_connected;
}

void DroneBackend::setIsConnected(bool is_connected)
{
    if (m_is_connected != is_connected) {
        m_is_connected = is_connected;
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
    if (!m_is_connected) {
        emit doConnect(portName);
        return true;
    }
    return false;
}

void DroneBackend::disconnectDrone()
{
    if(m_is_connected){
        emit doDisconnect();
    }
}

void DroneBackend::sendDataTest()
{
    if(!m_is_connected){
        qDebug() << "DroneBackend: Send data got called but the connection is closed";
        return;
    }

    DataPacket test{1,1,DataPacketType::START,{}};
    emit doWriteData(test);
}

void DroneBackend::onConnectionStatusChanged(const bool is_connected)
{
    if(is_connected){
        DataPacket startPacket{1,1,DataPacketType::START,{}};
        emit doWriteData(startPacket);

        DataPacket statusPacket{1,1,DataPacketType::STATUS,{}};
        emit doWriteData(statusPacket);
    }else{
        DataPacket startPacket{1,1,DataPacketType::STOP,{}};
        emit doWriteData(startPacket);
    }

    qDebug() << "DroneBackend: Connection status changed to " << is_connected;
    setIsConnected(is_connected);
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
    qInfo() << "_____________________________";
    qInfo() << "Data packet received";
    qInfo() << "Data size: " << dataPacket.m_header.dataSize;
    qInfo() << "Packet id: " << dataPacket.m_header.packetId;
    qInfo() << "Drone id: " << dataPacket.m_header.droneId;
    qInfo() << "Type: " << dataPacket.m_header.type;
    qInfo() << "Data: " << dataPacket.m_data;
    qInfo() << "_____________________________";
}

void DroneBackend::onRefreshUsbDevices()
{
    updateUsbDevices();
}

void DroneBackend::onRefreshGamepadDevices()
{
    updateGamepadDevices();
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
