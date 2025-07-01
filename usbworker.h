#ifndef USBWORKER_H
#define USBWORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "datapacket.h"

struct UsbInfo {
    QString portName;
    QString systemLocation;
    QString manufacturer;
    QString serialNumber;
    bool isConnected;
};

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
    void usbRefreshList();
    void usbAutoconnect();

private slots:
    void handleReadData();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void newPacketReceived(const DataPacket &packet);
    void connectionStatusChanged(const bool connected);
    void statusUpdated(const QString &status);
    void usbListChanged(const QList<UsbInfo>& usb);

private:
    bool isUsbConnected(const QSerialPortInfo& serialPortInfo);

    QList<UsbInfo> m_availableUsb;
    QSerialPort* m_serialPort{nullptr};
    QByteArray m_recvBuffer;
};

#endif // USBWORKER_H
