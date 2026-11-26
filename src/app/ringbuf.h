#ifndef RINGBUF_H
#define RINGBUF_H     /* 头文件保护：同一个 .h 被包含两次时，只展开第一次 */

#include <stdint.h>
#include <stdbool.h>

/*
 * 环形缓冲区（byte 级，先按字节存取）
 *
 * 为什么需要它：
 *   CAN 接收中断里拿到字节，不能当场处理（中断要快），先塞进缓冲区；
 *   主循环有空了再取出来解析。这就是"生产者/消费者"，缓冲区是中间的队列。
 *
 * 为什么是"环形"：
 *   内存只有一段固定的数组，不能一直往后写。写到末尾就绕回开头，
 *   像一个首尾相接的圆环。这样固定 256 字节就能一直循环用，不用 malloc。
 *
 * 成员含义：
 *   buf  : 外部传进来的数组（缓冲区自己不分配内存 —— 嵌入式禁止 malloc）
 *   size : 这块数组有多大
 *   head : 下一次"写"的位置（生产者动它）
 *   tail : 下一次"读"的位置（消费者动它）
 *   used : 当前存了多少字节（用计数比"留一格"更好懂，代价是 2 字节）
 */
typedef struct {
    uint8_t  *buf;
    uint16_t  size;
    uint16_t  head;
    uint16_t  tail;
    uint16_t  used;
} ringbuf_t;

/* 初始化：把一块现成的数组交给缓冲区管理。size 必须 > 0。 */
void ringbuf_init(ringbuf_t *rb, uint8_t *storage, uint16_t size);

/* 写入一个字节。成功返回 true；缓冲区满返回 false（不覆盖旧数据）。 */
bool ringbuf_put(ringbuf_t *rb, uint8_t byte);

/* 读出一个字节，放到 *out。成功返回 true；空返回 false。 */
bool ringbuf_get(ringbuf_t *rb, uint8_t *out);

/* 当前已存字节数 */
uint16_t ringbuf_used(const ringbuf_t *rb);

/* 还能再存多少字节 */
uint16_t ringbuf_free(const ringbuf_t *rb);

/* 清空（丢弃所有未读数据） */
void ringbuf_clear(ringbuf_t *rb);

#endif /* RINGBUF_H */
