#ifndef LOGLISTMODEL_H
#define LOGLISTMODEL_H

#include <QAbstractListModel>
#include <QDateTime>

struct Log {
    QString text;
    int type;
    QDateTime timestamp;
};

class LogListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum LogRoles {
        TextRole = Qt::UserRole + 1,
        TypeRole,
        TimestampRole
    };

    explicit LogListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addLog(const QString& text, int type);

private:
    QVector<Log> m_logs;
};

#endif // LOGLISTMODEL_H
