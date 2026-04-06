#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 18 线程安全环形缓冲区 Ring Buffer
 * 要求：使用互斥锁与条件变量实现多线程生产者/消费者的有界环形队列
 */

typedef struct {
    int *buf;                 /* 数据缓冲区（整数队列） */
    size_t capacity;          /* 容量（元素个数） */
    size_t head;              /* 读指针 */
    size_t tail;              /* 写指针 */
    size_t count;             /* 当前元素个数 */
    pthread_mutex_t mtx;      /* 互斥锁 */
    pthread_cond_t not_full;  /* 条件：非满 */
    pthread_cond_t not_empty; /* 条件：非空 */
} ring_buffer_t;

static int rb_init(ring_buffer_t *rb, size_t capacity) {
    rb->buf = (int *)malloc(capacity * sizeof(int));
    if (!rb->buf) {
        return -1;
    }
    
    rb->capacity = capacity;
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    
    if (pthread_mutex_init(&rb->mtx, NULL) != 0) {
        free(rb->buf);
        return -1;
    }
    
    if (pthread_cond_init(&rb->not_full, NULL) != 0) {
        pthread_mutex_destroy(&rb->mtx);
        free(rb->buf);
        return -1;
    }
    
    if (pthread_cond_init(&rb->not_empty, NULL) != 0) {
        pthread_cond_destroy(&rb->not_full);
        pthread_mutex_destroy(&rb->mtx);
        free(rb->buf);
        return -1;
    }
    
    return 0;
}

static void rb_destroy(ring_buffer_t *rb) {
    if (rb->buf) {
        free(rb->buf);
    }
    
    pthread_cond_destroy(&rb->not_empty);
    pthread_cond_destroy(&rb->not_full);
    pthread_mutex_destroy(&rb->mtx);
}

/* 入队：满则等待 not_full */
static void rb_push(ring_buffer_t *rb, int val) {
    pthread_mutex_lock(&rb->mtx);
    
    while (rb->count >= rb->capacity) {
        pthread_cond_wait(&rb->not_full, &rb->mtx);
    }
    
    rb->buf[rb->tail] = val;
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;
    
    pthread_cond_signal(&rb->not_empty);
    pthread_mutex_unlock(&rb->mtx);
}

/* 出队：空则等待 not_empty */
static int rb_pop(ring_buffer_t *rb, int *out) {
    pthread_mutex_lock(&rb->mtx);
    
    while (rb->count == 0) {
        pthread_cond_wait(&rb->not_empty, &rb->mtx);
    }
    
    *out = rb->buf[rb->head];
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count--;
    
    pthread_cond_signal(&rb->not_full);
    pthread_mutex_unlock(&rb->mtx);
    
    return 0;
}

typedef struct {
    ring_buffer_t *rb;
    const int *data;
    size_t n;
} producer_arg_t;

typedef struct {
    ring_buffer_t *rb;
    size_t n;
} consumer_arg_t;

static void *producer(void *arg) {
    producer_arg_t *pa = (producer_arg_t *)arg;
    
    for (size_t i = 0; i < pa->n; i++) {
        rb_push(pa->rb, pa->data[i]);
    }
    
    return NULL;
}

static void *consumer(void *arg) {
    consumer_arg_t *ca = (consumer_arg_t *)arg;
    int first = 1;
    
    for (size_t i = 0; i < ca->n; i++) {
        int val;
        rb_pop(ca->rb, &val);
        if (!first) {
            printf(",");
        }
        first = 0;
        printf("%d", val);
    }
    printf("\n");
    
    return NULL;
}

int main(void) {
    /* 输入：缓冲区容量 5，生产者线程写入 [1,2,3,4,5,6]（第 6 个元素等待消费者读取后写入）
     * 输出：消费者打印 1,2,3,4,5,6（顺序正确，无数据丢失）。
     */
    setvbuf(stdout, NULL, _IOLBF, 0); /* 行缓冲，便于在测试中看到输出 */

    ring_buffer_t rb;
    if (rb_init(&rb, 5) != 0) {
        fprintf(stderr, "ring buffer init failed\n");
        return 1;
    }

    const int data[] = {1, 2, 3, 4, 5, 6};
    const size_t n = sizeof(data) / sizeof(data[0]);

    pthread_t th_p, th_c;
    producer_arg_t pa = {.rb = &rb, .data = data, .n = n};
    consumer_arg_t ca = {.rb = &rb, .n = n};

    if (pthread_create(&th_c, NULL, consumer, &ca) != 0) {
        fprintf(stderr, "pthread_create consumer failed\n");
        rb_destroy(&rb);
        return 1;
    }
    if (pthread_create(&th_p, NULL, producer, &pa) != 0) {
        fprintf(stderr, "pthread_create producer failed\n");
        rb_destroy(&rb);
        return 1;
    }

    pthread_join(th_p, NULL);
    pthread_join(th_c, NULL);

    rb_destroy(&rb);
    return 0;
}
