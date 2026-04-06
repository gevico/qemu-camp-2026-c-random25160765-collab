#include <stdio.h>
#include <stdlib.h>

#include "singly_linked_list.h"

// 打印节点的值
void print_item(link p) { printf("%d ", p->item); }

// 通过遍历拿到当前链表的头结点
static link g_first_node = NULL;
static void capture_first(link p) {
    if (g_first_node == NULL) {
        g_first_node = p;
    }
}

static link get_head_node(void) {
    g_first_node = NULL;
    traverse(capture_first);
    return g_first_node;
}

// 获取下一个节点；若到达尾部则回绕到头结点
static inline link next_wrap(link p) {
    if (p == NULL) return get_head_node();
    return p->next ? p->next : get_head_node();
}

// 创建单链表
void create_list(int n) {
    // 参数校验
    if (n <= 0) return;

    destroy();
    for (int i = n; i >= 1; i--) {
        link new_node = make_node(i);
        push(new_node);
    }
}

// 用单链表模拟约瑟夫环问题
// n: 总人数
// k: 起始位置（从 1 开始计数）
// m: 报数阈值（数到 m 的人出列）
void josephus_problem(int n, int k, int m) {
    // 参数校验
    if (n <= 0 || k <= 0 || m <= 0) {
        printf("参数错误：n, k, m 都必须大于 0\n");
        return;
    }

    create_list(n);

    // 从头开始，移动到第 k 个位置作为起点（支持 k > n 的情况，按环形回绕）
    link current = get_head_node();
    if (!current) {
        printf("\n");
        return;
    }
    for (int i = 1; i < k; ++i) {
        current = next_wrap(current);
    }

    // 依次出列并打印顺序
    for (int out = 0; out < n; ++out) {
        if (m == 1) {
            // m==1 时当前节点直接出列
            printf("%d ", current->item);
            link to_delete = current;
            current = next_wrap(current);
            delete(to_delete);
            continue;
        }

        // 数到 m 的那个人出列：从 current 开始走 m-1 步，落在第 m 个节点
        link prev = NULL;
        for (int i = 0; i < m - 1; ++i) {
            prev = current;
            current = next_wrap(current);
        }

        // 此时 current 指向要出列的人
        printf("%d ", current->item);
        link to_delete = current;
        current = next_wrap(current);
        delete(to_delete);
    }

    printf("\n");
}

int main(void) {
    josephus_problem(5, 1, 2);  // 输出结果：2 4 1 5 3
    josephus_problem(7, 3, 1);  // 输出结果：3 4 5 6 7 1 2
    josephus_problem(9, 1, 8);  // 输出结果：8 7 9 2 5 4 1 6 3

    return 0;
}
