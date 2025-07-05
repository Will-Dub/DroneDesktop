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

struct Component {
    QString name;
    QString value;
    Status status;
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

private:
    void addComponent(const QString& name, const QString& value);
    void setStatus(int index, Status status);

private:
    QVector<Component> m_components;
};

#endif // COMPONENTLISTMODEL_H
