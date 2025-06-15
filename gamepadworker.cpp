#include "gamepadworker.h"

GamepadWorker::GamepadWorker(SDL_GameController *gameController, QObject *parent)
    : QObject(parent),
    m_pollTimer(new QTimer(this)),
    m_gameController(gameController)
{
    qDebug() << "Gamepad worker: Constructor called";
    connect(m_pollTimer, &QTimer::timeout, this, [this]() {
        if (m_gameController) {
            qInfo() << "Read";
        }
        stopPolling();
        emit eventQuit();
    });

    // 30 hz
    m_pollTimer->setInterval(8);
    startPolling();
}

GamepadWorker::~GamepadWorker()
{
    stopPolling();
    qDebug() << "Gamepad worker: Deconstructor called";
}

void GamepadWorker::startPolling()
{
    if (!m_pollTimer->isActive()){
        m_pollTimer->start();
    }
}

void GamepadWorker::stopPolling()
{
    if (m_pollTimer->isActive()){
        m_pollTimer->stop();
    }
}
