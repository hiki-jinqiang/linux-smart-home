#include "touch.h"

x_y Touch()
{
    //触摸屏的操作
    int ev_fd = open("/dev/input/event0", O_RDWR);
    if (-1 == ev_fd)
    {
        perror("open event0 error");
        exit(0); //将程序结束
    }

    struct input_event ev; //实例化这个结构体
    int flag = 0;
    int x, y;   //你的手可能会滑动 滑动坐标
    int x0, y0; //你的手点在屏幕上面的时候的坐标
    int i = 1;
    x_y getxy;

    while (1) //死循环 无限的操作这个触摸屏
    {
        int r = read(ev_fd, &ev, sizeof(ev)); //这里默认阻塞io就可以了
        if (r != sizeof(ev))
        {
            perror("read ev error");
            continue;
        }

        // printf("type:%d  code:%d  value:%d\n",ev.type,ev.code,ev.value);
        //获取坐标
        if (0x03 == ev.type) //拿坐标
        {
            if (0x00 == ev.code) // x轴
            {
                x = ev.value;
            }
            else if (0x01 == ev.code) // y轴
            {
                y = ev.value;
            }
        }

        //判断收点击的时候
        if (0x01 == ev.type && 0x14a == ev.code && 0x01 == ev.value) //手点下去的时候
        {
            x0 = x;
            y0 = y;
        }
        else if (0x01 == ev.type && 0x14a == ev.code && 0x00 == ev.value) //收抬起来了的时候进行操作
        {
            getxy.x = x;
            getxy.y = y;
            return getxy;
        }
    }
    close(ev_fd);
}

int Touch_slide()
{
    //触摸屏的操作
    int ev_fd = open("/dev/input/event0", O_RDWR);
    if (-1 == ev_fd)
    {
        perror("open event0 error");
        exit(0); //将程序结束
    }

    struct input_event ev; //实例化这个结构体
    int flag = 0;
    int x, y;   //你的手可能会滑动 滑动坐标
    int x0, y0; //你的手点在屏幕上面的时候的坐标
    int i = 1;
    x_y getxy;

    while (1) //死循环 无限的操作这个触摸屏
    {
        int r = read(ev_fd, &ev, sizeof(ev)); //这里默认阻塞io就可以了
        if (r != sizeof(ev))
        {
            perror("read ev error");
            continue;
        }

        // printf("type:%d  code:%d  value:%d\n",ev.type,ev.code,ev.value);
        //获取坐标
        if (0x03 == ev.type) //拿坐标
        {
            if (0x00 == ev.code) // x轴
            {
                x = ev.value;
            }
            else if (0x01 == ev.code) // y轴
            {
                y = ev.value;
            }
        }

        //判断收点击的时候
        if (0x01 == ev.type && 0x14a == ev.code && 0x01 == ev.value) //手点下去的时候
        {
            x0 = x;
            y0 = y;
        }
        else if (0x01 == ev.type && 0x14a == ev.code && 0x00 == ev.value) //收抬起来了的时候进行操作
        {
            if (x < x0 && abs(x - x0) >= abs(y - y0))           // obtain absolute value
            {
                return 0;
            }
            else if (x > x0 && abs(x - x0) >= abs(y - y0))
            {
                return 1;
            }
        }
    }
    close(ev_fd);
}
