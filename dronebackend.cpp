#include "dronebackend.h"

DroneBackend::DroneBackend(QObject *parent) : QObject(parent) {
    m_worker = new DroneWorker();

    m_workerThread = new QThread(this);
    m_worker->moveToThread(m_workerThread);

    // Start and finish
    connect(m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);

    connect(m_workerThread, &QThread::started, m_worker, [this]() {
        qDebug() << "Worker thread started";
    });

    // Connect buisness logic
    connect(this, &DroneBackend::initialize,
            m_worker, &DroneWorker::connectToDrone);

    connect(m_worker, &DroneWorker::connectionFailed,
            this, &DroneBackend::processRequest);

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

QString DroneBackend::processRequest(const QString &data) {
    emit dataChanged("Response to: " + data);

    return "Request processed";
}
