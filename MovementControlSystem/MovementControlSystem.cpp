#include <iostream>

#include <nlohmann/json.hpp>

#include "Rabbitmq.h"

#include <chrono>

#include <thread>

using namespace std;
using json = nlohmann::json;

int main(void) {
    char * host = std::getenv("RABBIT_HOST");
    char * user = std::getenv("RABBIT_USER");
    char * password = std::getenv("RABBIT_PASS");
    char * vhost = std::getenv("RABBIT_VHOST");

    if (!host || !user || !password || !vhost) {
        cerr << "One or more environment variables (RABBIT_HOST, RABBIT_USER, RABBIT_PASS, RABBIT_VHOST) are not set." << endl;
        return 1;
    }

    string conn_str = string(user) + ":" + string(password) + "@" + string(host) + "/" + string(vhost);
    const char * exchange_name = "monitor";
    const char * queue_name = "SecurityMonitor";
    const char * queue_name1 = "MovementControlSystem";
    const string consumer_tag = "MovementControlSystem";

    Publisher publisher(conn_str, exchange_name, queue_name);

    MessageHandlerFunction handler = [](AMQPMessage * message) -> int {
        uint32_t j = 0;
        char * data = message -> getMessage( & j);
        if (data) {
            try {
                json receivedMsg = json::parse(data);
                string operation = receivedMsg["operation"];
                cout << "Received operation: " << operation << endl;
            } catch (const json::parse_error & e) {
                cerr << "JSON parsing error: " << e.what() << endl;
            }
        }
        return 0;
    };

    Consumer consumer(conn_str, exchange_name, queue_name1, consumer_tag, handler);
    consumer.startConsuming();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    string lastOperation = consumer.getLastOperation();
    cout << "Last operation: " << lastOperation << endl;

    if (lastOperation == "movement_update") {
    json msg;
    msg["source"] = "MovementControlSystem";
    msg["data"] = "Current movement status";
    msg["operation"] = "movement_update";
    msg["deliver_to"] = "CentralControlSystem";

    string message = msg.dump();
    publisher.sendMessage("SecurityMonitor", message);
    }


    return 0;
}