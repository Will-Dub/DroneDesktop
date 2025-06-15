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
    explicit GamepadWorker(SDL_GameController *gameController, QObject *parent = nullptr);
    ~GamepadWorker();

signals:
    void eventQuit();

private:
    void startPolling();
    void stopPolling();

private:
    QTimer *m_pollTimer;
    SDL_GameController *m_gameController;
};

#endif // GAMEPADWORKER_H
