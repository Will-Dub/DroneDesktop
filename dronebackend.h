#ifndef DRONEBACKEND_H
#define DRONEBACKEND_H

#pragma once
#include <QObject>

class DroneBackend : public QObject
{
    Q_OBJECT
public:
    static DroneBackend* instance();

    Q_INVOKABLE bool initialize();
    Q_INVOKABLE QString processRequest(const QString &data);

signals:
    void dataChanged(const QString &newData);

private:
    DroneBackend(QObject *parent = nullptr);
    static DroneBackend* m_instance;
};

#endif // DRONEBACKEND_H
