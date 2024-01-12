#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "stdio.h"

int main()
{
    // 服务端文件描述符
    int YU_V_SERVER_FD = 0, YU_V_CLIENT_FD = 0;

    // 声明结构体

    struct sockaddr_in YU_V_SERVER_ADDR, YU_V_CLIENT_ADDR;

    // 结构体赋值

    YU_V_SERVER_ADDR.sin_family = AF_INET;
    YU_V_SERVER_ADDR.sin_port = htons(8080);
    YU_V_SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY);

    // 创建socket
    YU_V_SERVER_FD = socket(AF_INET, SOCK_STREAM, 0);
    if (YU_V_SERVER_FD < 0)
    {
        printf("SOCCKET创建失败\n");
        return -1;
    }

    // 绑定socket
    bind(YU_V_SERVER_FD, (struct sockaddr *)&YU_V_SERVER_ADDR, (socklen_t)sizeof(YU_V_SERVER_ADDR));
    // 监听地址和端口
    if (listen(YU_V_SERVER_FD, 10) < 0) // 几个链接
    {
        printf("SOCKET监听失败\n");
        return -1;
    }
    int YU_V_CLIENT_ADDR_LEN = sizeof(YU_V_CLIENT_ADDR);

    // 接收客户端连接

    YU_V_CLIENT_FD = accept(YU_V_SERVER_FD, (struct sockaddr *)&YU_V_SERVER_ADDR, (socklen_t *)&YU_V_CLIENT_ADDR_LEN);

    while (1)
    {
        char YU_V_BUFF[1024] = {0};
        // 接收到客户端数据
        recv(YU_V_CLIENT_FD, YU_V_BUFF, sizeof(YU_V_BUFF), 0);
        printf("接收到客户端数据: %s\n", YU_V_BUFF);

        // // 关闭socket
        // close(YU_V_SERVER_FD);
    }
}