#include <stdio.h>

// 定义四则运算函数
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b == 0) {
        printf("错误：除数不能为0\n");
        return 0;
    }
    return a / b;
}

int main() {
    int a, b;
    char op;

    // 函数指针数组
    int (*operations[])(int, int) = {add, subtract, multiply, divide};

    char operators[] = "+-*/";

    printf("输入两个整数和一个运算符 (+, -, *, /): ");
    scanf("%d %d %c", &a, &b, &op);

    // 查找对应的运算符索引
    int index = -1;
    for (int i = 0; i < 4; i++) {
        if (op == operators[i]) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("无效的运算符\n");
        return 1;
    }

    int result = operations[index](a, b);
    printf("%d %c %d = %d\n", a, op, b, result);

    return 0;
}