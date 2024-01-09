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

# Клонирование и сборка библиотеки rabbitmq-c (C-клиент для RabbitMQ)
RUN git clone --branch v0.11.0 https://github.com/alanxz/rabbitmq-c.git \
    && cd rabbitmq-c \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX=/usr .. \
    && cmake --build . --target install

# Клонирование и сборка библиотеки AMQPcpp (C++ клиент для AMQP)
RUN git clone https://github.com/akalend/amqpcpp.git \
    && cd amqpcpp \
    && sed -i 's|../rabbitmq-c/|/usr/include/|' Makefile \
    && make \
    && make install \
    && ldconfig

# Скачивание и установка библиотеки nlohmann/json (JSON для Modern C++)
RUN git config --global http.postBuffer 524288000 \
    && git clone https://github.com/nlohmann/json.git \
    && cp -r json/single_include/nlohmann /usr/include/

# Установка рабочего каталога в контейнере
WORKDIR /app

# Копирование исходного кода в контейнер
COPY . /app

# Компиляция приложения
RUN g++ -o MovementControlSystem MovementControlSystem.cpp -L/usr/local/lib -lamqpcpp -lrabbitmq

# Запуск скрипта wait_for_rabbitmq.py, затем приложения
CMD ["sh", "-c", "python3 /tmp/wait_for_rabbitmq.py && ./MovementControlSystem"]
