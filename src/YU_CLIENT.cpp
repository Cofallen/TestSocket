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
    YU_V_SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    YU_V_SERVER_FD = socket(AF_INET, SOCK_STREAM, 0);

    // 与服务端
    connect(YU_V_SERVER_FD, (struct sockaddr *)&YU_V_SERVER_ADDR, sizeof(YU_V_SERVER_ADDR));

    while (1)
    {
        char YU_V_BUFF[1024];

        fgets(YU_V_BUFF, sizeof(YU_V_BUFF), stdin);
        send(YU_V_SERVER_FD, YU_V_BUFF, sizeof(YU_V_BUFF), 0);
    }
}