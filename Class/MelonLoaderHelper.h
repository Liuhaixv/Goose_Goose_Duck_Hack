#pragma once
#include<string>

class MelonLoaderHelper {
public:
    static bool testConnection() {
        return send_and_receive(1234, "TestConnection") >= 0;
    }

    static bool sendChat(std::string chatMessage) {
        std::string message = "SendChat\n";
        return sendMessage(message.append(chatMessage).c_str());
    }

    static bool moveShuttle() {
        std::string message = "MoveShuttle";
        return sendMessage(message.c_str());
    }

private:
    static bool sendMessage(const char* message) {
        return send_and_receive(1234, message) >= 0;
    }
    
private:
    static int send_and_receive(int server_port, const char* message);
};


