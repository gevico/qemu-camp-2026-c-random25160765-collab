#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

// 全局头指针
static link head = NULL;

// 创建新节点
link make_node(unsigned char item) {
    link p = (link)malloc(sizeof(struct node));
    if (p) {
        p->item = item;
        p->next = NULL;
    }
    return p;
}

// 释放节点
void free_node(link p) { free(p); }

// 查找节点
link search(unsigned char key) {
    link p = head;
    while (p) {
        if (p->item == key) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// 在链表头部插入节点
void insert(link p) {
    if (p) {
        p->next = head;
        head = p;
    }
}

// 删除指定节点
void delete(link p) {
    if (!head || !p) return;
    
    if (head == p) {
        head = head->next;
        free(p);
        return;
    }
    
    link prev = head;
    while (prev->next && prev->next != p) {
        prev = prev->next;
    }
    
    if (prev->next == p) {
        prev->next = p->next;
        free(p);
    }
}

// 遍历链表
void traverse(void (*visit)(link)) {
    if (!visit) return;
    link p = head;
    while (p) {
        visit(p);
        p = p->next;
    }
}

// 销毁整个链表
void destroy(void) {
    link p;
    while (head) {
        p = head;
        head = head->next;
        free(p);
    }
}

// 在链表头部推入节点
void push(link p) { 
    if (p) {
        p->next = head;
        head = p;
    }
 }

// 从链表头部弹出节点
link pop(void) {
    if (!head) return NULL;
    link p = head;
    head = head->next;
    p->next = NULL;
    return p;
}

// 释放链表内存
void free_list(link list_head) {
    link p;
    while (list_head) {
        p = list_head;
        list_head = list_head->next;
        free(p);
    }
}
