#ifndef REALTIMEDATALISTMODEL_H
#define REALTIMEDATALISTMODEL_H

#include <QAbstractListModel>

enum RealTimeDataPointType {
    LATITUDE,
    ALTITUDE,
    MPU6050
};

struct RealTimeDataPoint {
    QString title;
    QString value;
    RealTimeDataPointType type;
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

private:
    void addRealTimeDataPoint(const QString& title, const QString& value, RealTimeDataPointType type);
    void setValueByDataType(RealTimeDataPointType type, const QString& value);

private:
    QVector<RealTimeDataPoint> m_realTimeDataPoints;

};

#endif // REALTIMEDATALISTMODEL_H
