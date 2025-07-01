#include "gamepadworker.h"

GamepadWorker::GamepadWorker(QObject *parent)
    : QObject(parent),
    m_pollTimer(new QTimer(this))
{
    initializeSDL();

    m_pollTimer->setInterval(16);
    connect(m_pollTimer, &QTimer::timeout, this, &GamepadWorker::pollGamepad);
}

GamepadWorker::~GamepadWorker()
{
    stopPolling();
    cleanupSDL();
    qDebug() << "Gamepad Worker: Deconstructor called";
}

QList<GamepadInfo> GamepadWorker::getAvailableGamepads() const
{
    return m_availableGamepads;
}

int GamepadWorker::getGamepadCount() const
{
    if(!m_sdlInitialized){
        return 0;
    }
    return SDL_NumJoysticks();
}

QString GamepadWorker::getGamepadName(int index) const
{
    if (!m_sdlInitialized || index < 0 || index >= SDL_NumJoysticks()) {
        return QString();
    }

    if (SDL_IsGameController(index)) {
        const char* name = SDL_GameControllerNameForIndex(index);
        return name ? QString::fromUtf8(name) : QString("Unknown");
    }

    return QString();
}

bool GamepadWorker::isGamepadConnected(int index) const
{
    return m_connectedGamepadIndex == index && m_gameController != nullptr;
}

void GamepadWorker::connectToGamepad(int joystickId)
{
    disconnectGamepad();

    // Check the index is valid
    if (!SDL_IsGameController(joystickId)) {
        qCritical() << "Gamepad Worker: Gamepad index " << joystickId << " is not valid";
        return;
    }

    // Open the gamepad
    m_gameController = SDL_GameControllerOpen(joystickId);

    if(m_gameController == nullptr){
        qWarning() << "Gamepad Worker: Error opening the gamepad";
        return;
    }

    m_connectedGamepadIndex = joystickId;
    gamepadRefreshList();
    qDebug() << "Gamepad Worker: Gamepad connected";

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
        qDebug() << "Gamepad Worker: Gamepad disconnected";
    }

    m_connectedGamepadIndex = -1;
    gamepadRefreshList();
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
                emit buttonPressed(event.cbutton.button);
            }
            break;

        case SDL_CONTROLLERBUTTONUP:
            if (event.cbutton.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_gameController))) {
                emit buttonReleased(event.cbutton.button);
            }
            break;

        case SDL_CONTROLLERAXISMOTION:
            if (event.caxis.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_gameController))) {
                emit axisChanged(event.caxis.axis, event.caxis.value);
            }
            break;

        case SDL_CONTROLLERDEVICEADDED:
            qDebug() << "Gamepad Worker: Gamepad added";
            gamepadRefreshList();
            break;

        case SDL_CONTROLLERDEVICEREMOVED:
            if (event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_gameController))) {
                qDebug() << "Gamepad Worker: Gamepad was disconnected";
                disconnectGamepad();
            }
            gamepadRefreshList();
            break;
        }
    }
}

void GamepadWorker::gamepadRefreshList()
{
    if (!m_sdlInitialized) {
        return;
    }

    QList<GamepadInfo> newList;
    int numJoysticks = SDL_NumJoysticks();

    for (int i = 0; i < numJoysticks; ++i) {
        if (SDL_IsGameController(i)) {
            GamepadInfo info;
            info.index = i;
            info.name = getGamepadName(i);
            info.isConnected = isGamepadConnected(i);
            info.type = gamepadTypeToString(SDL_GameControllerTypeForIndex(i));

            // GUID
            SDL_Joystick* joystick = SDL_JoystickOpen(i);
            if (joystick) {
                SDL_JoystickGUID guid = SDL_JoystickGetGUID(joystick);
                char guidStr[33];
                SDL_JoystickGetGUIDString(guid, guidStr, sizeof(guidStr));
                info.guid = QString::fromUtf8(guidStr);
                SDL_JoystickClose(joystick);
            }

            newList.append(info);
        }
    }

    // Emit if change
    if (newList.size() != m_availableGamepads.size()) {
        m_availableGamepads = newList;
        emit gamepadListChanged(m_availableGamepads);
    } else {
        // Check if any gamepad changed
        bool changed = false;
        for (int i = 0; i < newList.size(); ++i) {
            if (i >= m_availableGamepads.size() ||
                newList[i].name != m_availableGamepads[i].name ||
                newList[i].isConnected != m_availableGamepads[i].isConnected) {
                changed = true;
                break;
            }
        }

        if (changed) {
            m_availableGamepads = newList;
            emit gamepadListChanged(m_availableGamepads);
        }
    }
}

void GamepadWorker::gamepadAutoconnect()
{
    if(m_availableGamepads.size() >= 1){
        connectToGamepad(0);
    }
}

void GamepadWorker::initializeSDL()
{
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        qDebug() << "Gamepad Worker: Failed to initialize SDL" << SDL_GetError();
        return;
    }

    m_sdlInitialized = true;
    qDebug() << "Gamepad Worker: SDL initialized successfully";
}

void GamepadWorker::cleanupSDL()
{
    if (m_sdlInitialized) {
        SDL_Quit();
        m_sdlInitialized = false;
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

QString GamepadWorker::gamepadTypeToString(SDL_GameControllerType type) {
    switch (type) {
    case SDL_CONTROLLER_TYPE_XBOX360:
        return "Xbox 360";
    case SDL_CONTROLLER_TYPE_XBOXONE:
        return "Xbox One";
    case SDL_CONTROLLER_TYPE_PS3:
        return "PlayStation 3";
    case SDL_CONTROLLER_TYPE_PS4:
        return "PlayStation 4";
    case SDL_CONTROLLER_TYPE_PS5:
        return "PlayStation 5";
    case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
        return "Nintendo Switch Pro";
    case SDL_CONTROLLER_TYPE_UNKNOWN:
    default:
        return "Unknown";
    }
}
