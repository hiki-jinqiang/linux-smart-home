#include "gy39.h"

unsigned char GY39_cmd1[] = {0xA5, 0x51, 0xF6}; //测光照强度命令
unsigned char GY39_cmd2[] = {0xA5, 0x52, 0xF7}; //测温度、湿度、大气、海拔命令

int GZ, WD, SD, QY, HB;

void get_GZ(int fd)
{
    int i = 0, flag = 0;
    unsigned char recvbuf[9] = {0};
    write(fd, GY39_cmd1, 3);
    while (1)
    {
        unsigned char buf;
        read(fd, &buf, 1);
        if (buf == 0x5A)
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
    GZ = recvbuf[4] << 24 | recvbuf[5] << 16 | recvbuf[6] << 8 | recvbuf[7];
    GZ = GZ / 100;
}

void get_other(int fd)
{
    int i = 0, flag = 0;
    unsigned char recvbuf[15] = {0};
    write(fd, GY39_cmd2, 3);
    while (1)
    {
        unsigned char buf;
        read(fd, &buf, 1);
        if (buf == 0x45)        //标志头部分
        {
            flag = 1;
        }
        if (flag == 1)
        {
            recvbuf[i++] = buf;
            if (i == 15)
            {
                break;
            }
        }
    }
    WD = recvbuf[4] << 8 | recvbuf[5];
    WD = WD / 100;
    QY = recvbuf[6] << 24 | recvbuf[7] << 16 | recvbuf[8] << 8 | recvbuf[9];
    QY = QY / 100;
    SD = recvbuf[10] << 8 | recvbuf[11];
    SD = SD / 100;
    HB = recvbuf[12] << 8 | recvbuf[13];
}
