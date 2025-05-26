#include "gamepadworker.h"

GamepadWorker::GamepadWorker(SDL_Gamepad *gamepad, QObject *parent)
    : QObject(parent),
    m_pollTimer(new QTimer(this)),
    m_gamepad(gamepad)
{
    connect(m_pollTimer, &QTimer::timeout, this, [this]() {
        if (m_gamepad) {
            qInfo() << "Read";
        }
    });
    // 60 hz
    m_pollTimer->setInterval(16);
}

GamepadWorker::~GamepadWorker()
{
    stopPolling();
}

void GamepadWorker::startPolling() {
    if (!m_pollTimer->isActive()){
        m_pollTimer->start();
    }
}

void GamepadWorker::stopPolling() {
    if (m_pollTimer->isActive()){
        m_pollTimer->stop();
    }
}
