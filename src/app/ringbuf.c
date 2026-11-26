/*
 * ringbuf.c —— 环形缓冲区实现
 *
 * 核心只有两个动作：
 *   写：buf[head] = byte;  head 往后走一格（到末尾就回 0）; used++
 *   读：byte = buf[tail];  tail 往后走一格（到末尾就回 0）; used--
 *
 * "往后走一格"的写法：
 *   head = (head + 1) % size;   ← 取模，最直观
 *   head = (head + 1 == size) ? 0 : head + 1;  ← 嵌入式常用，因为 % 很慢
 * 本文件用第二种。速度差别在中断里才重要，但习惯要一开始就养好。
 */
#include "ringbuf.h"

void ringbuf_init(ringbuf_t *rb, uint8_t *storage, uint16_t size)
{
    rb->buf  = storage;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
    rb->used = 0;
}

bool ringbuf_put(ringbuf_t *rb, uint8_t byte)
{
    /* 满了就不写。注意：这里不覆盖旧数据 —— 覆盖会让消费者读到"半新半旧"的帧 */
    if (rb->used >= rb->size) {
        return false;
    }

    rb->buf[rb->head] = byte;
    rb->head = (uint16_t)((rb->head + 1u == rb->size) ? 0u : rb->head + 1u);
    rb->used++;
    return true;
}

bool ringbuf_get(ringbuf_t *rb, uint8_t *out)
{
    if (rb->used == 0) {
        return false;
    }

    *out = rb->buf[rb->tail];
    rb->tail = (uint16_t)((rb->tail + 1u == rb->size) ? 0u : rb->tail + 1u);
    rb->used--;
    return true;
}

uint16_t ringbuf_used(const ringbuf_t *rb)
{
    return rb->used;
}

uint16_t ringbuf_free(const ringbuf_t *rb)
{
    return (uint16_t)(rb->size - rb->used);
}

void ringbuf_clear(ringbuf_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->used = 0;
}
