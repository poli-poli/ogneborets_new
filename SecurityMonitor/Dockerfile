# Используем базовый образ Ubuntu версии 20.04
FROM ubuntu:20.04

# Установка временной зоны
ENV TZ=Asia/Dubai
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Обновление списка пакетов и установка необходимых пакетов
RUN apt-get update -y \
    && apt-get upgrade -y \
    && apt-get install -y \
    g++ \             
    pkg-config \     
    python3 \          
    python3-pip \
    cargo       

# Установка модуля pika для Python
RUN pip3 install pika

# Создание рабочего каталога
WORKDIR /app

# Копирование исходного кода в контейнер
COPY . /app

# Копирование скрипта wait_for_rabbitmq.py в контейнер
COPY wait_for_rabbitmq.py /app

# Сборка проекта
RUN cargo build

# Запуск юнит-тестов
RUN cargo test
