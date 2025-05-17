#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QPixmap>
#include <QIcon>
#include "dronebackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(QPixmap(1,1)));

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance<DroneBackend>("DroneDesktop", 1, 0, "DroneBackend",
                                                 DroneBackend::instance());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("DroneDesktop", "Main");

    return app.exec();
}
