#include "droneworker.h"

DroneWorker::DroneWorker(QObject *parent)
    : QObject{parent},
    m_connected(false)
{
    qDebug() << "DroneWorker constructed in thread:" << QThread::currentThreadId();
}

DroneWorker::~DroneWorker()
{
    stopWorking();

    qDebug() << "DroneWorker destroyed in thread:" << QThread::currentThreadId();
}

void DroneWorker::stopWorking()
{
    qDebug() << "DroneWorker stopped in thread:" << QThread::currentThreadId();
}

void DroneWorker::connectToDrone()
{
    QThread::sleep(1);
    emit connectionStatusChanged(true);
}

void DroneWorker::disconnectDrone()
{
    QThread::sleep(1);
    emit connectionStatusChanged(false);
}
