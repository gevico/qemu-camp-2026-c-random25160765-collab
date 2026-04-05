#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_QUEUE_SIZE 100

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

// 坐标结构体
typedef struct {
    int row;
    int col;
} Position;

// 队列结构体
typedef struct {
    Position data[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// 父节点数组，用于记录路径
Position parent[MAX_ROW][MAX_COL];

// 初始化队列
void init_queue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

// 入队
void enqueue(Queue *q, Position pos) {
    if ((q->rear + 1) % MAX_QUEUE_SIZE != q->front) {
        q->data[q->rear] = pos;
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    }
}

// 出队
Position dequeue(Queue *q) {
    Position pos = {-1, -1};
    if (q->front != q->rear) {
        pos = q->data[q->front];
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    }
    return pos;
}

// 检查队列是否为空
int is_empty(Queue *q) {
    return q->front == q->rear;
}

// 检查位置是否有效
int is_valid(int row, int col) {
    return row >= 0 && row < MAX_ROW && col >= 0 && col < MAX_COL && maze[row][col] == 0;
}

// 标记已访问的位置
void mark_visited(int row, int col) {
    maze[row][col] = 2; // 2 表示已访问
}

// 打印路径
void print_path(Position end) {
    Position path[MAX_ROW * MAX_COL];
    int path_length = 0;
    
    // 收集路径
    Position current = end;
    while (current.row != -1 && current.col != -1) {
        path[path_length++] = current;
        current = parent[current.row][current.col];
    }
    
    // 逆序输出路径（从终点到起点）
    for (int i = 0; i < path_length; i++) {
        printf("(%d, %d)\n", path[i].row, path[i].col);
    }
}

int main(void)
{
    Queue queue;
    init_queue(&queue);
    
    // 初始化父节点数组
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            parent[i][j].row = -1;
            parent[i][j].col = -1;
        }
    }
    
    // 起点 (0, 0)，终点 (4, 4)
    Position start = {0, 0};
    Position end = {4, 4};
    
    enqueue(&queue, start);
    mark_visited(start.row, start.col);
    
    int found = 0;
    
    while (!is_empty(&queue) && !found) {
        Position current = dequeue(&queue);
        
        // 检查是否到达终点
        if (current.row == end.row && current.col == end.col) {
            found = 1;
            break;
        }
        
        // 尝试四个方向：下、右、上、左
        int directions[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
        
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + directions[i][0];
            int new_col = current.col + directions[i][1];
            
            if (is_valid(new_row, new_col)) {
                Position next = {new_row, new_col};
                enqueue(&queue, next);
                mark_visited(new_row, new_col);
                parent[new_row][new_col] = current;
            }
        }
    }
    
    // 输出结果
    if (found) {
        printf("找到路径！\n");
        printf("路径：\n");
        print_path(end);
        printf("\n");
    } else {
        printf("未找到路径！\n");
    }

	return 0;
}