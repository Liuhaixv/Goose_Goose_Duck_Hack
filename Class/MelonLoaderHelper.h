#pragma once
#include<string>

class MelonLoaderHelper {
public:
    static bool testConnection() {
        return send_and_receive(port, "TestConnection");
    }

    static bool sendChat(std::string chatMessage) {
        std::string message = "SendChat\n";
        return sendMessage(message.append(chatMessage).c_str());
    }

    static bool moveShuttle() {
        std::string message = "MoveShuttle";
        return sendMessage(message.c_str());
    }

    static bool suicide() {
        std::string message = "Suicide";
        return sendMessage(message.c_str());
    }

    static bool remoteKill(std::string userId) {
        std::string message = "RemoteKill\n";
        return sendMessage((message + userId).c_str());
    }

private:
    static const int port = 1234;

    static bool sendMessage(const char* message) {
        return send_and_receive(port, message);
    }
    
private:
    static bool send_and_receive(int server_port, const char* message);
};


