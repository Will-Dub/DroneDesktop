#include "dronebackend.h"

DroneBackend::DroneBackend(QObject *parent) : QObject(parent), m_is_connected(false), m_status("Disconnected") {
    m_worker = new DroneWorker();

    m_workerThread = new QThread(this);
    m_worker->moveToThread(m_workerThread);

    // Connect start and finish
    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);

    connect(m_workerThread, &QThread::started, m_worker, [this]() {
        qDebug() << "Worker thread started";
    });

    // Connect backend to worker
    connect(this, &DroneBackend::doConnect,
            m_worker, &DroneWorker::connectToDrone);

    connect(this, &DroneBackend::doDisconnect,
            m_worker, &DroneWorker::disconnectDrone);

    connect(this, &DroneBackend::doWriteData,
            m_worker, &DroneWorker::writeData);

    // Connect worker to backend
    connect(m_worker, &DroneWorker::connectionStatusChanged,
            this, &DroneBackend::onConnectionStatusChanged);

    connect(m_worker, &DroneWorker::newPacketReceived,
            this, &DroneBackend::onNewPacketReceived);

    // Start the thread
    m_workerThread->start();

    // Update the list of usb device connected
    updateUsbDevices();
}

DroneBackend::~DroneBackend(){
    if (m_worker && m_workerThread) {
        m_worker->stopWorking();

        // tell to quit and wait
        m_workerThread->quit();

        if (!m_workerThread->wait(3000)) {
            qWarning() << "DroneBackend: Worker thread failed to terminate, forcing termination";
            m_workerThread->terminate();
            m_workerThread->wait();
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

QVariantMap DroneBackend::mapDeviceInfo(const QSerialPortInfo &portInfo)
{
    QVariantMap deviceInfo;

    deviceInfo["portName"] = portInfo.portName();
    deviceInfo["systemLocation"] = portInfo.systemLocation();
    deviceInfo["manufacturer"] = portInfo.manufacturer();
    deviceInfo["serialNumber"] = portInfo.serialNumber();

    return deviceInfo;
}
