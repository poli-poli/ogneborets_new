#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Rabbitmq.h"  
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// Функция для обработки сообщений
string handleMessage(const string& data) {
    try {
        json receivedMsg = json::parse(data);
        return receivedMsg["operation"];
    } catch (const json::parse_error &e) {
        cerr << "JSON parsing error: " << e.what() << endl;
        return "";
    }
}

// Тестовый класс
class MessageHandlerTest : public ::testing::Test {
protected:
    
};

// Тест для проверки корректной обработки сообщений
TEST_F(MessageHandlerTest, CorrectMessageHandling) {
    string testData = R"({"operation": "check_battery"})";
    string expectedOperation = "check_battery";
    ASSERT_EQ(handleMessage(testData), expectedOperation);
}

// Тест для проверки обработки некорректного JSON
TEST_F(MessageHandlerTest, IncorrectJsonHandling) {
    string testData = "invalid json";
    ASSERT_EQ(handleMessage(testData), "");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    cout << "test ok";
}
