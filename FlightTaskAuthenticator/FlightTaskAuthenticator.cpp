#include <functional>

#include <iostream>

#include <nlohmann/json.hpp>

#include <chrono>

#include <thread>

#include <vector>

#include "Rabbitmq.h"

using namespace std;
using json = nlohmann::json;
using MessageHandlerFunction = std:: function < int(AMQPMessage * ) > ;

int main(void) {
    char * host = std::getenv("RABBIT_HOST");
    char * user = std::getenv("RABBIT_USER");
    char * password = std::getenv("RABBIT_PASS");
    char * vhost = std::getenv("RABBIT_VHOST");

    if (!host || !user || !password || !vhost) {
      cerr << "One or more environment variables are not set." << endl;
      return 1;
    }
    string conn_str = string(user) + ":" + string(password) + "@" + string(host) + "/" + string(vhost);
    const char * exchange_name = "monitor";
    const char * queue_name = "SecurityMonitor";
    const char * queue_name1 = "FlightTaskAuthenticator";
    const string consumer_tag = "FlightTaskAuthenticator";

    vector < string > validTasks = {
      "start_extinguishing1",
      "start_extinguishing2"
    };

    Publisher publisher(conn_str, exchange_name, queue_name);

    MessageHandlerFunction handler = [ & ](AMQPMessage * message) -> int {
        uint32_t j = 0;
        char * data = message -> getMessage( & j);
        if (data) {
          try {
            json receivedMsg = json::parse(data);
            string operation = receivedMsg["operation"];
            cout << "Received operation: " << operation << endl;

            if(find(validTasks.begin(), validTasks.end(), operation) != validTasks.end()) {
              if (operation == "start_extinguishing1") {
                json msg;
                msg["source"] = "FlightTaskAuthenticator";
                msg["data"] = "Extinguishing task";
                msg["operation"] = "start_extinguishing";
                msg["deliver_to"] = "CentralControlSystem";

                string message = msg.dump();
                publisher.sendMessage("SecurityMonitor", message);
              }

              if (operation == "start_extinguishing2") {
                json msg;
                msg["source"] = "FlightTaskAuthenticator";
                msg["data"] = "Extinguishing task";
                msg["operation"] = "start_extinguishing";
                msg["deliver_to"] = "ExtinguishingControlSystem";

                string message = msg.dump();
                publisher.sendMessage("SecurityMonitor", message);
              }
            }

              } catch (const json::parse_error & e) {
                cerr << "JSON parsing error: " << e.what() << endl;
              }
            }
            return 0;
          };

          Consumer consumer(conn_str, exchange_name, queue_name1, consumer_tag, handler);

          consumer.startConsuming();

          return 0;
        }