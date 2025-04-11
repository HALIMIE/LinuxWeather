# LinuxWeather

# 🌤️ Weather UI Application (Qt/QML + C++)

##  프로젝트 개요

이 프로젝트는 **Qt(QML + C++) 기반의 날씨 정보 UI 애플리케이션**입니다.  
실시간 날씨 데이터를 외부 API 또는 JSON 파일로부터 수신하여 사용자에게 당일 및 주간 날씨 정보를 시각적으로 제공합니다.  
**Linux(Ubuntu) 환경에서 개발 및 테스트**하였으며, QML UI와 C++ 백엔드를 연동한 구조를 따릅니다.

---

##  사용 기술

| 분류           | 내용 |
|----------------|------|
| 언어           | C++, QML |
| 프레임워크     | Qt 6 (QtQuick, Qt QML) |
| UI 엔진        | QQmlApplicationEngine |
| 운영체제       | **Linux (Ubuntu 22.04)** |
| 데이터 처리    | JSON (conditions.json) |
| 프로젝트 구조  | MVVM 기반 데이터 바인딩 |
| 기타           | QTranslator, Qt Resource System, Qt Creator |

---

##  핵심 기능

- ✅ 실시간 날씨 데이터 바인딩 (`WeatherDataManager` 클래스)
- ✅ QML UI에서 todayWeatherData / weeklyWeatherModel 표시
- ✅ JSON 기반 다국어 날씨 상태 텍스트 지원
- ✅ 시스템 언어 감지 및 자동 번역 적용
- ✅ 리눅스 Qt Creator에서 빌드 및 실행 가능
