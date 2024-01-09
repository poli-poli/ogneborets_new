#include <iostream>
#include <nlohmann/json.hpp>
#include "Rabbitmq.h"
#include <chrono>
#include <thread>
#include <set>

using namespace std;
using json = nlohmann::json;

int main(void) {
    char *host = std::getenv("RABBIT_HOST");
    char *user = std::getenv("RABBIT_USER");
    char *password = std::getenv("RABBIT_PASS");
    char *vhost = std::getenv("RABBIT_VHOST");

    if (!host || !user || !password || !vhost) {
        cerr << "One or more environment variables are not set." << endl;
        return 1;
    }

    string conn_str = string(user) + ":" + string(password) + "@" + string(host) + "/" + string(vhost);
    const char *exchange_name = "monitor";
    const char *queue_name = "SecurityMonitor";
    const char *queue_name1 = "FlightTaskAuthenticator";
    const string consumer_tag = "FlightTaskAuthenticator";

    Publisher publisher(conn_str, exchange_name, queue_name);

    // Список валидных задач
    set<string> validTasks = {"start_extinguishing1", "start_extinguishing2"};

    MessageHandlerFunction handler = [&](AMQPMessage *message) -> int {
        uint32_t j = 0;
        char *data = message->getMessage(&j);
        if (!data) return 0;

        try {
            json receivedMsg = json::parse(data);
            string task = receivedMsg.value("task", "");

            if (validTasks.find(task) != validTasks.end()) {
                cout << "Received valid task: " << task << endl;
              
            } else {
                cout << "Invalid task received: " << task << endl;
            }
        } catch (const json::parse_error &e) {
            cerr << "JSON parsing error: " << e.what() << endl;
        }
        return 0;
    };


    Consumer consumer(conn_str, exchange_name, queue_name1, consumer_tag, handler);
    consumer.startConsuming();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
