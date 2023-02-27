#include "mq.h"

unsigned char cmd[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

int value;

void get_value(int fd)
{
    int i = 0, flag = 0;
    unsigned char recvbuf[9] = {0};
    write(fd, cmd, 9);
    while (1)
    {
        unsigned char buf;
        read(fd, &buf, 1);
        if (buf == 0xFF)
        {
            flag = 1;
        }
        if (flag == 1)
        {
            recvbuf[i++] = buf;
            if (i == 9)
            {
                break;
            }
        }
    }
    value = recvbuf[2] << 8 | recvbuf[3];
}
