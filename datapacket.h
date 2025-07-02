#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <QObject>

enum DataPacketType : uint8_t{
    UNDEFINED,
    IMAGE,
    CONTROL,
    INFO,
    STATUS,
    LOG,
    SENSOR,
    GPS,
    STOP,
    STOP_SPECIFIC,
    START, // 10
    START_SPECIFIC,
    CHANGE_SPEED
};

struct StatusData {
    bool uartZeroConnected = false;
    bool uartGpsConnected = false;
    bool i2cConnected = false;
    bool loraConnected = false;
    bool useMotor1 = false;
    bool useMotor2 = false;
    bool useMotor3 = false;
    bool useMotor4 = false;
    bool useMpu6050 = false;
    bool useQmc5883l = false;
    bool useGps = false;
    bool useLog = false;
    bool useMotorInformation = false;
    bool maxMotorSpeed = false;

    bool deserialize(const QByteArray& rawData) {
        QString str = QString::fromUtf8(rawData);
        QStringList parts = str.split(';', Qt::SkipEmptyParts);

        if (parts.size() < 14)
            return false;

        uartZeroConnected = (parts[0] == "1");
        uartGpsConnected = (parts[1] == "1");
        i2cConnected = (parts[2] == "1");
        loraConnected = (parts[3] == "1");
        useMotor1 = (parts[4] == "1");
        useMotor2 = (parts[5] == "1");
        useMotor3 = (parts[6] == "1");
        useMotor4 = (parts[7] == "1");
        useMpu6050 = (parts[8] == "1");
        useQmc5883l = (parts[9] == "1");
        useGps = (parts[10] == "1");
        useLog = (parts[11] == "1");
        useMotorInformation = (parts[12] == "1");
        maxMotorSpeed = (parts[13] == "1");

        return true;
    }
};

#pragma pack(push, 1) // No padding
struct DataPacketHeader {
    uint8_t droneId;         //1
    int packetId;            //4
    DataPacketType type;     //1
    int dataSize;            //4
};
#pragma pack(pop)

class DataPacket
{
public:
    DataPacket();

    ~DataPacket();

    DataPacket(const DataPacketHeader& hdr, const QByteArray& payload);
    DataPacket(uint8_t droneId, int packetId, DataPacketType type, const QByteArray& payload);
    DataPacket(const DataPacket& other);

    QByteArray serialize() const;

    static DataPacket deserialize(const QByteArray &rawData);

public:
    DataPacketHeader m_header;
    QByteArray m_data;
    bool m_isValid;

signals:
};

Q_DECLARE_METATYPE(DataPacket)

#endif // DATAPACKET_H
