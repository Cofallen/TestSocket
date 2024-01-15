#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// 数据包
char RxMsg[1024] = {0};
char TxMsg[5] = {0, 1, 2, 3, 4};

union DATA
{
    char Head[100];
    int16_t Tail[25];
};

int main()
{
    int SERVER_FD = 0;
    struct sockaddr_in SERVER_ADDR = {}, CLIENT_ADDR = {};
    socklen_t CLIENT_ADDR_LEN;

    // 初始化
    memset(&SERVER_ADDR, 0, sizeof(SERVER_ADDR));
    SERVER_ADDR.sin_family = AF_INET;
    SERVER_ADDR.sin_port = htons(12345);
    SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY);

    // FD创建
    if (-1 == (SERVER_FD = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        printf("FD失败\n");
        exit(1);
    }

    // bind
    if (bind(SERVER_FD, (struct sockaddr *)&SERVER_ADDR, sizeof(SERVER_ADDR)) < 0)
    {
        printf("bind error!\n");
        exit(1);
    }

    /* 接受客户端的返回数据 */
    int RxMsg_len;
    if ((RxMsg_len = recvfrom(SERVER_FD, RxMsg, sizeof(RxMsg), 0, (struct sockaddr *)&CLIENT_ADDR, &CLIENT_ADDR_LEN)) < 0)
    {
        perror("recvfrom failed\n");
        exit(1);
    }

    printf("客户端发送过来的数据为：%s\n", RxMsg);

    while (1)
    {
        /* 清空缓冲区 */
        // printf("请发送\n");
        // fgets((char *)TxMsg, sizeof(TxMsg), stdin);

        /* 发送数据到客户端 */
        if (sendto(SERVER_FD, TxMsg, 5, 0, (struct sockaddr *)&CLIENT_ADDR, CLIENT_ADDR_LEN) < 0)
        {
            perror("sendto failed");
            exit(1);
        }

        // memset(TxMsg, 0, sizeof(TxMsg));
    }
    return 0;
}
