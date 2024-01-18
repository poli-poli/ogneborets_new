#include <httplib.h>
#include "Rabbitmq.h"
#include <nlohmann/json.hpp>
#include <thread>


using json = nlohmann::json;
using namespace std;
using namespace httplib;

const int PORT = 8082;

void handle_api(const Request & req, Response & res) {
  string task = req.get_param_value("task");
  string command = req.get_param_value("command");

  if (command == "start" && !task.empty()) {
    cout << "Received task: " << task << endl;
    res.set_content("Task " + task + " started successfully", "text/plain");
    char * host = std::getenv("RABBIT_HOST");
    char * user = std::getenv("RABBIT_USER");
    char * password = std::getenv("RABBIT_PASS");
    char * vhost = std::getenv("RABBIT_VHOST");

    if (!host || !user || !password || !vhost) {
      cerr << "One or more environment variables are not set." << endl;
    }
     string conn_str = string(user) + ":" + string(password) + "@" + string(host) + "/" + string(vhost);
    const char * exchange_name = "monitor";
    const char * queue_name = "SecurityMonitor";
    const char * queue_name1 = "Connection";
    const string consumer_tag = "Connection";

    cout << "Starting publisher  " << task << endl;

    Publisher publisher(conn_str, exchange_name, queue_name);

    json msg;
    msg["source"] = "Connection";
    msg["data"] = "Command to activate extinguishing";
    msg["operation"] = "start_extinguishing1";
    msg["deliver_to"] = "FlightTaskAuthenticator";

    string message = msg.dump();
    publisher.sendMessage("SecurityMonitor", message);

  } else {
    res.set_content("Invalid request", "text/plain");
  }
}

int main() {

  cout << "Starting Connection at http://0.0.0.0:" << PORT << endl;

  Server svr;
  cout << "Starting consumer"  << endl;
  svr.Get("/api", handle_api);
  svr.listen("0.0.0.0", PORT);

  return 0;
}