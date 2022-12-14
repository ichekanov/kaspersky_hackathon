# Kaspersky hackathon 2022

Робот под управлением операционной системы KasperskyOS, принимающий команды по протоколу MQTT. Реализовано управление в «ручном» режиме; планируется автоматический объезд зоны по заданным точкам, которые распознаются при помощи компьютерного зрения.

**Особенности:**
- Киберимунность системы
- Удалённое управление
- Легковесность протокола управления
- Масштабируемость

## Квалификационный этап хакатона

Задания квалификационного этапа хакатона [здесь](Задание_квалификационного_этапа.pdf).

Результат выполнения задания **2** находится в папке [«hello»](hello): [results.txt](hello/results.txt).

Результат выполнения задания **3** находится в папке [«mosquitto/mqtt_subscriber»](mosquitto/mqtt_subscriber).

## Основной этап хакатона

Задания основного этапа хакатона [здесь](Задание_основного_этапа.pdf).

Результат выполнения задания хакатона находится в папке [«alphabot»](alphabot/).

Система состоит из двух модулей (сущностей): **Subscriber** и **ShowApp**. Первый отвечает за получение команд от MQTT брокера, второй — за управление моторами. Такое разделение обусловлено принципами кибербезопасности, лежащими в основе системы KasperskyOS. Связь между модулями осуществляется по стандарту IPC. 

Документация к операционной системе и инструкции по запуску кода находятся [в разделе KasperskyOS для разработчиков](https://support.kaspersky.ru/help/KCE/1.1/ru-RU/whats_new.htm).

По результатам хакатона в полном объёме выполнено задание 1 «Непосредственное управление». Задание 2 «Движение по маршруту» выполнено на уровне алгоритмов и математических вычислений, но не доведено до конца в реализации для ОС.

Для управления роботом в ручном режиме с помощью клавиатуры мы разработали [скрипт на языке Python](control.py). 

![IMAGE 2022-11-20 18:27:33](https://user-images.githubusercontent.com/78589729/202910718-4e74e58d-f974-40f8-a66e-dc71e65f08d8.jpg)

## Команда

Название: **«Мысли Жака Фреско»**
- [Абрамов Илья](https://github.com/Eagle-008)
- [Созинов Кирилл](https://github.com/KirillSozin)
- [Торопин Иван](https://github.com/Lhesnor)
- [Чеканов Иван](https://github.com/ichekanov)

## Лицензия 
[**MIT License**](LICENSE)

**(с) Kaspersky 2022**
