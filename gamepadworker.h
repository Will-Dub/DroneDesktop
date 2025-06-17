#define SDL_MAIN_HANDLED
#ifndef GAMEPADWORKER_H
#define GAMEPADWORKER_H

#include <QObject>
#include <QTimer>
#include <SDL2/SDL.h>
#include <QDebug>

class GamepadWorker : public QObject
{
    Q_OBJECT

public:
    explicit GamepadWorker(QObject *parent = nullptr);
    ~GamepadWorker();

public slots:
    void connectToGamepad(int joystickId = 0);
    void disconnectGamepad();
    void pollGamepad();

signals:
    void gamepadConnectionStatusChanged(bool connected);
    void buttonPressed(int button);
    void buttonReleased(int button);
    void axisChanged(int axis, int value);

private:
    void startPolling();
    void stopPolling();

private:
    QTimer *m_pollTimer;
    SDL_GameController *m_gameController = nullptr;
};

#endif // GAMEPADWORKER_H
