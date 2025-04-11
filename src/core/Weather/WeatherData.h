#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>

class WeatherData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature NOTIFY dataChanged)
    Q_PROPERTY(QString condition READ condition NOTIFY dataChanged)
    Q_PROPERTY(QString iconUrl READ iconUrl NOTIFY dataChanged)
    Q_PROPERTY(QString maxTemperature READ maxTemperature NOTIFY dataChanged)
    Q_PROPERTY(QString minTemperature READ minTemperature NOTIFY dataChanged)
public:
    explicit WeatherData(QObject *parent = nullptr)
        : QObject(parent)
    {
        m_temperature = "15C";
        m_condition = "Sunny";
        m_iconUrl = "";
        m_maxTemperature = "";
        m_minTemperature = "";
    }
    // today
    explicit WeatherData(const QString &temperature,
                         const QString &condition,
                         const QString &iconUrl,
                         QObject *parent = nullptr)
        : QObject(parent),
          m_temperature(temperature),
          m_condition(condition),
          m_iconUrl(iconUrl)
    {
        m_maxTemperature = "";
        m_minTemperature = "";
    }
    // forecast
    explicit WeatherData(const QString &maxTemperature,
                         const QString &minTemperature,
                         const QString &condition,
                         const QString &iconUrl,
                         QObject *parent = nullptr)
        : QObject(parent),
          m_maxTemperature(maxTemperature),
          m_minTemperature(minTemperature),
          m_condition(condition),
          m_iconUrl(iconUrl)
    {
        m_temperature = "";
    }

    QString temperature() const { return m_temperature; }
    QString condition() const { return m_condition; }
    QString iconUrl() const { return m_iconUrl; }
    QString maxTemperature() const { return m_maxTemperature; }
    QString minTemperature() const { return m_minTemperature; }

    // update today
    void updateData(const QString &temperature,
                    const QString &condition,
                    const QString &iconUrl)
    {
        m_temperature = temperature;
        m_condition = condition;
        m_iconUrl = iconUrl;
        emit dataChanged();
    }

    // update forecast
    void updateForecastData(const QString &maxTemperature,
                            const QString &minTemperature,
                            const QString &condition,
                            const QString &iconUrl)
    {
        m_maxTemperature = maxTemperature;
        m_minTemperature = minTemperature;
        m_condition = condition;
        m_iconUrl = iconUrl;
        emit dataChanged();
    }

signals:
    void dataChanged();

private:
    QString m_temperature;
    QString m_condition;
    QString m_iconUrl;
    QString m_maxTemperature;
    QString m_minTemperature;
};

#endif // WEATHERDATA_H
