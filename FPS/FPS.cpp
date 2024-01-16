#include <iostream> 
#include "httplib.h"

#include <string>
#include <vector>


using namespace std;
using namespace httplib;


const int PORT = 8081;
const string CONNECTION_HOST = "connection"; 
const int CONNECTION_PORT = 8082;

vector<vector<string>> tasks;
bool start_send = false;
string task = "";



void handle_get(const Request& req, Response& res) {
    //  логика обработки GET запросов
}


void sendTaskToConnection(const string& task) {
    Client cli(CONNECTION_HOST, CONNECTION_PORT);
    auto res = cli.Get(("/api?task=" + task + "&command=start").c_str());
    if (res) {
        cout << "Response status: " << res->status << endl;
        cout << "Response body: " << res->body << endl;
    } else {
        cout << "Failed to communicate with Connection" << endl;
    }
}


void handle_api(const Request& req, Response& res) {
    string task = req.get_param_value("task");
    string command = req.get_param_value("command");

    if (command == "start") {
        string status = (task == "A") ? "started" : "not authentic";
        tasks.push_back({task, status});
        res.set_content("Task " + task + " " + status, "text/plain");
    } else if (req.has_param("tasks_log")) {
        string log;
        for (const auto& t : tasks) {
            log += "Task " + t[0] + ": " + t[1] + "\n";
        }
        res.set_content(log, "text/plain");
    } else {
        res.set_content("Invalid request", "text/plain");
    }
}


int main() {
    cout << "Starting FPS at http://0.0.0.0:" << PORT << endl;


    Server svr;
    string task = "start_extinguishing1";
    sendTaskToConnection(task);
    svr.Get("/", handle_get);
    svr.Get("/api", handle_api);

    svr.listen("0.0.0.0", PORT);

 
    return 0;
}