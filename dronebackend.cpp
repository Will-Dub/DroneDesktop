#include "dronebackend.h"

DroneBackend::DroneBackend(QObject *parent) : QObject(parent), m_is_connected(false), m_status("Disconnected") {
    m_worker = new DroneWorker();

    m_workerThread = new QThread(this);
    m_worker->moveToThread(m_workerThread);

    // Connect start and finish
    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);

    connect(m_workerThread, &QThread::started, m_worker, [this]() {
        qDebug() << "Worker thread started";
    });

    // Connect backend to worker
    connect(this, &DroneBackend::doConnect,
            m_worker, &DroneWorker::connectToDrone);

    connect(this, &DroneBackend::doDisconnect,
            m_worker, &DroneWorker::disconnectDrone);

    // Connect worker to backend
    connect(m_worker, &DroneWorker::connectionStatusChanged,
            this, &DroneBackend::onConnectionStatusChanged);

    // Start the thread
    m_workerThread->start();
}

DroneBackend::~DroneBackend(){
    if (m_worker && m_workerThread) {
        m_worker->stopWorking();

        // tell to quit and wait
        m_workerThread->quit();

        if (!m_workerThread->wait(3000)) {
            qWarning() << "Worker thread failed to terminate, forcing termination";
            m_workerThread->terminate();
            m_workerThread->wait();
        }
    }
}

bool DroneBackend::isConnected() const
{
    return m_is_connected;
}

void DroneBackend::setIsConnected(bool is_connected)
{
    if (m_is_connected != is_connected) {
        m_is_connected = is_connected;
        emit connectedChanged();
    }
}

QString DroneBackend::status() const
{
    return m_status;
}

bool DroneBackend::connectToDrone()
{
    if (!m_is_connected) {
        emit doConnect();
        return true;
    }
    return false;
}

void DroneBackend::disconnectDrone()
{
    if(m_is_connected){
        emit doDisconnect();
    }
}

void DroneBackend::onConnectionStatusChanged(bool is_connected)
{
    setIsConnected(is_connected);
}

void DroneBackend::onStatusUpdate(const QString &newStatus)
{
    if (m_status != newStatus) {
        m_status = newStatus;
        emit statusChanged();
    }
}
