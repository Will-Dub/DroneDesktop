#include "gamepadworker.h"

GamepadWorker::GamepadWorker(QObject *parent)
    : QObject(parent),
    m_pollTimer(new QTimer(this))
{
    m_pollTimer->setInterval(16);
    connect(m_pollTimer, &QTimer::timeout, this, &GamepadWorker::pollGamepad);
}

GamepadWorker::~GamepadWorker()
{
    stopPolling();
    qDebug() << "Gamepad worker: Deconstructor called";
}

void GamepadWorker::connectToGamepad(int joystickId)
{
    disconnectGamepad();

    // Check the index is valid
    if (!SDL_IsGameController(joystickId)) {
        qCritical() << "Gamepad index " << joystickId << " is not valid";
        return;
    }

    // Open the gamepad
    m_gameController = SDL_GameControllerOpen(joystickId);

    if(m_gameController == nullptr){
        qWarning() << "Error opening the gamepad";
        return;
    }

    qDebug() << "Gamepad connected";

    // Start the pooling
    startPolling();

    emit gamepadConnectionStatusChanged(true);
}

void GamepadWorker::disconnectGamepad()
{
    stopPolling();

    if (m_gameController) {
        SDL_GameControllerClose(m_gameController);
        m_gameController = nullptr;
        qDebug() << "Gamepad disconnected";
    }

    emit gamepadConnectionStatusChanged(false);
}

void GamepadWorker::pollGamepad()
{
    if (!m_gameController) {
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_CONTROLLERBUTTONDOWN:
            if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_gameController))) {
                qDebug() << "Button down " << event.cbutton.button;
                emit buttonPressed(event.cbutton.button);
            }
            break;

        case SDL_CONTROLLERBUTTONUP:
            if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_gameController))) {
                qDebug() << "Button up " << event.cbutton.button;
                emit buttonReleased(event.cbutton.button);
            }
            break;

        case SDL_CONTROLLERAXISMOTION:
            if (event.caxis.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_gameController))) {
                qDebug() << "Axis motion " << event.caxis.axis << " " << event.caxis.value;
                emit axisChanged(event.caxis.axis, event.caxis.value);
            }
            break;

        case SDL_CONTROLLERDEVICEREMOVED:
            if (event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_gameController))) {
                qDebug() << "Gamepad was disconnected";
                disconnectGamepad();
            }
            break;
        }
    }
}

void GamepadWorker::startPolling()
{
    if (m_gameController && !m_pollTimer->isActive()){
        m_pollTimer->start();
    }
}

void GamepadWorker::stopPolling()
{
    if (m_pollTimer->isActive()){
        m_pollTimer->stop();
    }
}
