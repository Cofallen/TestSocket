#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <unordered_map>

#include <string>

#include "stdio.h"

struct PID
{
    float Kp;
    float Ki;
    float Kd;

    float target;
    float err;
    float lastErr;
    float P_out;
    float I_out;
    float D_out;
    float D_lit;
    float I_lit;
    float allIit;
    float allOut;
};
struct YU_TYPEDEF_DEBUG
{
    struct PID PID_A;
    struct PID PID_S;
};
struct YU_TYPEDEF_DEBUG YU_V_DEBUG[10];

void YU_F_DEBUG_THREAD()
{
    // UDP地址结构
    struct sockaddr_in YU_V_SERVER_ADDR
    {
    };
    struct sockaddr_in YU_V_CLIENT_ADDR
    {
    };
    socklen_t YU_V_CLIENT_ADDR_LEN = sizeof(YU_V_CLIENT_ADDR);
    // UDP发送共用体
    union YU_TYPEDEF_SEND_UNION
    {
        struct
        {
            struct YU_TYPEDEF_DEBUG YU_V_DEBUG;
            unsigned char TAIL[4];
        } PACKET;
        char RAW[160];
    };
    YU_TYPEDEF_SEND_UNION YU_U_SEND{};

    // 包尾
    YU_U_SEND.PACKET.TAIL[2] = 0x80;
    YU_U_SEND.PACKET.TAIL[3] = 0x7f;
    printf("调试线程启动\n");

    // 创建UDP socket
    int YU_V_SOCKET_FD = socket(AF_INET, SOCK_DGRAM, 0);
    if (YU_V_SOCKET_FD < 0)
    {
        printf("调试线程创建失败\n");
        close(YU_V_SOCKET_FD);
    }

    int8_t YU_V_MOTOR_TYPE = 0;
    printf("调试线程启动\n");
    printf("尝试获取UDP服务端IP地址\n");

    while (true)
    {
        memcpy(&YU_U_SEND.PACKET.YU_V_DEBUG, &YU_V_DEBUG[YU_V_MOTOR_TYPE], sizeof(YU_TYPEDEF_DEBUG));

        // 发送数据包
        sendto(YU_V_SOCKET_FD, YU_U_SEND.RAW, sizeof(YU_U_SEND.RAW), 0, (struct sockaddr *)&YU_V_CLIENT_ADDR, sizeof(YU_V_CLIENT_ADDR));
        usleep(1);
    }
}