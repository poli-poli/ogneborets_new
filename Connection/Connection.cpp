#include <iostream>
#include <httplib.h>
#include "Rabbitmq.h"
#include <string>

using namespace std;
using namespace httplib;

const int PORT = 8082;

void handle_api(const Request& req, Response& res) {
    string task = req.get_param_value("task");
    string command = req.get_param_value("command");

    if (command == "start" && !task.empty()) {
        cout << "Received task: " << task << endl;
        res.set_content("Task " + task + " started successfully", "text/plain");
    } else {
        res.set_content("Invalid request", "text/plain");
    }
}

int main() {
    cout << "Starting Connection at http://0.0.0.0:" << PORT << endl;

    Server svr;
    svr.Get("/api", handle_api);
    svr.listen("0.0.0.0", PORT);

    return 0;
}
