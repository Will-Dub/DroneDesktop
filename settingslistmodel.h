#ifndef SETTINGSLISTMODEL_H
#define SETTINGSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

enum SettingType {
    MAX_MOTOR_PERCENTAGE
};

enum InputType {
    PercentageInput,
};

struct Setting {
    QString title;
    QString value;
    SettingType type;
    InputType inputType;
    bool isChanging;

    static QString valueFromBool(bool value){
        if(value == true){
            return "True";
        }
        return "False";
    }
};

class SettingsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SettingRoles {
        TitleRole = Qt::UserRole + 1,
        ValueRole,
        InputTypeRole,
        IsChangingRole
    };

    explicit SettingsListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setValueBySettingType(SettingType type, const QString& value);
    void setIsChangingByIndex(int index, bool isChanging);
    void setValueByIndex(int index, const QString &value);

    SettingType getSettingTypeByIndex(int index);

private:
    void addSetting(const QString& title, const QString& value, SettingType type, InputType inputType);

private:
    QVector<Setting> m_settings;
};

#endif // SETTINGSLISTMODEL_H
