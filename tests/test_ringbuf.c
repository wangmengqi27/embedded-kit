/*
 * test_ringbuf.c —— ringbuf 的 PC 端测试
 *
 * 测试的思路：不打印一堆调试信息，而是"断言" —— 我期望它是什么，
 * 如果不是就报错。跑完所有用例，最后告诉你几过几败。
 *
 * 编译并运行（在 D:\embedded-kit 下）：
 *   gcc -std=c11 -Wall -Wextra -O2 -g tests/test_ringbuf.c src/app/ringbuf.c -o tests/test_ringbuf.exe
 *   .\tests\test_ringbuf.exe
 */
#include <stdio.h>
#include <stdint.h>
#include "../src/app/ringbuf.h"

static int g_fail = 0;
static int g_total = 0;

/* 检查条件是否为真 */
static void check(int cond, const char *what)
{
    g_total++;
    if (cond) {
        printf("  [PASS] %s\n", what);
    } else {
        printf("  [FAIL] %s\n", what);
        g_fail++;
    }
}

int main(void)
{
    uint8_t storage[4];
    ringbuf_t rb;

    /* ---------- 用例 1：刚初始化应该是空的 ---------- */
    printf("case 1: init\n");
    ringbuf_init(&rb, storage, 4);
    check(ringbuf_used(&rb) == 0, "used == 0 after init");
    check(ringbuf_free(&rb) == 4, "free == 4 after init");

    /* ---------- 用例 2：存进去再读出来，顺序要对 ---------- */
    printf("case 2: put / get order\n");
    check(ringbuf_put(&rb, 0x11), "put 0x11 ok");
    check(ringbuf_put(&rb, 0x22), "put 0x22 ok");
    check(ringbuf_put(&rb, 0x33), "put 0x33 ok");
    check(ringbuf_used(&rb) == 3, "used == 3");

    uint8_t v = 0;
    check(ringbuf_get(&rb, &v) && v == 0x11, "get -> 0x11");
    check(ringbuf_get(&rb, &v) && v == 0x22, "get -> 0x22");
    check(ringbuf_get(&rb, &v) && v == 0x33, "get -> 0x33");
    check(ringbuf_get(&rb, &v) == false, "get on empty returns false");

    /* ---------- 用例 3：写满就不能再写（不覆盖旧数据） ---------- */
    printf("case 3: full\n");
    check(ringbuf_put(&rb, 1), "put 1");
    check(ringbuf_put(&rb, 2), "put 2");
    check(ringbuf_put(&rb, 3), "put 3");
    check(ringbuf_put(&rb, 4), "put 4 (last slot)");
    check(ringbuf_put(&rb, 5) == false, "put 5 fails when full");
    check(ringbuf_used(&rb) == 4, "used == 4 when full");

    check(ringbuf_get(&rb, &v) && v == 1, "first byte still 1 (not overwritten)");
    ringbuf_clear(&rb);
    check(ringbuf_used(&rb) == 0, "clear -> used == 0");

    /* ---------- 用例 4：回绕 —— 反复读写字，指针要能绕回开头 ---------- */
    printf("case 4: wrap around (20 rounds)\n");
    int wrap_ok = 1;
    for (int round = 0; round < 20; round++) {
        /* 每轮存 3 个、读 3 个，故意让 head/tail 跨过数组末尾 */
        for (uint8_t i = 0; i < 3; i++) {
            if (!ringbuf_put(&rb, (uint8_t)(round * 3 + i))) {
                wrap_ok = 0;
            }
        }
        for (uint8_t i = 0; i < 3; i++) {
            uint8_t expect = (uint8_t)(round * 3 + i);
            if (!ringbuf_get(&rb, &v) || v != expect) {
                wrap_ok = 0;
            }
        }
    }
    check(wrap_ok, "20 rounds of put/get keep data in order");
    check(ringbuf_used(&rb) == 0, "empty after wrap test");

    /* ---------- 汇总 ---------- */
    printf("\n%d checks, %d failed\n", g_total, g_fail);
    if (g_fail == 0) {
        printf("ALL TESTS PASSED\n");
    }
    return (g_fail == 0) ? 0 : 1;
}
