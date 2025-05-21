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
    START,
    START_SPECIFIC,
    CHANGE_SPEED
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
