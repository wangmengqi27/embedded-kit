/*
 * day01_hello.c —— 第一个能编译、能运行的程序
 *
 * 它做的事：构造一帧"传感器数据"，打印成人能读懂的样子。
 * 这是后面真正 can_frame 模块的缩小版。
 *
 * 编译：gcc -std=c11 -Wall -Wextra -O2 -g learn/day01_hello.c -o learn/day01_hello.exe
 * 运行：.\learn\day01_hello.exe
 */
#include <stdio.h>
#include <stdint.h>

/* 结构体：把三个"字段"打包成一种新类型。
 * uint8_t  = 1 字节无符号整数（0~255）
 * int16_t  = 2 字节有符号整数（-32768~32767）
 * uint16_t = 2 字节无符号整数（0~65535）
 * 用固定宽度类型，是因为 CAN 帧里每个字节的位置都必须确定。 */
typedef struct {
    uint8_t  seq;        /* 帧序号，每发一帧加一，0~255 循环 */
    int16_t  temp_x100;  /* 温度 ×100，3520 表示 35.20 ℃ */
    uint16_t volt_mv;    /* 电压，单位毫伏，12100 表示 12.100 V */
} SensorFrame_t;

/* 打印一帧。
 * const SensorFrame_t *f 的意思是：f 是一个指针，指向一帧数据；
 * const 表示"我只读，不修改它"，这是 C 里表达意图的好习惯。
 * 访问指针指向的结构体成员用 ->，访问普通变量的成员用 . */
static void frame_print(const SensorFrame_t *f)
{
    printf("frame #%u\n", (unsigned)f->seq);
    printf("  temp = %d.%02d C\n", f->temp_x100 / 100, f->temp_x100 % 100);
    printf("  volt = %d.%03d V\n", f->volt_mv / 1000, f->volt_mv % 1000);
}

int main(void)
{
    /* 指定初始化器（C99）：按名字赋值，比按顺序赋值更不容易出错 */
    SensorFrame_t f = {
        .seq       = 37,
        .temp_x100 = 3520,
        .volt_mv   = 12100,
    };

    frame_print(&f);   /* &f = f 的地址；函数需要指针，所以取地址传进去 */

    /* sizeof 返回类型/变量占多少字节。这里应该是 6 字节（1+2+2 再加对齐） */
    printf("sizeof(SensorFrame_t) = %u byte(s)\n", (unsigned)sizeof f);

    return 0;          /* 返回 0 表示程序正常结束 */
}
