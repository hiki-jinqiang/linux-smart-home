#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <linux/input.h>

typedef struct
{
    int x;
    int y;
} x_y;

x_y Touch();

int Touch_slide();
