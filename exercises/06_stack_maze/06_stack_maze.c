#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5
#define MAX_STACK_SIZE 100

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

typedef struct {
    int row;
    int col;
} Position;

typedef struct {
    Position data[MAX_STACK_SIZE];
    int top;
} Stack;

void init_stack(Stack *s) {
    s->top = -1;
}

void push(Stack *s, Position pos) {
    if (s->top < MAX_STACK_SIZE - 1) {
        s->data[++s->top] = pos;
    }
}

Position pop(Stack *s) {
    Position pos = {-1, -1};
    if (s->top >= 0) {
        pos = s->data[s->top--];
    }
    return pos;
}

int is_empty(Stack *s) {
    return s->top == -1;
}

int is_valid(int row, int col) {
    return row >= 0 && row < MAX_ROW && col >= 0 && col < MAX_COL && maze[row][col] == 0;
}

void mark_visited(int row, int col) {
    maze[row][col] = 2;
}

int main(void)
{
    Stack stack;
    init_stack(&stack);
    
    Position start = {0, 0};
    Position end = {4, 4};
    
    push(&stack, start);
    mark_visited(start.row, start.col);
    
    int found = 0;
    
    while (!is_empty(&stack) && !found) {
        Position current = stack.data[stack.top];
        
        if (current.row == end.row && current.col == end.col) {
            found = 1;
            break;
        }
        
        int directions[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
        int has_next = 0;
        
        for (int i = 0; i < 4; i++) {
            int new_row = current.row + directions[i][0];
            int new_col = current.col + directions[i][1];
            
            if (is_valid(new_row, new_col)) {
                Position next = {new_row, new_col};
                push(&stack, next);
                mark_visited(new_row, new_col);
                has_next = 1;
                break;
            }
        }
        
        if (!has_next) {
            pop(&stack);
        }
    }
    
    if (found) {
        for (int i = stack.top; i >= 0; i--) {
            printf("(%d, %d)\n", stack.data[i].row, stack.data[i].col);
        }
    } else {
        printf("未找到路径！\n");
    }

	return 0;
}