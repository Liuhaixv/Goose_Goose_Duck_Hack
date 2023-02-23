#include "MelonLoaderHelper.h"
#include "../httplib.h"

//public:

//private
bool MelonLoaderHelper::send_and_receive(int server_port, const char* message) {
    const char* server_ip = "127.0.0.1";

    // 创建socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        //std::cerr << "Error: socket creation failed" << std::endl;
        return false;
    }

    // 设置超时时间
    struct timeval timeout;
    timeout.tv_sec = 1; // 1秒超时
    timeout.tv_usec = 0;

    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
        //std::cerr << "Error: setting send timeout failed" << std::endl;
        closesocket(sock);
        return false;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
        //std::cerr << "Error: setting receive timeout failed" << std::endl;
        closesocket(sock);
        return false;
    }

    // 构造server地址
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    // 连接到server
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        //std::cerr << "Error: connection failed" << std::endl;
        closesocket(sock);
        return false;
    }

    // 发送消息
    if (send(sock, message, strlen(message), 0) < 0) {
        //std::cerr << "Error: sending message failed" << std::endl;
        closesocket(sock);
        return false;
    }

    const int buffer_len = 1024;
    char buffer[buffer_len];

    // 接收响应
    int bytes_received = recv(sock, buffer, buffer_len - 1, 0);
    if (bytes_received <= 0) {
        //std::cerr << "Error: receiving message failed" << std::endl;
        closesocket(sock);
        return false;
    }

    buffer[bytes_received] = '\0';

    //判断返回响应是否为success
    bool result =  strcmp(buffer, "success") == 0;
    // 关闭socket
    closesocket(sock);

    return result;
}