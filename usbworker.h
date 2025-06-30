#ifndef USBWORKER_H
#define USBWORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "datapacket.h"

class UsbWorker : public QObject
{
    Q_OBJECT
public:
    explicit UsbWorker(QObject* parent = nullptr);
    ~UsbWorker();

    void stopWorking();

private:
    void processReadBuffer();

public slots:
    void connectToUsb(const QString &portName);
    void disconnectUsb();
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

#endif // USBWORKER_H
