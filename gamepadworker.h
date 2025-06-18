#define SDL_MAIN_HANDLED
#ifndef GAMEPADWORKER_H
#define GAMEPADWORKER_H

#include <QObject>
#include <QTimer>
#include <SDL2/SDL.h>
#include <QDebug>

struct GamepadInfo {
    int index;
    QString name;
    QString guid;
    QString type;
    bool isConnected;
};

class GamepadWorker : public QObject
{
    Q_OBJECT

public:
    explicit GamepadWorker(QObject *parent = nullptr);
    ~GamepadWorker();

    // Gamepad listing functions
    QList<GamepadInfo> getAvailableGamepads() const;
    int getGamepadCount() const;
    QString getGamepadName(int index) const;
    bool isGamepadConnected(int index) const;

public slots:
    void connectToGamepad(int joystickId = 0);
    void disconnectGamepad();
    void refreshGamepadList();

private slots:
    void pollGamepad();

signals:
    void gamepadConnectionStatusChanged(bool connected);
    void buttonPressed(int button);
    void buttonReleased(int button);
    void axisChanged(int axis, int value);
    void gamepadListChanged(const QList<GamepadInfo>& gamepads);

private:
    void initializeSDL();
    void cleanupSDL();
    void startPolling();
    void stopPolling();
    void updateGamepadList();
    QString gamepadTypeToString(SDL_GameControllerType type);

private:
    QTimer *m_pollTimer;
    SDL_GameController *m_gameController = nullptr;
    bool m_sdlInitialized = false;
    QList<GamepadInfo> m_availableGamepads;
    int m_connectedGamepadIndex;
};

#endif // GAMEPADWORKER_H
