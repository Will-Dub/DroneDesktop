#include "usbworker.h"

UsbWorker::UsbWorker(QObject *parent)
    : QObject{parent}
{
    qDebug() << "UsbWorker: Constructed in thread " << QThread::currentThreadId();
}

UsbWorker::~UsbWorker()
{
    disconnectUsb();
    stopWorking();

    delete m_serialPort;

    qDebug() << "UsbWorker: Destroyed in thread " << QThread::currentThreadId();
}

void UsbWorker::stopWorking()
{
    qDebug() << "UsbWorker: Stopped in thread " << QThread::currentThreadId();
}

void UsbWorker::processReadBuffer()
{
    DataPacket packet = DataPacket::deserialize(m_recvBuffer);

    if(packet.m_isValid){
        // Shrink the buffer
        int totalPacketSize = sizeof(DataPacketHeader) + packet.m_header.dataSize;
        m_recvBuffer.remove(0, totalPacketSize);

        emit newPacketReceived(packet);
        processReadBuffer();
    }
}

void UsbWorker::connectToUsb(const QString &portName)
{
    qDebug() << "UsbWorker: Connect to usb called with port " << portName;

    if(m_serialPort == nullptr){
        m_serialPort = new QSerialPort(this);
    }

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "UsbWorker: Failed to open port: " << m_serialPort->errorString();
        emit connectionStatusChanged(false);
    }

    qDebug() << "UsbWorker: Serial port opened";

    // Call readData function when there is new data
    connect(m_serialPort, &QSerialPort::readyRead, this, &UsbWorker::handleReadData);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &UsbWorker::handleError);

    emit connectionStatusChanged(true);
}

void UsbWorker::disconnectUsb()
{
    disconnect(m_serialPort, &QSerialPort::readyRead, this, &UsbWorker::handleReadData);
    disconnect(m_serialPort, &QSerialPort::errorOccurred, this, &UsbWorker::handleError);
    m_serialPort->close();
    emit connectionStatusChanged(false);
}

void UsbWorker::handleReadData()
{
    qDebug() << "UsbWorker: " << m_serialPort->bytesAvailable() << " bytes available";
    QByteArray received = m_serialPort->readAll();

    qDebug() << "UsbWorker: Received from usb: " << received;
    m_recvBuffer.append(received);
    processReadBuffer();
}

void UsbWorker::handleError(QSerialPort::SerialPortError error) {
    qWarning() << "UsbWorker: Serial port error " << error;
    disconnectUsb();
    emit connectionStatusChanged(false);
}

bool UsbWorker::isUsbConnected(const QSerialPortInfo &serialPortInfo)
{
    if(!m_serialPort || !m_serialPort->isOpen()){
        return false;
    }

    QSerialPortInfo info(*m_serialPort);

    if (info.isNull()) {
        return false;
    }

    if(info.serialNumber() != serialPortInfo.serialNumber()){
        return false;
    }

    return true;
}

void UsbWorker::writeData(const DataPacket &dataPacket)
{
    if(!m_serialPort->isOpen()){
        qWarning() << "UsbWorker: Trying to write while the serial is not openned";
        return;
    }

    QByteArray data = dataPacket.serialize();
    qint64 bytesWritten = m_serialPort->write(data);

    if (bytesWritten == -1) {
        qWarning() << "UsbWorker: Failed to write data:" << m_serialPort->errorString();
        return;
    }

    if (bytesWritten != data.size()) {
        qWarning() << "UsbWorker: Partial write " << data.size()<< "/" << bytesWritten;
    }

    m_serialPort->flush();
}

void UsbWorker::usbRefreshList()
{
    QList<UsbInfo> newList;
    const auto serialPortInfoList = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &serialPortInfo : serialPortInfoList) {
        UsbInfo info;
        info.portName = serialPortInfo.portName();
        info.systemLocation = serialPortInfo.systemLocation();
        info.manufacturer = serialPortInfo.manufacturer();
        info.serialNumber = serialPortInfo.serialNumber();
        info.isConnected = isUsbConnected(serialPortInfo);

        newList.append(info);
    }

    if(newList.size() != m_availableUsb.size()){
        m_availableUsb = newList;
        emit usbListChanged(m_availableUsb);
    } else {
        // Check if any gamepad changed
        bool changed = false;
        for (int i = 0; i < newList.size(); ++i) {
            if (i >= m_availableUsb.size() ||
                newList[i].serialNumber != m_availableUsb[i].serialNumber ||
                newList[i].isConnected != m_availableUsb[i].isConnected) {
                changed = true;
                break;
            }
        }

        if (changed) {
            m_availableUsb = newList;
            emit usbListChanged(m_availableUsb);
        }
    }
}

void UsbWorker::usbAutoconnect()
{
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    if(!serialPortInfos.empty()){
        const auto name = serialPortInfos.first().portName();
        connectToUsb(name);
    }
    else{
        qDebug() << "UsbWorker: No device connected, cannot autoconnect";
    }
}
