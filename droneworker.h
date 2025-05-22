#ifndef DRONEWORKER_H
#define DRONEWORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "datapacket.h"

class DroneWorker : public QObject
{
    Q_OBJECT
public:
    explicit DroneWorker(QObject* parent = nullptr);
    ~DroneWorker();

    void stopWorking();

private:
    void processReadBuffer();

public slots:
    void connectToDrone(const QString &portName);
    void disconnectDrone();
    void writeData(const DataPacket& dataPacket);

private slots:
    void handleReadData();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void newPacketReceived(const DataPacket &packet);
    void connectionStatusChanged(const bool connected);
    void statusUpdated(const QString &status);

private:
    QSerialPort* m_serialPort{nullptr};
    QByteArray m_recvBuffer;
};

#endif // DRONEWORKER_H
