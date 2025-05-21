#include "datapacket.h"

DataPacket::DataPacket() : m_isValid(false)
{}

DataPacket::~DataPacket()
{}

DataPacket::DataPacket(const DataPacketHeader &hdr, const QByteArray &payload)
    : m_header(hdr), m_data(payload), m_isValid(true) {}

DataPacket::DataPacket(uint8_t droneId, int packetId, DataPacketType type, const QByteArray &payload)
{
    m_header.droneId = droneId;
    m_header.packetId = packetId;
    m_header.type = type;
    m_header.dataSize = payload.size();
    m_data = payload;
    m_isValid = true;
}

DataPacket::DataPacket(const DataPacket &other)
    : m_header(other.m_header),
    m_data(other.m_data),
    m_isValid(other.m_isValid)
{}

DataPacket DataPacket::deserialize(const QByteArray &rawData) {
    DataPacket packet;

    // Check if the array is large enough
    if (rawData.size() >= sizeof(DataPacketHeader)) {
        memcpy(&packet.m_header, rawData.constData(), sizeof(DataPacketHeader));

        // Get the data
        if (rawData.size() > sizeof(DataPacketHeader)) {
            // Shrink the recv buffer
            packet.m_data = rawData.mid(sizeof(DataPacketHeader), packet.m_header.dataSize);
            packet.m_isValid = true;
        }
    }
    return packet;
}
QByteArray DataPacket::serialize() const {
    // TODO Utiliser const?
    QByteArray result;

    // Add header
    result.append(reinterpret_cast<const char *>(&m_header), sizeof(DataPacketHeader));

    // Add data
    result.append(m_data);
    return result;
}
