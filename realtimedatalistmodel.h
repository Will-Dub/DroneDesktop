#ifndef REALTIMEDATALISTMODEL_H
#define REALTIMEDATALISTMODEL_H

#include <QAbstractListModel>

enum RealTimeDataPointType {
    LATITUDE,
    LONGITUDE,
    ALTITUDE,
    UART_ZERO_CONNECTED,
    UART_GPS_CONNECTED,
    I2C_CONNECTED,
    LORA_CONNECTED
};

struct RealTimeDataPoint {
    QString title;
    QString value;
    RealTimeDataPointType type;

    static QString valueFromBool(bool value){
        if(value == true){
            return "True";
        }
        return "False";
    }
};

class RealTimeDataListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ComponentRoles {
        TitleRole = Qt::UserRole + 1,
        ValueRole
    };

    explicit RealTimeDataListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setValueByDataType(RealTimeDataPointType type, const QString& value);

private:
    void addRealTimeDataPoint(const QString& title, const QString& value, RealTimeDataPointType type);

private:
    QVector<RealTimeDataPoint> m_realTimeDataPoints;

};

#endif // REALTIMEDATALISTMODEL_H
