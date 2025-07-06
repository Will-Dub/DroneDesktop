#ifndef COMPONENTLISTMODEL_H
#define COMPONENTLISTMODEL_H

#include <QAbstractListModel>

enum Status{
    OFF_LOCKED, // Checkbox off and disabled
    OFF, // Checkbox off
    CHANGING, // Checkbox is tristate and disabled
    ON, // Checkbox on
    ON_LOCKED // Checkbox on and disabled
};

enum ComponentValue {
    COMPONENT_MOTOR,
    COMPONENT_MOTOR_1,
    COMPONENT_MOTOR_2,
    COMPONENT_MOTOR_3,
    COMPONENT_MOTOR_4,
    COMPONENT_MOTOR_INFORMATION,
    COMPONENT_MPU6050,
    COMPONENT_QMC5883L,
    COMPONENT_GPS,
    COMPONENT_LOG,
};

inline QString componentValueToString(ComponentValue value) {
    switch(value) {
    case COMPONENT_MOTOR: return "MOTOR";
    case COMPONENT_MOTOR_1: return "MOTOR_1";
    case COMPONENT_MOTOR_2: return "MOTOR_2";
    case COMPONENT_MOTOR_3: return "MOTOR_3";
    case COMPONENT_MOTOR_4: return "MOTOR_4";
    case COMPONENT_MOTOR_INFORMATION: return "MOTOR_INFORMATION";
    case COMPONENT_MPU6050: return "MPU6050";
    case COMPONENT_QMC5883L: return "QMC5883L";
    case COMPONENT_GPS: return "GPS";
    case COMPONENT_LOG: return "LOG";
    default: return "";
    }
}

inline ComponentValue stringToComponentValue(const QString& str) {
    if (str == "MOTOR") return COMPONENT_MOTOR;
    if (str == "MOTOR_1") return COMPONENT_MOTOR_1;
    if (str == "MOTOR_2") return COMPONENT_MOTOR_2;
    if (str == "MOTOR_3") return COMPONENT_MOTOR_3;
    if (str == "MOTOR_4") return COMPONENT_MOTOR_4;
    if (str == "MOTOR_INFORMATION") return COMPONENT_MOTOR_INFORMATION;
    if (str == "MPU6050") return COMPONENT_MPU6050;
    if (str == "QMC5883L") return COMPONENT_QMC5883L;
    if (str == "GPS") return COMPONENT_GPS;
    if (str == "LOG") return COMPONENT_LOG;
    return COMPONENT_LOG;
}

struct Component {
    QString name;
    ComponentValue value;
    Status status;

    static Status statusFromBool(bool enabled) {
        return enabled ? Status::ON : Status::OFF;
    }
};

class ComponentListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ComponentRoles {
        NameRole = Qt::UserRole + 1,
        StatusRole
    };

    explicit ComponentListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void toggleStatus(int index);
    const Component& getComponent(int index) const;

    void setStatus(int index, Status status);
    void setStatusByComponentValue(ComponentValue componentValue, Status status);

private:
    void addComponent(const QString& name, ComponentValue componentValue);

private:
    QVector<Component> m_components;
};

#endif // COMPONENTLISTMODEL_H
