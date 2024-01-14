#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <cstdio>

#define BUF_LEN 100

int main(void)
{
    int ClientFd;
    char Buf[BUF_LEN] = {0};
    struct sockaddr_in ServerSockAddr;

    /* 向服务器发起请求 */
    memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
    ServerSockAddr.sin_family = AF_INET;
    ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerSockAddr.sin_port = htons(6666);

    while (1)
    {
        /* 创建客户端socket */
        if (-1 == (ClientFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
        {
            printf("socket error!\n");
            exit(1);
        }

        /* 连接 */
        if (-1 == connect(ClientFd, (struct sockaddr *)&ServerSockAddr, sizeof(ServerSockAddr)))
        {
            printf("connect error!\n");
            exit(1);
        }

        printf("请输入一个字符串，发送给服务端：");
        // gets(Buf);
        fgets(Buf, BUF_LEN, stdin);

        /* 发送数据到服务端 */
        send(ClientFd, Buf, strlen(Buf), 0);
        memset(Buf, 0, BUF_LEN); // 重置缓冲区

        /* 接受服务端的返回数据 */
        recv(ClientFd, Buf, BUF_LEN, 0);
        printf("服务端发送过来的数据为：%s\n", Buf);

        memset(Buf, 0, BUF_LEN); // 重置缓冲区
        close(ClientFd);         // 关闭套接字
    }
    return 0;
}