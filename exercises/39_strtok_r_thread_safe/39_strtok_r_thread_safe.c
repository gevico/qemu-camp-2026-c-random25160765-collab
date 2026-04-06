#include <stdio.h>
#include <stdlib.h>

/*
 * 19 字符串分割器（类似 strtok_r）
 * 目标：实现线程安全的字符串分割函数，不依赖静态全局状态
 * 原型：char* strtok_r(char *str, const char *delim, char **saveptr)
 * 示例：输入 "hello,world test"，分隔符 ", " → 依次得到 hello、world、test
 */

/* 判断字符 c 是否在分隔符集合 delim 中 */
static int is_delim(char c, const char *delim) {
    while (*delim) {
        if (c == *delim++) {
            return 1;
        }
    }
    return 0;
}

/* 线程安全版本：通过 saveptr 维护调用状态，不使用静态变量 */
char *strtok_r(char *str, const char *delim, char **saveptr) {
    char *start, *end;
    
    // 第一次调用时，使用传入的 str
    // 后续调用时，使用 saveptr 保存的位置
    start = str ? str : *saveptr;
    
    // 跳过开头的分隔符
    while (*start && is_delim(*start, delim)) {
        start++;
    }
    
    // 如果已经到达字符串末尾，返回 NULL
    if (!*start) {
        *saveptr = start;
        return NULL;
    }
    
    // 找到当前 token 的结束位置
    end = start + 1;
    while (*end && !is_delim(*end, delim)) {
        end++;
    }
    
    // 如果不是字符串末尾，将结束位置设置为 '\0'，并保存下一次的起始位置
    if (*end) {
        *end = '\0';
        *saveptr = end + 1;
    } else {
        *saveptr = end;
    }
    
    return start;
}

int main(void) {
    char buf[] = "hello,world test";
    const char *delim = ", ";
    char *save = NULL;

    char *tok = strtok_r(buf, delim, &save);
    while (tok) {
        printf("%s\n", tok);
        tok = strtok_r(NULL, delim, &save);
    }
    return 0;
}
