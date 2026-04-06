#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#ifdef DEBUG_LEVEL

#if DEBUG_LEVEL == 1
#define DEBUG_PRINT(fmt, ...) \
    printf("DEBUG: func=%s, line=%d\n", __func__, __LINE__)
#elif DEBUG_LEVEL == 2
#define DEBUG_PRINT(fmt, ...) \
    printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)
#elif DEBUG_LEVEL == 3
#define DEBUG_PRINT(fmt, ...) \
    do { \
        printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); \
        void* callstack[10]; \
        int frames = backtrace(callstack, 10); \
        char** symbols = backtrace_symbols(callstack, frames); \
        if (symbols) { \
            for (int i = 0; i < frames; i++) { \
                printf("DEBUG: stack[%d]: %s\n", i, symbols[i]); \
            } \
            free(symbols); \
        } \
    } while (0)
#endif

#else

#define DEBUG_PRINT(fmt, ...) do {} while (0)

#endif









//! MUST BE ENSURE THE DEBUG_PRINT("x=%d", x) AT THE 48 LINE

// 测试代码
void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}