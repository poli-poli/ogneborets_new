#include <iostream>

#include <nlohmann/json.hpp>


#include "Rabbitmq.h"

using namespace std;
using json = nlohmann::json;
using MessageHandlerFunction = std:: function < int(AMQPMessage * ) > ;

int main(void) {
    cout << "GNSS navigation started"  << "\n";
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
    const char * queue_name1 = "GNSSNavigationSystem";
    const string consumer_tag = "GNSSNavigationSystem";

    cout << "Starting publisher "  << "\n";

    Publisher publisher(conn_str, exchange_name, queue_name);

    MessageHandlerFunction handler = [ & ](AMQPMessage * message) -> int {
        uint32_t j = 0;
        char * data = message -> getMessage( & j);
        if (data) {

            json receivedMsg = json::parse(data);
            string operation = receivedMsg["operation"];
            cout << "Received operation: " << operation << endl;

            if (operation == "request_coordinates") {
                //process operation
                json responseMsg;
                responseMsg["source"] = "GNSSNavigationSystem";
                responseMsg["data"] = 1;
                responseMsg["operation"] = "send_coordinates";
                responseMsg["deliver_to"] = "ComplexingSystem";

                string responseMessage = responseMsg.dump();
                publisher.sendMessage("SecurityMonitor", responseMessage);
                cout << "Sent response message: " << responseMessage << "\n";

            }
        }
        return 0;
    };

    cout << "Starting consumer "  << "\n";

    Consumer consumer(conn_str, exchange_name, queue_name1, consumer_tag, handler);

    consumer.startConsuming();

    return 0;
}