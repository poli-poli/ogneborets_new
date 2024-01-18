#include <iostream>

#include <nlohmann/json.hpp>


#include "Rabbitmq.h"

using namespace std;
using json = nlohmann::json;
using MessageHandlerFunction = std:: function < int(AMQPMessage * ) > ;

int main(void) {

    cout << "CentralControlSystem started "  << "\n";

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
    const char * queue_name1 = "CentralControlSystem";
    const string consumer_tag = "CentralControlSystem";

    cout << "Starting publisher "  << "\n";

    Publisher publisher(conn_str, exchange_name, queue_name);

    MessageHandlerFunction handler = [ & ](AMQPMessage * message) -> int {
        uint32_t j = 0;
        char * data = message -> getMessage( & j);
        if (data) {

            json receivedMsg = json::parse(data);
            string operation = receivedMsg["operation"];
            cout << "Received operation: " << operation << endl;

            if (operation == "start_extinguishing") { //test
                json msg;
                msg["source"] = "CentralControlSystem";
                msg["data"] = "Extinguishing task";
                msg["operation"] = "check_battery";
                msg["deliver_to"] = "BatteryChargeControlSystem";

                string message = msg.dump();
                publisher.sendMessage("SecurityMonitor", message);
            }

            if (operation == "move_to_area") {
                //process operation
                json msg;
                msg["source"] = "CentralControlSystem";
                msg["data"] = "Area coordinates";
                msg["operation"] = "move_to_area";
                msg["deliver_to"] = "MovementControlSystem";

                string message = msg.dump();
                publisher.sendMessage("SecurityMonitor", message);
                cout << "Sent message: " << message << "\n";
            }

            if (operation == "activate_extinguishing") {
                //process operation
                json msg;
                msg["source"] = "CentralControlSystem";
                msg["data"] = "Activate extinguishing";
                msg["operation"] = "activate_extinguishing";
                msg["deliver_to"] = "FireExtinguishingSystem";

                string message = msg.dump();
                publisher.sendMessage("SecurityMonitor", message);
                cout << "Sent message: " << message << "\n";
            }

            if (operation == "activate_ignition") {
                //process operation
                json msg;
                msg["source"] = "CentralControlSystem";
                msg["data"] = "Command to activate ignition";
                msg["operation"] = "activate_ignition";
                msg["deliver_to"] = "FireIgnitionSystem";

                string message = msg.dump();
                publisher.sendMessage("SecurityMonitor", message);
                cout << "Sent message: " << message << "\n";
            }

            if (operation == "request_coordinates") {
                //process operation
                json msg;
                msg["source"] = "CentralControlSystem";
                msg["data"] = "Request for GNSS coordinates";
                msg["operation"] = "request_coordinates";
                msg["deliver_to"] = "GNSSNavigationSystem";

                string message = msg.dump();
                publisher.sendMessage("SecurityMonitor", message);
                cout << "Sent message: " << message << "\n";
            }

            if (operation == "request_movement_update") {
                //process operation
                json msg;
                msg["source"] = "CentralControlSystem";
                msg["data"] = "Request for movement update";
                msg["operation"] = "request_movement_update";
                msg["deliver_to"] = "MovementControlSystem";

                string message = msg.dump();
                publisher.sendMessage("SecurityMonitor", message);
                cout << "Sent message: " << message << "\n";
            }

        }
        return 0;
    };

    cout << "Starting consumer "  << "\n";

    Consumer consumer(conn_str, exchange_name, queue_name1, consumer_tag, handler);

    consumer.startConsuming();

    return 0;
}