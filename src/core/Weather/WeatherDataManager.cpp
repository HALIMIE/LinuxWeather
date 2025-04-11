#include "WeatherDataManager.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include <QDateTime>
#include <QLocale>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include "WeatherData.h"
#include "../../models/Weather/WeeklyWeatherModel.h"

WeatherDataManager::WeatherDataManager(QObject *parent)
    : QObject(parent)
{
    // load conditions.json and generate mapping
    loadConditionMapping();

    // initial data for test
    m_todayWeatherData = new WeatherData("영상 1도", "맑음", "", this);

    // initial weekly forecast data for test
    m_weeklyWeatherModel = new WeeklyWeatherModel(this);
    m_weeklyWeatherModel->addWeatherData("금", new WeatherData("영상 0도", "맑음", "", m_weeklyWeatherModel));
    m_weeklyWeatherModel->addWeatherData("토", new WeatherData("영하 1도", "눈", "", m_weeklyWeatherModel));
    m_weeklyWeatherModel->addWeatherData("일", new WeatherData("영상 1도", "비", "", m_weeklyWeatherModel));
    m_weeklyWeatherModel->addWeatherData("월", new WeatherData("영하 3도", "맑음", "", m_weeklyWeatherModel));
    m_weeklyWeatherModel->addWeatherData("화", new WeatherData("영하 2도", "맑음", "", m_weeklyWeatherModel));
    m_weeklyWeatherModel->addWeatherData("수", new WeatherData("영하 1도", "맑음", "", m_weeklyWeatherModel));
    m_weeklyWeatherModel->addWeatherData("목", new WeatherData("영상 0도", "맑음", "", m_weeklyWeatherModel));
}

WeeklyWeatherModel *WeatherDataManager::weeklyWeatherModel() const
{
    return m_weeklyWeatherModel;
}

WeatherData *WeatherDataManager::todayWeatherData() const
{
    return m_todayWeatherData;
}

void WeatherDataManager::loadConditionMapping()
{
    QFile file(":/json/conditions.json");
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open conditions.json";
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray())
    {
        qWarning() << "conditions.json is not an array";
        return;
    }

    QJsonArray arr = doc.array();
    // m_conditionMapping: QHash<int, QPair<QPair<QString, QString>, QString>>
    // save ((day_text, night_text), iconUrl) for each condition code
    for (const QJsonValue &val : arr)
    {
        if (val.isObject())
        {
            QJsonObject obj = val.toObject();
            int code = obj.value("code").toInt();

            // find the item in the languages array where lang_name is "Korean"
            QString koreanDayText;
            QString koreanNightText;
            QJsonArray langs = obj.value("languages").toArray();
            for (const QJsonValue &langVal : langs)
            {
                QJsonObject langObj = langVal.toObject();
                if (langObj.value("lang_name").toString().compare("Korean", Qt::CaseInsensitive) == 0)
                {
                    koreanDayText = langObj.value("day_text").toString();
                    koreanNightText = langObj.value("night_text").toString();
                    break;
                }
            }

            // example of icon field: "//cdn.weatherapi.com/weather/64x64/day/113.png"
            QString iconUrl = obj.value("icon").toString();
            if (iconUrl.startsWith("//"))
                iconUrl.prepend("https:");
            m_conditionMapping.insert(code, qMakePair(qMakePair(koreanDayText, koreanNightText), iconUrl));
        }
    }
}

/**
 * @brief Update weather data from the JSON document
 * @param doc document from the weather API
 */
void WeatherDataManager::updateWeatherData(const QJsonDocument &doc)
{
    QJsonObject rootObj = doc.object();

    // [1] Update today's weather
    QJsonObject currentObj = rootObj.value("current").toObject();
    double tempC = currentObj.value("temp_c").toDouble();
    QString tempStr = QString::number(tempC, 'f', 1) + "°C";
    int isDay = currentObj.value("is_day").toInt();
    QJsonObject condObj = currentObj.value("condition").toObject();
    int currentCode = condObj.value("code").toInt();
    QString conditionKorean;
    if (m_conditionMapping.contains(currentCode))
    {
        auto mapping = m_conditionMapping.value(currentCode);
        conditionKorean = (isDay == 1) ? mapping.first.first : mapping.first.second;
    }
    else
    {
        conditionKorean = condObj.value("text").toString();
    }
    QString currentIconUrl = condObj.value("icon").toString();
    if (currentIconUrl.startsWith("//"))
        currentIconUrl.prepend("https:");
    m_todayWeatherData->updateData(tempStr, conditionKorean, currentIconUrl);

    // [2] Update weekly forecast
    QJsonObject forecastObj = rootObj.value("forecast").toObject();
    QJsonArray forecastDays = forecastObj.value("forecastday").toArray();
    m_weeklyWeatherModel->clear();
    QLocale koreanLocale(QLocale::Korean);
    for (int i = 0; i < forecastDays.size(); ++i)
    {
        QDate forecastDate = QDate::currentDate().addDays(i + 1);
        QString dayFull = koreanLocale.toString(forecastDate, "dddd");
        QJsonObject dayForecast = forecastDays.at(i).toObject();
        QJsonObject dayObj = dayForecast.value("day").toObject();
        double maxTempC = dayObj.value("maxtemp_c").toDouble();
        double minTempC = dayObj.value("mintemp_c").toDouble();
        QString maxTempStr = QString::number(maxTempC, 'f', 1) + "°C";
        QString minTempStr = QString::number(minTempC, 'f', 1) + "°C";
        QJsonObject dayCondObj = dayObj.value("condition").toObject();
        int dayCode = dayCondObj.value("code").toInt();
        QString dayConditionKorean;
        if (m_conditionMapping.contains(dayCode))
        {
            auto mapping = m_conditionMapping.value(dayCode);
            dayConditionKorean = mapping.first.first;
        }
        else
        {
            dayConditionKorean = dayCondObj.value("text").toString();
        }
        QString dayIconUrl = dayCondObj.value("icon").toString();
        if (dayIconUrl.startsWith("//"))
            dayIconUrl.prepend("https:");
        m_weeklyWeatherModel->addWeatherData(dayFull,
                                             new WeatherData(maxTempStr, minTempStr, dayConditionKorean, dayIconUrl, m_weeklyWeatherModel));
    }

    // [3] Update hourly forecast
    qint64 nowEpoch = QDateTime::currentSecsSinceEpoch();
    m_hourlyForecast.clear();
    int count = 0;
    for (int i = 0; i < forecastDays.size() && count < 6; ++i)
    {
        QJsonObject dayForecast = forecastDays.at(i).toObject();
        QJsonArray hourArray = dayForecast.value("hour").toArray();
        for (int j = 0; j < hourArray.size() && count < 6; ++j)
        {
            QJsonObject hourObj = hourArray.at(j).toObject();
            qint64 hourEpoch = hourObj.value("time_epoch").toVariant().toLongLong();
            if (hourEpoch >= nowEpoch)
            {
                QString timeStr = hourObj.value("time").toString();
                QString displayTime = timeStr.split(" ")[1];
                double hourTemp = hourObj.value("temp_c").toDouble();
                QString hourTempStr = QString::number(hourTemp, 'f', 1) + "°C";
                QJsonObject hourCondObj = hourObj.value("condition").toObject();
                QString hourIcon = hourCondObj.value("icon").toString();
                if (hourIcon.startsWith("//"))
                    hourIcon.prepend("https:");
                QVariantMap hourForecast;
                hourForecast["time"] = displayTime;
                hourForecast["temp"] = hourTempStr;
                hourForecast["icon"] = hourIcon;
                m_hourlyForecast.append(hourForecast);
                count++;
            }
        }
    }
    emit hourlyForecastChanged();
}

void WeatherDataManager::updateWeather()
{
    QString fileName = ":/json/weather_output.json";
    bool needUpdate = true;
    QFile file(fileName);
    qDebug() << "Opening cached file...";

    // if cached file exists and can be opened, check its modification time
    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = file.readAll();
        file.close();
        QJsonParseError parseError;
        QJsonDocument cachedDoc = QJsonDocument::fromJson(fileData, &parseError);
        if (!cachedDoc.isNull() && parseError.error == QJsonParseError::NoError)
        {
            QFileInfo fileInfo(fileName);
            QDateTime lastModified = fileInfo.lastModified();
            QDateTime currentTime = QDateTime::currentDateTime();
            // if the file is modified within 12 hours, use the cache
            if (lastModified.secsTo(currentTime) < 43200)
            {
                needUpdate = false;
                qDebug() << "Using cached weather data. Last modified:" << lastModified.toString();
                updateWeatherData(cachedDoc);
            }
        }
        else
        {
            qDebug() << "JSON parsing error for cache file:" << parseError.errorString();
        }
    }
    else
    {
        qDebug() << "Cannot open cache file or it doesn't exist. Proceeding with network request.";
    }

    // if cache not used or update is needed, make a network request
    if (needUpdate)
    {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QString apiKey = "44c0db60ba0245309cd60250250302";
        QString city = "Seoul";
        QString urlString = QString("http://api.weatherapi.com/v1/forecast.json?key=%1&q=%2&days=7&lang=kr")
                                .arg(apiKey)
                                .arg(city);
        QUrl url(urlString);
        QNetworkRequest request(url);
        QNetworkReply *reply = manager->get(request);

        connect(reply, &QNetworkReply::finished, this, [this, reply, fileName]()
                {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
                QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                if (!jsonDoc.isNull() && jsonDoc.isObject()) {
                    updateWeatherData(jsonDoc);

                    // Save the response to file
                    QFile outFile(fileName);
                    if (outFile.open(QIODevice::WriteOnly)) {
                        outFile.write(response);
                        outFile.close();
                        qDebug() << "Weather data saved to" << fileName;
                    } else {
                        qWarning() << "Failed to open output file for writing.";
                    }
                } else {
                    qDebug() << "Network response is not a valid JSON";
                }
            } else {
                qDebug() << "Network error:" << reply->errorString();
            }
            reply->deleteLater(); });
    }
}
