#include "droneworker.h"

DroneWorker::DroneWorker(QObject *parent)
    : QObject{parent}
{}

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
    QThread::sleep(10);
    emit connectionFailed("asdfasdfasdf");
}
