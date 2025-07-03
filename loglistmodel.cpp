#include "loglistmodel.h"

LogListModel::LogListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    addLog("Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++Du c++", 0);
    addLog("Du c++1", 1);
    addLog("Du c++2", 2);
    addLog("Du c++3", 3);
    addLog("Du c++4", 4);
    addLog("Du c++5", 5);
}

int LogListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_logs.size();
}

QVariant LogListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_logs.size())
        return QVariant();

    const Log& log = m_logs.at(index.row());
    switch (role) {
        case TextRole:
            return log.text;
        case TypeRole:
            return log.type;
        case TimestampRole:
            return log.timestamp.toString("hh:mm:ss");
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> LogListModel::roleNames() const
{
    return {
        { TextRole, "text" },
        { TypeRole, "type" },
        { TimestampRole, "timestamp" }
    };
}

void LogListModel::addLog(const QString &text, int type)
{
    beginInsertRows(QModelIndex(), m_logs.size(), m_logs.size());
    m_logs.append({text, type, QDateTime::currentDateTime()});
    endInsertRows();
}
