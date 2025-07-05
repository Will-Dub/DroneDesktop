#include "componentlistmodel.h"

ComponentListModel::ComponentListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    addComponent("Motors", "MOTOR");
    addComponent("Motor 1", "MOTOR_1");
    addComponent("Motor 2", "MOTOR_2");
    addComponent("Motor 3", "MOTOR_3");
    addComponent("Motor 4", "MOTOR_4");
    addComponent("Get motor informations", "MOTOR_INFORMATION");
    addComponent("Use MPU 6050", "MPU6050");
    addComponent("Use QMC 5883l", "QML5883L");
    addComponent("Use GPS", "GPS");
    addComponent("Get logs", "LOG");
}

int ComponentListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_components.size();
}

QVariant ComponentListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_components.size())
        return QVariant();

    const Component& component = m_components.at(index.row());
    switch (role) {
    case NameRole:
        return component.name;
    case StatusRole:
        return component.status;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ComponentListModel::roleNames() const
{
    return {
        { NameRole, "name" },
        { StatusRole, "status" },
    };
}

void ComponentListModel::addComponent(const QString &name, const QString& value)
{
    beginInsertRows(QModelIndex(), m_components.size(), m_components.size());
    m_components.append({name, value, Status::OFF_LOCKED});
    endInsertRows();
}

void ComponentListModel::setStatus(int index, Status status)
{
    if (index < 0 || index >= m_components.size()) {
        return;
    }

    m_components[index].status = status;

    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, {StatusRole});
}

void ComponentListModel::toggleStatus(int index)
{
    if (index < 0 || index >= m_components.size()) {
        return;
    }

    Status currentStatus = m_components[index].status;

    if(currentStatus == Status::ON || currentStatus == Status::OFF){
        m_components[index].status = Status::CHANGING;
    }else{
        qWarning() << "Component list model: Invalid toggle event";
    }

    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, {StatusRole});
}

const Component& ComponentListModel::getComponent(int index) const
{
    if (index < 0 || index >= m_components.size()) {
        throw std::out_of_range("Component list model: Index " + std::to_string(index) + " is out of range");
    }

    return m_components[index];
}
