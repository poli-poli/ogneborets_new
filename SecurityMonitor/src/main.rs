

mod check_operation;
mod test;

use lapin::{
    BasicProperties, Channel, Connection, ConnectionProperties, message::Delivery,
    options::*, types::FieldTable,
};
use serde_json::Value;
use futures_util::stream::StreamExt;
use check_operation::check_operation;


// Обрабатывает новое сообщение, полученное из RabbitMQ.
async fn on_new_delivery(channel: &Channel, delivery: &Delivery) {
    println!("Received message: {:?}", delivery);
    let json_str = match std::str::from_utf8(&delivery.data) {
        Ok(str) => str,
        Err(_) => {
            eprintln!("Failed to decode message as UTF-8");
            return;
        }
    };

    let json: Value = match serde_json::from_str(json_str) {
        Ok(json) => json,
        Err(_) => {
            eprintln!("Failed to parse JSON");
            return;
        }
    };
    println!("Parsed JSON: {:?}", json);

    if !(check_operation(&json)) {
        println!("BAD POLICIES!!!!!!!!");
        return;
    }

    let deliver_to = json.get("deliver_to")
        .and_then(Value::as_str)
        .unwrap();

    channel.basic_publish(
    &delivery.exchange.as_str(),
    deliver_to,
        BasicPublishOptions::default(),
        &delivery.data,
        BasicProperties::default(),
        
    )
        .await
        .expect("Failed to retranslate delivery");

    println!("Success");
}

/// Инициализирует RabbitMQ, создавая необходимые очереди.
async fn init_rabbit(channel: &Channel) {
    let opt = QueueDeclareOptions {
        passive: false,
        durable: true,
        exclusive: false,
        auto_delete: false,
        nowait: false,
    };

    // Объявление очередей
    let queues = [
        "SecurityMonitor", "BatteryChargeControlSystem", "CentralControlSystem",
        "ComplexingSystem", "ExtinguishingControlSystem", "FireExtinguishingSystem",
        "FireIgnitionSystem", "FlightTaskAuthenticator", "GNSSNavigationSystem",
        "INSNavigationSystem", "MovementControlSystem", "Connection", 
        "FPS",
    ];

    for queue in queues.iter() {
        channel.queue_declare(
            queue,
            opt,
            FieldTable::default(),
        ).await.expect("Failed to declare queue");
    }
}

#[tokio::main]
async fn main() {
    // Получение переменных окружения для подключения к RabbitMQ
    let host: String = std::env::var("RABBIT_HOST").expect("RABBIT_HOST not set");
    let user: String = std::env::var("RABBIT_USER").expect("RABBIT_USER not set");
    let password: String = std::env::var("RABBIT_PASS").expect("RABBIT_PASS not set");
    let vhost: String = std::env::var("RABBIT_VHOST").expect("RABBIT_VHOST not set");

    let addr = format!("amqp://{}:{}@{}:5672/{}", user, password, host, vhost);

    println!("Trying connect to {addr}");

    // Подключение к RabbitMQ
    let conn = Connection::connect(&addr, ConnectionProperties::default())
        .await
        .expect("connection error");

    println!("Connected!");

    let channel = conn.create_channel().await.expect("create_channel error");
    init_rabbit(&channel).await;
    
    println!("All queues has been initiated!");

    // Подписка на очередь и обработка сообщений
    let mut consumer = channel
        .basic_consume(
            "SecurityMonitor",
            "monitor",
            BasicConsumeOptions::default(),
            FieldTable::default(),
        )
        .await
        .expect("Failed to consume from SecurityMonitor");

    println!("Consumer has been started!");

    while let Some(delivery) = consumer.next().await {
        println!("message!");
        let delivery = delivery.expect("error in consumer"); 
        on_new_delivery(&channel, &delivery).await;
        delivery.ack(BasicAckOptions::default())
            .await
            .expect("Failed to process the delivery");
    }
}
