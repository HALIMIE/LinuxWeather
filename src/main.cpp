#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QLocale>
#include <QTranslator>

#include "core/Weather/WeatherDataManager.h"
int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "piProject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            app.installTranslator(&translator);
            break;
        }
    }

    QQmlApplicationEngine engine;
    WeatherDataManager weatherManager;

    engine.rootContext()->setContextProperty("todayWeatherData", weatherManager.todayWeatherData());
    engine.rootContext()->setContextProperty("weeklyWeatherModel", weatherManager.weeklyWeatherModel());
    engine.rootContext()->setContextProperty("weatherManager", &weatherManager);

    const QUrl url("qrc:/Main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
                     {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    weatherManager.updateWeather();

    return app.exec();
}
