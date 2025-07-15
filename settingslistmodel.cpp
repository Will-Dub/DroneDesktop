#include "settingslistmodel.h"

SettingsListModel::SettingsListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    addSetting("Max motor percentage:", "100", SettingType::MAX_MOTOR_PERCENTAGE, InputType::PercentageInput);
}

int SettingsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_settings.size();
}

QVariant SettingsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_settings.size())
        return QVariant();

    const Setting& setting = m_settings.at(index.row());
    switch (role) {
    case TitleRole:
        return setting.title;
    case ValueRole:
        return setting.value;
    case InputTypeRole:
        return setting.inputType;
    case IsChangingRole:
        return setting.isChanging;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> SettingsListModel::roleNames() const
{
    return {
        { TitleRole, "title" },
        { ValueRole, "value" },
        { InputTypeRole, "inputType" },
        { IsChangingRole, "isChanging" },
    };
}

void SettingsListModel::setValueByIndex(int index, const QString &value)
{
    if (index < 0 || index >= m_settings.size()) {
        return;
    }

    if(m_settings[index].value == value) return;

    m_settings[index].value = value;

    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, {ValueRole});
}

SettingType SettingsListModel::getSettingTypeByIndex(int index)
{
    if (index < 0 || index >= m_settings.size()) {
        throw std::out_of_range("Index is out of range");
    }

    return m_settings[index].type;
}

void SettingsListModel::setValueBySettingType(SettingType type, const QString &value)
{
    // TODO optimize this?
    auto it = std::find_if(m_settings.begin(), m_settings.end(), [type](const Setting &setting) {
        return setting.type == type;
    });

    if(it == m_settings.end() || it == nullptr) return;

    if(it->value == value) return;

    it->value = value;
    it->isChanging = false;

    int row = std::distance(m_settings.begin(), it);
    QModelIndex index = createIndex(row, 0);

    emit dataChanged(index, index, {IsChangingRole});
    emit dataChanged(index, index, {ValueRole});
}

void SettingsListModel::setIsChangingByIndex(int index, bool isChanging)
{
    if (index < 0 || index >= m_settings.size()) {
        return;
    }

    if(m_settings[index].isChanging == isChanging) return;

    m_settings[index].isChanging = isChanging;

    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, {IsChangingRole});
}

void SettingsListModel::addSetting(const QString &title, const QString &value, SettingType type, InputType inputType)
{
    beginInsertRows(QModelIndex(), m_settings.size(), m_settings.size());
    m_settings.append({title, value, type, inputType, false});
    endInsertRows();
}
