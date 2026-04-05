#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

// 初始化队列
void init_queue(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

// 入队
void enqueue(Queue *q, People p) {
    if (q->count < MAX_PEOPLE) {
        q->data[q->tail] = p;
        q->tail = (q->tail + 1) % MAX_PEOPLE;
        q->count++;
    }
}

// 出队
People dequeue(Queue *q) {
    People p = {0};
    if (q->count > 0) {
        p = q->data[q->head];
        q->head = (q->head + 1) % MAX_PEOPLE;
        q->count--;
    }
    return p;
}

int main() {
    Queue q;
    int total_people=50;
    int report_interval=5;

    // 初始化队列
    init_queue(&q);
    
    // 将50个人加入队列
    for (int i = 1; i <= total_people; i++) {
        People p;
        p.id = i;
        enqueue(&q, p);
    }
    
    // 模拟约瑟夫环过程
    while (q.count > 1) {
        // 数到第report_interval-1个人时，将第report_interval个人出队
        for (int i = 1; i < report_interval; i++) {
            People p = dequeue(&q);
            enqueue(&q, p);
        }
        People removed = dequeue(&q); // 移除第report_interval个人
        printf("淘汰: %d\n", removed.id); // 按出圈顺序输出
    }
    
    printf("最后剩下的人是: %d\n", q.data[q.head].id); // 输出最后剩下的人

    return 0;
}