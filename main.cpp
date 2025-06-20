#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QPixmap>
#include <QIcon>
#include <QQmlContext>
#include "dronebackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(QPixmap(1,1)));

    QCoreApplication::setOrganizationName("William Dubuc");
    QCoreApplication::setOrganizationDomain("william.dubuc.com");
    QCoreApplication::setApplicationName("Drone Desktop");

    QQmlApplicationEngine engine;

    DroneBackend* droneBackend = new DroneBackend(&app);

    engine.rootContext()->setContextProperty("drone", droneBackend);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("DroneDesktop", "Main");

    return app.exec();
}
