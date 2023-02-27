#include "led.h"

int led_set(char on_off, char led)
{
    int fd = open("/dev/led_drv", O_RDWR);
    if (-1 == fd)
    {
        perror("open led error");
        return -1;
    }
    char led_buf[2] = {on_off, led}; //第一个是控制亮灭，第二个控制哪个LED
    int r = write(fd, led_buf, 2);
    if (-1 == r)
    {
        perror("write led on error");
    }

    close(fd);
    return 0;
}
