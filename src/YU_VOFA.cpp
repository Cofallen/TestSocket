#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    // 创建UDP socket
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "Error: Failed to create socket" << std::endl;
        return 1;
    }

    // 服务器地址结构
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1500);              // 服务端监听的端口号
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 设置为本地主机地址

    // 绑定socket到服务器地址
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error: Bind failed" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "UDP 服务端已启动，等待接收数据..." << std::endl;

    // 接收数据
    char buffer[1024];
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    ssize_t recvLen = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (recvLen < 0)
    {
        std::cerr << "Error: Failed to receive data" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "接收到来自 " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " 的数据：" << buffer << std::endl;

    // 向上位机发送数据
    const char *responseMsg = "Hello, this is the server!";
    ssize_t sendLen = sendto(serverSocket, responseMsg, strlen(responseMsg), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    if (sendLen < 0)
    {
        std::cerr << "Error: Failed to send response" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "已向 " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << " 发送数据：" << responseMsg << std::endl;

    // 关闭socket
    close(serverSocket);

    return 0;
}
