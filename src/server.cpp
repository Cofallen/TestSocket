#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_LEN 100

int main(void)
{
    int ServerFd; // 定义服务端文件描述符

    struct sockaddr_in ServerSockAddr; // 定义服务端socket地址
    struct sockaddr ClientAddr;        // 定义客户端socket地址
    char Buf[BUF_LEN] = {0};           // 定义接受缓冲区的buf
    int addr_len = 0, recv_len = 0;
    int optval = 1;

    /* 创建服务端文件描述符 */
    if (-1 == (ServerFd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        printf("socket error!\n");
        exit(1);
    }

    /* 设置服务端信息 */

    memset(&ServerSockAddr, 0, sizeof(ServerSockAddr)); // 给结构体ServerSockAddr清零
    ServerSockAddr.sin_family = AF_INET;                // 使用IPv4地址
    ServerSockAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 自动获取IP地址
    ServerSockAddr.sin_port = htons(6666);              // 端口

    // 设置地址和端口号可以重复使用
    if (setsockopt(ServerFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        printf("setsockopt error!\n");
        exit(1);
    }

    /* 绑定操作，绑定前加上上面的socket属性可重复使用地址 */
    if (-1 == bind(ServerFd, (struct sockaddr *)&ServerSockAddr, sizeof(struct sockaddr)))
    {
        printf("bind error!\n");
        exit(1);
    }

    addr_len = sizeof(struct sockaddr);

    while (1)
    {
        /* 接受客户端的返回数据 */
        if ((recv_len = recvfrom(ServerFd, Buf, BUF_LEN, 0, (struct sockaddr *)&ClientAddr, (socklen_t *)&addr_len)) < 0)
        {
            printf("recvfrom error!\n");
            exit(1);
        }

        printf("客户端发送过来的数据为：%s\n", Buf);

        /* 发送数据到客户端 */
        sendto(ServerFd, Buf, recv_len, 0, (struct sockaddr *)&ClientAddr, addr_len);

        /* 清空缓冲区 */
        memset(Buf, 0, BUF_LEN);
    }
    return 0;
}
