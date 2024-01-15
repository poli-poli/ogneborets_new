## Описание проекта
Этот проект представляет собой распределенную систему, использующую RabbitMQ для обмена сообщениями между различными компонентами. Система включает в себя несколько подсистем, каждая из которых выполняет свою уникальную функцию в рамках общей архитектуры.

# Разработка и Тестирование

## Окружение для Разработки

Для работы с проектом убедитесь, что следующие инструменты установлены:

### Docker

- **Проверка:** `docker --version`
- **Установка:** [Docker](https://www.docker.com/get-started)

### Docker Compose

- **Проверка:** `docker-compose --version`
- **Установка:** Включен в установку Docker

### GNU Make

- **Проверка:** `make --version`
- **Установка (Linux):** `sudo apt-get install make` или `sudo yum install make`

### Компилятор C++

- **Linux:** Обычно уже установлен или `sudo apt-get install g++`
- **Windows:** [MinGW](http://www.mingw.org/) или компилятор Visual Studio
### Компилятор Rust

- **Проверка:** `rustc --version`
- **Установка:** [Установка Rust](https://www.rust-lang.org/tools/install)
  - В Linux и macOS: Команда `curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh`
  - В Windows: Скачайте и запустите [rustup-init.exe](https://win.rustup.rs/)


### PowerShell (Windows)

- **Установка:** Уже предустановлен на Windows 10, для старых версий [сайт Microsoft](https://docs.microsoft.com/ru-ru/powershell/scripting/install/installing-powershell)


## Запуск сборки проекта:
`make all`
## Остановка и очистка:
`make clean`
## Тестирование

### Тесты SecurityMonitor
Включают четыре теста, направленных на проверку корректности функций обработки операций. В каждом тесте проверяется, что функция check_operation корректно определяет допустимые и недопустимые операции. Тесты охватывают различные сценарии, включая валидные операции, невалидные источники, невалидные операции и сценарии с отсутствующими полями.
### Ожидаемые результаты тестов SecurityMonitor:

**test_check_operation_valid:** Успешно проходит для валидных комбинаций source, deliver_to и operation. Подтверждает, что функция правильно идентифицирует допустимые операции.

**test_check_operation_invalid_source:** Не проходит, если source недействительный или неизвестный. Демонстрирует, что функция отклоняет операции от недопустимых источников.

**test_check_operation_invalid_operation:** Не проходит при недопустимых операциях. Подтверждает, что только разрешенные операции принимаются.

**test_check_operation_missing_field:** Не проходит, если в данных отсутствует обязательное поле. Гарантирует, что все необходимые данные присутствуют для каждой операции.

### End-to-End Тесты
Для запуска end-to-end тестов в используется команда `make tests`. В Makefile определены разные скрипты для Windows (e2e-test.ps1) и Linux (e2e-test-a.sh, e2e-test-b.sh). Вы можете выбрать в Makefile нужный скрипт для запуска в зависимости от вашей операционной системы. Для запуска end-to-end тестов используйте команду: 
`make tests`
### Ожидаемые Результаты End-to-End Тестов:

**E2E Test A - Запуск аутентичной задачи A**
1. Аналогичная проверка доступности сервиса FPS.
2. Запуск аутентичной задачи A. Ожидается подтверждение успешного запуска задачи.
3. Проверка логов FPS. Успех теста подтверждается наличием записи "Task A: started" в логах. Если запись не появляется в течение 60 секунд, тест считается неудачным.


**E2E Test B - Попытка запуска неаутентичную задачу B**
1. Проверка доступности сервиса FPS. Если сервис недоступен в течение 60 секунд, тест считается неудачным.
2. Запуск неаутентичной задачи B. Ожидается подтверждение успешного запуска задачи.
3. Проверка логов FPS. Успех теста подтверждается наличием записи "Task B: not authentic" в логах. Если запись не появляется в течение 60 секунд, тест считается неудачным.
## Цели безопасности:
1. Тушение возможно только в авторизованном районе.
2. Тушение возможно только авторизованным способом.

## Полное описание компонентов:
1. **Связь** - получает полётное задание от Источника полетного задания.
2. **Контроль обстановки** - программно-аппаратные средства для оценки состояния пожарной обстановки в районе действия Огнеборца.
3. **Управление перемещением** - программно-аппаратные средства для перемещения дрона в заданную точку (на вход получает координаты, сам считает и выполняет перемещение).
4. **Контроль уровня заряда батареи** - проверяет остаточный уровень заряда аккумуляторной батареи, предоставляет информацию по запросу.
5. **Навигация** - теперь содержит две программно-аппаратные подсистемы - спутниковый навигационный приёмник и инерциальную навигационную систему (работает автономно), разработанные разными поставщиками.
6. **Тушение водой** - программно-аппаратные средства для тушения водой, для работы необходима дополнительная активация аппаратных цепей тушения.
7. **Поджигание** - программно-аппаратные средства для поджигания (запуска встречного пала), для работы необходима дополнительная активация аппаратных цепей поджига.
8. **Контроль аутентичности полётного задания** - программные средства, позволяющие достоверно проверить аутентичность полученного полётного задания.
9. **Контроль активации цепей тушения и поджига** - на основе полётного задания и текущих координат по запросу активирует необходимые аппаратные цепи, только этот компонент имеет возможность активации этих цепей, без которых тушение или поджиг невозможны. Работает непрерывно после успешной активации любой цепи, отключает цепи при нарушении условий активации.
10. **Комплексирование** - программный компонент, непрерывно анализирует информацию от двух источников навигационных данных, алгоритмы системы комплексирования позволяют обнаружить нарушение целостности навигационных данных.

## Описание сценариев нарушения целей безопасности

| Компонент | Цель безопасности 1 | Цель безопасности 2 |
|-----------|---------------------|---------------------|
| Связь | Не нарушит | Не нарушит |
| Центральная система управления | Не нарушит | Не нарушит |
| Тушение водой | Не нарушит | Не нарушит |
| Поджигание | Не нарушит | Не нарушит |
| Контроль обстановки | Не нарушит | Не нарушит |
| Управление перемещением | Нарушит | Не нарушит |
| Контроль уровня заряда батареи | Не нарушит | Не нарушит |
| Навигация GNSS | Не нарушит | Не нарушит |
| Навигация ИНС | Не нарушит | Не нарушит |
| Комплексирование | Нарушит | Не нарушит |
| Контроль аутентичности полётного задания | Нарушит | Нарушит |
| Контроль активации цепей тушения и поджига | Нарушит | Нарушит |

`+` отмечены доверенные компоненты

## Негативные сценарии

### НС-1
Связь получила изменённое полётное задание, что привело к нарушению ЦБ1 и ЦБ2.
![нс1 drawio](https://github.com/poli-poli/ogneborets_new/assets/85806007/5d821993-506c-4daa-b23d-b02920521335)



### НС-2
Комплексирование неверно вычислило текущее местонахождение БПЛА, что привело к нарушению ЦБ1.
![нс2 drawio](https://github.com/poli-poli/ogneborets_new/assets/85806007/311e67c8-6391-4b28-b9cf-72433e23634c)



### НС-3
Контроль активации цепей тушения и поджига скомпрометирован, что привело к нарушению ЦБ1 и ЦБ2.
![нс3 drawio](https://github.com/poli-poli/ogneborets_new/assets/85806007/7fe9a9d0-082a-4b7d-83d0-53da84f5df92)


### НС-4
Управление перемещением скомпрометировано, что привело к ошибке в расчете координат, что привело к нарушению ЦБ1.
![нс4 drawio](https://github.com/poli-poli/ogneborets_new/assets/85806007/479f023d-47c4-4d20-a9ac-67ee8d26d4ac)



### НС-5
Тушение водой скомпрометировано, что привело к нарушению ЦБ1 и ЦБ2.
![нс56 drawio](https://github.com/poli-poli/ogneborets_new/assets/85806007/e8035a07-80eb-4a91-be36-b776b73de7d0)


### НС-6
Поджигание скомпрометировано, что привело к нарушению ЦБ1 и ЦБ2.
![нс56 drawio](https://github.com/poli-poli/ogneborets_new/assets/85806007/27e0dae8-2f3f-4aae-85f2-b0e9e664e067)



### НС-7
Контроль аутентичности полётного задания скомпрометирован, что привело к неспособности распознать поддельное или измененное полетное задание и нарушению ЦБ1.
![нс7 drawio](https://github.com/poli-poli/ogneborets_new/assets/85806007/f377c2ad-bc74-481f-ac93-fb481edb3f2b)





## Схема с указанием доверенных компонентов
![image](https://github.com/poli-poli/ogneborets_new/assets/85806007/a3a7b5f5-3aea-4a52-91b9-708751eea964)



## Доверенные компоненты

| Компонент | Уровень доверия | Обоснование | Комментарии |
|-----------|-----------------|-------------|-------------|
| Связь | Недоверенный | — | — |
| Центральная система управления | Недоверенный | — | — |
| Тушение водой | Недоверенный | — | — |
| Поджигание | Недоверенный | — | — |
| Контроль обстановки | Недоверенный | — | — |
| Управление перемещением | Недоверенный | — | — |
| Контроль уровня заряда батареи | Недоверенный | — | — |
| Навигация GNSS | Недоверенный | — | — |
| Навигация ИНС | Недоверенный | — | — |
| Комплексирование | Доверенный | Соблюдает ЦБ1 | Комплексирование отвечает за анализ и синтез данных от систем навигации GNSS и ИНС. Точность и надёжность навигационных данных критически важны для выполнения ЦБ1. Комплексирование обеспечивает корректное слияние данных, обнаружение и коррекцию ошибок, что делает его ключевым элементом для поддержания целостности и точности навигационной информации. |
| Контроль аутентичности полётного задания | Доверенный | Соблюдает ЦБ1 и ЦБ2 | Контроль аутентичности полётного задания обеспечивает проверку и подтверждение того, что полётное задание пришло от надежного источника и не было изменено или подделано. Аутентификация и проверка подлинности данных являются фундаментальными для предотвращения несанкционированных действий и манипуляций, что делает этот компонент критически важным для обеспечения ЦБ1 и ЦБ2. |
| Контроль активации цепей тушения и поджига | Доверенный | Соблюдает ЦБ1 и ЦБ2 | Контроль активации цепей тушения и поджига отвечает за активацию и деактивацию механизмов тушения водой и поджига. Он обеспечивает выполнение тушения водой или поджига только при соответствующих условиях и только после получения подтвержденного полётного задания, что предотвращает случайное или несанкционированное использование этих механизмов, что обеспечивает выполнение ЦБ1 и ЦБ2. |

## Качественная оценка доменов

| Компонент | Доверие | Оценка | Комментарий |
|-----------|---------|--------|-------------|
| Связь | Недоверенный | cL | Получает полётное задание от Источника полетного задания. |
| Центральная система управления | Недоверенный | cL | Программно-аппаратные средства для оценки состояния пожарной обстановки в районе действия Огнеборца. |
| Тушение водой | Недоверенный | sS | Программно-аппаратные средства для перемещения дрона в заданную точку (на вход получает координаты, сам считает и выполняет перемещение). |
| Поджигание | Недоверенный | sS | Проверяет остаточный уровень заряда аккумуляторной батареи, предоставляет информацию по запросу. |
| Контроль обстановки | Недоверенный | sM | Теперь содержит две программно-аппаратные подсистемы - спутниковый навигационный приёмник и инерциальную навигационную систему (работает автономно), разработанные разными поставщиками. |
| Управление перемещением | Недоверенный | cXL | Программно-аппаратные средства для тушения водой, для работы необходима дополнительная активация аппаратных цепей тушения. |
| Контроль уровня заряда батареи | Недоверенный | sS | Программно-аппаратные средства для поджигания (запуска встречного пала), для работы необходима дополнительная активация аппаратных цепей поджига. |
| Навигация GNSS | Недоверенный | mM | Программные средства, позволяющие достоверно проверить аутентичность полученного полётного задания. |
| Навигация ИНС | Недоверенный | mM | На основе полётного задания и текущих координат по запросу активирует необходимые аппаратные цепи, только этот компонент имеет возможность активации этих цепей, без которых тушение или поджиг невозможны. Работает непрерывно после успешной активации любой цепи, отключает цепи при нарушении условий активации. |
| Комплексирование | Доверенный | sS | Программный компонент, непрерывно анализирует информацию от двух источников навигационных данных, алгоритмы системы комплексирования позволяют обнаружить нарушение целостности навигационных данных. |
| Контроль аутентичности полётного задания | Доверенный | sM | Получает полётное задание от Источника полетного задания. |
| Контроль активации цепей тушения и поджига | Доверенный | sS | Программно-аппаратные средства для оценки состояния пожарной обстановки в районе действия Огнеборца. |

Качественные оценки доменов: 
- оценка сложности кодовой базы:
  - c — complex (сложный);
  - m — medium (средней сложности);
  - s — simple (простой);
- оценка объёма кодовой базы:
  - S — small (малый);
  - M — medium (среднего объёма);
  - L — large (большой);
  - XL — extra large (огромный).

## Проверка негативных сценариев

| Название | Описание |
|----------|----------|
| НС-1 | Связь получила изменённое полётное задание, что привело к нарушению ЦБ1 и ЦБ2. |
| НС-2 | Комплексирование неверно вычислило текущее местонахождение БПЛА, что привело к нарушению ЦБ1. |
| НС-3 | Контроль активации цепей тушения и поджига скомпрометирован, что привело к нарушению ЦБ1 и ЦБ2. |
| НС-4 | Управление перемещением скомпрометировано, что привело к ошибке в расчете координат, что привело к нарушению ЦБ1. |
| НС-5 | Тушение водой скомпрометировано, что привело к нарушению ЦБ1 и ЦБ2. |
| НС-6 | Поджигание скомпрометировано, что привело к нарушению ЦБ1 и ЦБ2. |
| НС-7 | Контроль аутентичности полётного задания скомпрометирован, что привело к неспособности распознать поддельное или измененное полетное задание и нарушению ЦБ1. |

## Сопоставление компонент с кодом

| Компонент | Доверие |
|-----------|---------|
| Связь | Недоверенный |
| Центральная система управления | CentralControlSystem |
| Тушение водой | ExtinguishingControlSystem |
| Поджигание | FireIgnitionSystem |
| Контроль обстановки | SituationControlSystem |
| Управление перемещением | MovementControlSystem |
| Контроль уровня заряда батареи | BatteryChargeControlSystem |
| Навигация GNSS | GNSSNavigationSystem |
| Навигация ИНС | INSNavigationSystem |
| Комплексирование | ComplexingSystem |
| Контроль аутентичности полётного задания | FlightTaskAuthenticator |
| Контроль активации цепей тушения и поджига | FireExtinguishingSystem |

В компонент Связь входят компоненты FPS (Flight Processing System) и Connection. Они выполняют следующие функции:

- FPS (Flight Processing System): отвечает за обработку задач, связанных с полетами. В его задачи входит отслеживание полетных заданий. 

- Connection: действует как интерфейс связи в системе. Он может ответственный за управление подключениями между различными системами, обработку передачи данных и обеспечение надежной связи между внешним источником полётных задач и системой.

Оба компонента критически важны для обеспечения эффективной и координированной работы в рамках более широкой архитектуры системы.
## Политики безопасности
| Источник | Получатель | Операция | Результат |
|--------|-------------|-----------|--------|
| FlightTaskAuthenticator | CentralControlSystem | start_extinguishing | true |
| CentralControlSystem | ComplexingSystem | request_coordinates | true |
| CentralControlSystem | MovementControlSystem | move_to_area | true |
| MovementControlSystem | CentralControlSystem | movement_completed | true |
| CentralControlSystem | ExtinguishingControlSystem | start_extinguishing | true |
| CentralControlSystem | ExtinguishingControlSystem | start_ignition | true |
| CentralControlSystem | SituationControlSystem | check_extinguishing_status | true |
| CentralControlSystem | ExtinguishingControlSystem | stop_extinguishing | true |
| CentralControlSystem | BatteryChargeControlSystem | check_battery | true |
| GNSSNavigationSystem | ComplexingSystem | send_coordinates | true |
| INSNavigationSystem | ComplexingSystem | send_coordinates | true |
| ComplexingSystem | CentralControlSystem | coordinates | true |
| SituationControlSystem | CentralControlSystem | extinguishing_status | true |
| ExtinguishingControlSystem | CentralControlSystem | extinguishing_completed | true |
| ExtinguishingControlSystem | CentralControlSystem | ignition_completed | true |
| CentralControlSystem | FireIgnitionSystem | activate_ignition | true |
| FireIgnitionSystem | CentralControlSystem | ignition_activated | true |
| ComplexingSystem | GNSSNavigationSystem | request_coordinates | true |
| ComplexingSystem | INSNavigationSystem | request_coordinates | true |
| GNSSNavigationSystem | ComplexingSystem | coordinates | true |
| INSNavigationSystem | ComplexingSystem | coordinates | true |
| CentralControlSystem | SituationControlSystem | request_situation_update | true |
| SituationControlSystem | CentralControlSystem | situation_update | true |
| CentralControlSystem | MovementControlSystem | request_movement_update | true |
| MovementControlSystem | CentralControlSystem | movement_update | true |
| CentralControlSystem | FireExtinguishingSystem | activate_extinguishing | true |
| FireExtinguishingSystem | CentralControlSystem | extinguishing_activated | true |
| BatteryChargeControlSystem | CentralControlSystem | battery_status | true |
| Connection | FlightTaskAuthenticator | _ | true | *test policies* |
| FlightTaskAuthenticator | CentralControlSystem | _ | true |*test policies* |
| FlightTaskAuthenticator | ExtinguishingControlSystem | _ | true |*test policies* |




## Типы заданий для монитора безопасности
1. Пуск процедуры тушения в районе А.
2. Запрос текущих координат и получение координат от комплексирования.
3. Команда перемещения в район А.
4. Получение сигнала о завершении перемещения.
5. Команда тушения/поджига.
6. Запрос и получение ответа от контроля обстановки о запуске процедуры тушения/поджига.
7. Команда о завершении алгоритма тушения.
8. Контроль целостности координат и отправка ответа с координатами.
9. Приём запроса на координаты от комплексирования и отправка координат.
10. Обработка запроса на проверку запущен ли алгоритм тушения и отправка ответа.
11. Команда на активацию цепей тушения и поджига.
12. Информация об остановке тушения или поджига.
13. Запрос на контроль уровня заряда батареи и отправка информации.


