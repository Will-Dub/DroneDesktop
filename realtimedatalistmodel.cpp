#include "realtimedatalistmodel.h"

RealTimeDataListModel::RealTimeDataListModel(QObject *parent)
{
    addRealTimeDataPoint("Latitude", "0", RealTimeDataPointType::LATITUDE);
    addRealTimeDataPoint("Longitude", "0", RealTimeDataPointType::LONGITUDE);
    addRealTimeDataPoint("Altitude", "0", RealTimeDataPointType::ALTITUDE);
    addRealTimeDataPoint("Is zero connected", RealTimeDataPoint::valueFromBool(false), RealTimeDataPointType::UART_ZERO_CONNECTED);
    addRealTimeDataPoint("Is GPS connected", RealTimeDataPoint::valueFromBool(false), RealTimeDataPointType::UART_GPS_CONNECTED);
    addRealTimeDataPoint("Is i2c connected", RealTimeDataPoint::valueFromBool(false), RealTimeDataPointType::I2C_CONNECTED);
    addRealTimeDataPoint("Is lora connected", RealTimeDataPoint::valueFromBool(false), RealTimeDataPointType::LORA_CONNECTED);
}

int RealTimeDataListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_realTimeDataPoints.size();
}

QVariant RealTimeDataListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_realTimeDataPoints.size())
        return QVariant();

    const RealTimeDataPoint& realTimeDataPoint = m_realTimeDataPoints.at(index.row());
    switch (role) {
    case TitleRole:
        return realTimeDataPoint.title;
    case ValueRole:
        return realTimeDataPoint.value;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> RealTimeDataListModel::roleNames() const
{
    return {
        { TitleRole, "title" },
        { ValueRole, "value" },
    };
}

void RealTimeDataListModel::setValueByDataType(RealTimeDataPointType type, const QString& value)
{
    // TODO optimize this?
    auto it = std::find_if(m_realTimeDataPoints.begin(), m_realTimeDataPoints.end(), [type](const RealTimeDataPoint &realTimeDataPoint) {
        return realTimeDataPoint.type == type;
    });

    if(it == m_realTimeDataPoints.end() || it == nullptr) return;

    if(it->value == value) return;

    it->value = value;

    int row = std::distance(m_realTimeDataPoints.begin(), it);
    QModelIndex index = createIndex(row, 0);

    emit dataChanged(index, index, {ValueRole});
}

void RealTimeDataListModel::addRealTimeDataPoint(const QString &title, const QString &value, RealTimeDataPointType type)
{
    beginInsertRows(QModelIndex(), m_realTimeDataPoints.size(), m_realTimeDataPoints.size());
    m_realTimeDataPoints.append({title, value, type});
    endInsertRows();
}
