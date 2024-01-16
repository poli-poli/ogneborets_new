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
    make \             
    libssl-dev \        
    git \              
    cmake \             
    pkg-config \        
    python3 \           
    python3-pip 

# Установка модуля pika для Python (клиент RabbitMQ)
RUN pip3 install pika




# Установка рабочего каталога в контейнере
WORKDIR /app

# Копирование исходного кода в контейнер
COPY . /app

# Компиляция приложения
RUN g++ -o FPS FPS.cpp -L/usr/local/lib -std=c++11 -pthread

# Запуск скрипта wait_for_rabbitmq.py, затем приложения
CMD ["sh", "-c", "python3 wait_for_rabbitmq.py && ./FPS"]
