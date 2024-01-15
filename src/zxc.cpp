#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <termios.h>

int main()
{
    // Open the serial port
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0)
    {
        perror("Error opening serial port");
        return 1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(serial_port, &tty) != 0)
    {
        perror("Error from tcgetattr");
        return 1;
    }

    tty.c_cflag &= ~PARENB;  // No parity
    tty.c_cflag &= ~CSTOPB;  // 1 stop bit
    tty.c_cflag |= CS8;      // 8 bits per byte
    tty.c_cflag &= ~CRTSCTS; // No hardware flow control

    cfsetospeed(&tty, B9600); // Set the baud rate to 9600
    cfsetispeed(&tty, B9600);

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        perror("Error from tcsetattr");
        return 1;
    }

    // Create a UDP socket
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(12345);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error binding");
        return 1;
    }

    char buffer[1024];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1)
    {
        printf("Waiting for data...\n");

        ssize_t recv_len = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len < 0)
        {
            perror("Error receiving data");
            return 1;
        }

        printf("Received data: %s\n", buffer);

        // Send the received data to the serial port
        write(serial_port, buffer, strlen(buffer));

        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
    }

    close(serial_port); // Close the serial port
    close(server_fd);   // Close the socket
    return 0;
}
