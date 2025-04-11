#ifndef WEEKLYWEATHERMODEL_H
#define WEEKLYWEATHERMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include "../../core/Weather/WeatherData.h"

// global enum for exposing roles to QML
enum WeatherRoles
{
    DayRole = Qt::UserRole + 1,
    TemperatureRole,
    ConditionRole,
    IconUrlRole,
    MaxTempRole,
    MinTempRole
};

class WeeklyWeatherModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeeklyWeatherModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // add weekly weather data to the model
    void addWeatherData(const QString &day, WeatherData *data);

    // clear all data in the model
    void clear();

private:
    QList<WeatherData *> m_weatherList;
    QStringList m_days;
};

#endif // WEEKLYWEATHERMODEL_H
