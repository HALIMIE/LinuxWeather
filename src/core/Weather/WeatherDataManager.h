#ifndef WEATHERDATAMANAGER_H
#define WEATHERDATAMANAGER_H

#include "../../models/Weather/WeeklyWeatherModel.h"
#include "WeatherData.h"
#include <QHash>
#include <QPair>
#include <QObject>
#include <QVariantList>

class WeatherDataManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList hourlyForecast READ hourlyForecast NOTIFY hourlyForecastChanged)
public:
    explicit WeatherDataManager(QObject *parent = nullptr);

    WeeklyWeatherModel *weeklyWeatherModel() const;
    WeatherData *todayWeatherData() const;

    Q_INVOKABLE void updateWeather();

    QVariantList hourlyForecast() const { return m_hourlyForecast; }

signals:
    void hourlyForecastChanged();

private:
    // mapping from conditions.json: key = condition code, value = ( (day_text, night_text), iconUrl )
    QHash<int, QPair<QPair<QString, QString>, QString>> m_conditionMapping;
    void loadConditionMapping();
    void updateWeatherData(const QJsonDocument &doc);

    WeatherData *m_todayWeatherData;
    WeeklyWeatherModel *m_weeklyWeatherModel;

    // 6 hours forecast data
    QVariantList m_hourlyForecast;
};

#endif // WEATHERDATAMANAGER_H
