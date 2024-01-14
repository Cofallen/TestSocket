#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEST_PORT 3000
#define DSET_IP_ADDRESS "47.245.91.147"

int main()
{
    int sock_fd; /* 套接字文件描述符 */
    int send_num;
    int recv_num;
    socklen_t client_len; // 更正为 socklen_t 类型
    char send_buf[20] = {"hello tiger"};
    char recv_buf[20];
    struct sockaddr_in addr_serv; /* 服务端地址，客户端地址 */
    struct sockaddr_in addr_client;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0); // 创建套接字
    if (sock_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    // 初始化服务器端地址
    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_serv.sin_port = htons(DEST_PORT);

    if (bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)
    {
        perror("bind");
        exit(1);
    }

    client_len = sizeof(addr_client);
    printf("Waiting for data:\n");

    // 修改为 socklen_t 类型，并传递其地址作为 recvfrom 的参数
    recv_num = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_client, (socklen_t *)&client_len);

    if (recv_num < 0)
    {
        perror("recvfrom");
        exit(1);
    }
    else
    {
        printf("Received data: %s\n", recv_buf);
    }

    send_num = sendto(sock_fd, send_buf, sizeof(send_buf), 0, (struct sockaddr *)&addr_client, client_len);
    if (send_num < 0)
    {
        perror("sendto");
        exit(1);
    }
    else
    {
        printf("Sent data: %s\n", send_buf);
    }

    close(sock_fd);
    return 0;
}
