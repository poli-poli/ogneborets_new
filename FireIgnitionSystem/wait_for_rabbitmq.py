#!/usr/bin/env python3

import os
import pika
import time
import sys

def wait_for_rabbitmq(host, username, password):
    credentials = pika.PlainCredentials(username, password)
    connection = None
    while True:
        try:
            connection = pika.BlockingConnection(
                pika.ConnectionParameters(host=host, credentials=credentials)
            )
            # Проверяем, что соединение установлено
            if connection.is_open:
                print("RabbitMQ is up and running.")
                break
        except pika.exceptions.AMQPConnectionError:
            print("RabbitMQ is not available yet. Retrying...")
            time.sleep(5)
        finally:
            # Закрываем соединение после проверки
            if connection and connection.is_open:
                connection.close()

if __name__ == "__main__":
    rabbitmq_host = os.environ["RABBIT_HOST"]
    rabbitmq_user = os.environ["RABBIT_USER"]
    rabbitmq_pass = os.environ["RABBIT_PASS"]
    wait_for_rabbitmq(rabbitmq_host, rabbitmq_user, rabbitmq_pass)
