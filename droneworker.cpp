#include "droneworker.h"

DroneWorker::DroneWorker(QObject *parent)
    : QObject{parent}
{
    qDebug() << "DroneWorker constructed in thread:" << QThread::currentThreadId();
}

DroneWorker::~DroneWorker()
{
    stopWorking();

    qDebug() << "DroneWorker destroyed in thread:" << QThread::currentThreadId();
}

void DroneWorker::stopWorking()
{
    qDebug() << "DroneWorker stopped in thread:" << QThread::currentThreadId();
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
    qDebug() << "Connect to drone called with port " << portName;

    m_serialPort.setPortName(portName);
    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if (!m_serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port: " << m_serialPort.errorString();
        emit connectionStatusChanged(false);
    }

    qDebug() << "Serial port opened";

    // Call readData function when there is new data
    connect(&m_serialPort, &QSerialPort::readyRead, this, &DroneWorker::readData);

    emit connectionStatusChanged(true);
}

void DroneWorker::disconnectDrone()
{
    m_serialPort.close();
    emit connectionStatusChanged(false);
}

void DroneWorker::readData()
{
    qDebug() << m_serialPort.bytesAvailable() << " bytes available";
    QByteArray received = m_serialPort.readAll();

    auto error = m_serialPort.error();
    if(m_serialPort.error() != QSerialPort::NoError){
        qDebug() << "Error while reading the data " << error;
        disconnectDrone();
        return;
    }

    qDebug() << "Received from usb: " << received;
    m_recvBuffer.append(received);
    processReadBuffer();
}

void DroneWorker::writeData(const DataPacket &dataPacket)
{
    if(!m_serialPort.isOpen()){
        return;
    }

    m_serialPort.write(dataPacket.serialize());
    m_serialPort.flush();

    auto error = m_serialPort.error();
    if(m_serialPort.error() != QSerialPort::NoError){
        qDebug() << "Error while writing the data " << error;
        disconnectDrone();
        return;
    }
}
