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

    static bool unlockAllItems() {
        std::string message = "UnlockAllItems";
        return sendMessage(message.c_str());
    }

    static bool silencePlayer(std::string userId) {
        std::string message = "SilencePlayer\n";
        return sendMessage((message + userId).c_str());
    }

    static bool ringBell() {
        std::string message = "RingBell";
        return sendMessage(message.c_str());
    }

    static bool showAllPlayersArrow() {
        std::string message = "ShowAllPlayersArrow";
        return sendMessage(message.c_str());
    }

    static bool pickUpAllBodies() {
        std::string message = "PickUpAllBodies";
        return sendMessage(message.c_str());
    }

    static bool throwAwayBomb() {
        std::string message = "ThrowAwayBomb";
        return sendMessage(message.c_str());
    }

    static bool startGame() {
        std::string message = "StartGame";
        return sendMessage(message.c_str());
    }    

    /// <summary>
    /// 远程吃尸体
    /// </summary>
    /// <returns></returns>
    static bool remoteEat() {
        std::string message = "RemoteEat";
        return sendMessage(message.c_str());
    }

private:
    static const int port = 29241;

    static bool sendMessage(const char* message) {
        return send_and_receive(port, message);
    }
    
private:
    static bool send_and_receive(int server_port, const char* message);
};


