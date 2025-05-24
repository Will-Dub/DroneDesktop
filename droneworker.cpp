#include "droneworker.h"

DroneWorker::DroneWorker(QObject *parent)
    : QObject{parent}
{
    qDebug() << "DroneWorker: Constructed in thread " << QThread::currentThreadId();
}

DroneWorker::~DroneWorker()
{
    disconnectDrone();
    stopWorking();

    delete m_serialPort;

    qDebug() << "DroneWorker: Destroyed in thread " << QThread::currentThreadId();
}

void DroneWorker::stopWorking()
{
    qDebug() << "DroneWorker: Stopped in thread " << QThread::currentThreadId();
}

void DroneWorker::processReadBuffer()
{
    DataPacket packet = DataPacket::deserialize(m_recvBuffer);

    if(packet.m_isValid){
        emit newPacketReceived(packet);
    }
}

void DroneWorker::connectToDrone(const QString &portName)
{
    qDebug() << "DroneWorker: Connect to drone called with port " << portName;

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
        qDebug() << "DroneWorker: Failed to open port: " << m_serialPort->errorString();
        emit connectionStatusChanged(false);
    }

    qDebug() << "DroneWorker: Serial port opened";

    // Call readData function when there is new data
    connect(m_serialPort, &QSerialPort::readyRead, this, &DroneWorker::handleReadData);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &DroneWorker::handleError);

    emit connectionStatusChanged(true);
}

void DroneWorker::disconnectDrone()
{
    disconnect(m_serialPort, &QSerialPort::readyRead, this, &DroneWorker::handleReadData);
    disconnect(m_serialPort, &QSerialPort::errorOccurred, this, &DroneWorker::handleError);
    m_serialPort->close();
    emit connectionStatusChanged(false);
}

void DroneWorker::handleReadData()
{
    qDebug() << "DroneWorker: " << m_serialPort->bytesAvailable() << " bytes available";
    QByteArray received = m_serialPort->readAll();

    qDebug() << "DroneWorker: Received from usb: " << received;
    m_recvBuffer.append(received);
    processReadBuffer();
}

void DroneWorker::handleError(QSerialPort::SerialPortError error) {
    qWarning() << "DroneWorker: Serial port error " << error;
    disconnectDrone();
    emit connectionStatusChanged(false);
}

void DroneWorker::writeData(const DataPacket &dataPacket)
{
    if(!m_serialPort->isOpen()){
        qWarning() << "DroneWorker: Trying to write while the serial is not openned";
        return;
    }

    QByteArray data = dataPacket.serialize();
    qint64 bytesWritten = m_serialPort->write(data);

    if (bytesWritten == -1) {
        qWarning() << "DroneWorker: Failed to write data:" << m_serialPort->errorString();
        return;
    }

    if (bytesWritten != data.size()) {
        qWarning() << "DroneWorker: Partial write " << data.size()<< "/" << bytesWritten;
    }

    m_serialPort->flush();

}
