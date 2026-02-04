#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "calculator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    qmlRegisterType<Calculator>("Calculator", 1, 0,"Calculator");
    qRegisterMetaType<PairQml>("PairQml");
    engine.loadFromModule("qml_calculator", "Main");

    return app.exec();
}
