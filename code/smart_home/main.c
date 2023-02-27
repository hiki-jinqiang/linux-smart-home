#include <pthread.h>
#include "beep.h"
#include "gy39.h"
#include "lcd.h"
#include "led.h"
#include "mq.h"
#include "touch.h"
#include "uartinit.h"

extern int Lcd_fd;         //屏幕文件的描述符  全局变量
extern unsigned int *plcd; //全局映射首地址

extern unsigned char *title[];      //智能家居四个字的二维数组
extern unsigned char *data_array[]; // 0-9数字数组

extern int value;              //烟雾值
extern int GZ, WD, SD, QY, HB; // gy39传感器数据

int flag = 0; //标记位

// gy39传感器调用
void *gy39()
{
    pthread_detach(pthread_self()); //只能子线程用

    int uart_fd = uart_init("/dev/ttySAC1"); //串口初始化

    while (1)
    {
        int i = 0;
        i = led_set(1, 8);
        if (i == -1)
        {
            break;
        }
        i = led_set(1, 9);
        if (i == -1)
        {
            break;
        }
        get_other(uart_fd);
        printf("WD:%d SD:%d QY:%d HB:%d ", WD, SD, QY, HB);
        get_GZ(uart_fd);
        printf("GZ:%d ", GZ);
        if (flag == 1)
        {
            DisplayData(WD, 1);
            DisplayData(SD, 2);
            DisplayData(QY, 3);
            DisplayData(HB, 4);
            DisplayData(GZ, 5);
        }
        i = led_set(0, 10);
        if (i == -1)
        {
            break;
        }
        i = led_set(0, 9);
        if (i == -1)
        {
            break;
        }
        i = led_set(0, 8);
        if (i == -1)
        {
            break;
        }
        sleep(3);
    }
    close(uart_fd);
}

//烟雾传感器调用
void *mq2()
{
    pthread_detach(pthread_self()); //只能子线程用

    int uart_fd = uart_init("/dev/ttySAC2"); //串口初始化

    while (1)
    {
        int i = 0;
        i = beep_set(1);
        if (i == -1)
        {
            break;
        }
        get_value(uart_fd);
        printf("YW:%d\n", value);
        if (flag == 1)
        {
            DisplayData(value, 6);
        }
        i = beep_set(0);
        if (i == -1)
        {
            break;
        }
        sleep(3);
    }
    close(uart_fd);
}

void *music()
{
    pthread_detach(pthread_self()); //资源自动回收 这个只能在子线程用

    system("killall -9 madplay");
    char buf[128] = {0};
    sprintf(buf, "madplay ./1.mp3 ");
    printf("buf=%s\n", buf);
    system(buf);
}

int main()
{
    //初始化
    Lcdinit();

    //显示项目开机图片
    OpenBmpAndDisplay("./startup.bmp");

    //标题:智能家居
    int i;
    for (i = 0; i < 4; i++)
    {
        DisplayZimo(title[i], 32, 32, 300 + 100 * i, 220, 0xFF5733);
    }

    //延时三秒
    while (i < 7)
    {
        sleep(1);
        i++;
    }

    //进入菜单界面
    Menu();

    //线程获取gy39数据
    pthread_t thread_1;
    if (pthread_create(&thread_1, NULL, gy39, NULL) != 0)
    {
        perror("ptread_create_1 error");
        return -1;
    }

    //线程获取烟雾传感器数据
    pthread_t thread_2;
    if (pthread_create(&thread_2, NULL, mq2, NULL) != 0)
    {
        perror("ptread_create_2 error");
        return -1;
    }

    while (1)
    {
        //获取触摸屏
        x_y Getx_y = Touch();

        //第一个按钮
        if ((Getx_y.x * 800 / 1060) >= 100 && (Getx_y.x * 800 / 1060) < 300 && (Getx_y.y * 480 / 600) >= 60 && (Getx_y.y * 480 / 600) < 160)
        {
            printf("串口通信数据显示\n");
            flag = 1;                             //在线程打印数据
            DisplaySql(800, 480, 0, 0, 0x000000); //背景
            while (1)
            {
                DisplaySql(50, 50, 750, 0, 0xFF0000);
                x_y Getx_y_off = Touch();
                if ((Getx_y_off.x * 800 / 1060) >= 750 && (Getx_y_off.x * 800 / 1060) < 800 && (Getx_y_off.y * 480 / 600) >= 0 && (Getx_y_off.y * 480 / 600) < 50)
                {
                    printf("退出\n");
                    flag = 0; //关闭显示
                    Menu();
                    break;
                }
            }
        }
        else if ((Getx_y.x * 800 / 1060) >= 520 && (Getx_y.x * 800 / 1060) < 720 && (Getx_y.y * 480 / 600) >= 60 && (Getx_y.y * 480 / 600) < 160)
        {
            printf("music\n");
            i = led_set(1, 10);
            if (i == -1)
            {
                break;
            }

            pthread_t thread_3;
            if (pthread_create(&thread_3, NULL, music, NULL) != 0)
            {
                perror("ptread_create_2 error");
                return -1;
            }

            while (1)
            {
                DisplaySql(50, 50, 750, 0, 0xFF0000);
                x_y Getx_y_off = Touch();
                if ((Getx_y_off.x * 800 / 1060) >= 750 && (Getx_y_off.x * 800 / 1060) < 800 && (Getx_y_off.y * 480 / 600) >= 0 && (Getx_y_off.y * 480 / 600) < 50)
                {
                    printf("退出\n");
                    i = led_set(0, 10);
                    if (i == -1)
                    {
                        break;
                    }
                    system("killall -9 madplay");
                    Menu();
                    break;
                }
            }
        }
        else if ((Getx_y.x * 800 / 1060) >= 100 && (Getx_y.x * 800 / 1060) < 300 && (Getx_y.y * 480 / 600) >= 300 && (Getx_y.y * 480 / 600) < 400)
        {
            printf("album\n");
            OpenBmpAndDisplay("./3.bmp");
            while (1)
            {
                DisplaySql(50, 50, 750, 0, 0xFF0000);
                x_y Getx_y_off = Touch();

                int Get_slide = Touch_slide();
                if (Get_slide = 0)
                {
                    OpenBmpAndDisplay("./1.bmp");
                }
                else if (Get_slide = 1)
                {
                    OpenBmpAndDisplay("./7.bmp");
                }
                else if ((Getx_y_off.x * 800 / 1060) >= 750 && (Getx_y_off.x * 800 / 1060) < 800 && (Getx_y_off.y * 480 / 600) >= 0 && (Getx_y_off.y * 480 / 600) < 50)
                {
                    printf("退出\n");
                    break;
                }
            }
        }
        else if ((Getx_y.x * 800 / 1060) >= 520 && (Getx_y.x * 800 / 1060) < 720 && (Getx_y.y * 480 / 600) >= 300 && (Getx_y.y * 480 / 600) < 400)
        {
            system("reboot");
            // printf("reboot\n");
        }
    }
    LcdDestory();

    return 0;
}
