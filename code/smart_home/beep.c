#include "beep.h"

int beep_set(char on_off)
{
    int fd = open("/dev/pwm", O_RDWR);
    if (-1 == fd)
    {
        perror("open beep error");
        return -1;
    }
    int r = write(fd, &on_off, 1);
    if (-1 == r)
    {
        perror("write beep on error");
    }

    close(fd);
    return 0;
}
