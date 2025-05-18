#ifndef DRONEWORKER_H
#define DRONEWORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>

class DroneWorker : public QObject
{
    Q_OBJECT
public:
    explicit DroneWorker(QObject* parent = nullptr);
    ~DroneWorker();

    void stopWorking();

public slots:
    void connectToDrone();

signals:
    void connectionEstablished();
    void connectionFailed(const QString& reason);

private:
};

#endif // DRONEWORKER_H
