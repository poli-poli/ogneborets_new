#include <AMQPcpp.h>
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
using MessageHandlerFunction = std::function<int(AMQPMessage*)>;

class Consumer {
private:
    AMQP *amqp;
    AMQPQueue *qu2;
    AMQPExchange *ex;
    std::string lastOperation;

public:
    Consumer(const string &conn_str, const string &exchange_name,
             const string &queue_name, const string &consumer_tag,
             MessageHandlerFunction messageHandler) {
        amqp = new AMQP(conn_str);
        ex = amqp->createExchange(exchange_name);
        ex->Declare(exchange_name, "direct", AMQP_DURABLE);

        qu2 = amqp->createQueue(queue_name);
        qu2->Declare(queue_name, AMQP_DURABLE);
        qu2->Bind(exchange_name, queue_name);
        qu2->setConsumerTag(consumer_tag);

        qu2->addEvent(AMQP_MESSAGE, messageHandler);
    }

    ~Consumer() { delete amqp; }

    virtual void startConsuming() {
        try {
            qu2->Consume(AMQP_NOACK);
        } catch (const AMQPException& e) {
            cerr << "Ошибка при запуске приема сообщений: " << e.getMessage() << endl;
        }
    }

    std::string getLastOperation() const {
        return lastOperation;
    }
};

class Publisher {
private:
    AMQP *amqp;
    AMQPExchange *ex;

public:
    Publisher(const string &conn_str, const string &exchange_name, const string &queue_name)
        : amqp(new AMQP(conn_str)), ex(amqp->createExchange(exchange_name)) {
        ex->Declare(exchange_name, "direct", AMQP_DURABLE);
    }

    ~Publisher() { delete amqp; }

    virtual void sendMessage(const string &routing_key, const string &message) {
        ex->Publish(message, routing_key);
        cout << "Sent message: " << message << "\n";
    }
};
