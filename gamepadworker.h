#ifndef GAMEPADWORKER_H
#define GAMEPADWORKER_H

#include <QObject>
#include <QTimer>
#include <SDL3/SDL_gamepad.h>
#include <QDebug>

class GamepadWorker : public QObject
{
    Q_OBJECT
public:
    explicit GamepadWorker(SDL_Gamepad *gamepad, QObject *parent = nullptr);
    ~GamepadWorker();

public slots:
    void startPolling();
    void stopPolling();

private:
    QTimer *m_pollTimer;
    SDL_Gamepad *m_gamepad;
};

#endif // GAMEPADWORKER_H
