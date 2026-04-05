#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

    if (!url) {
        return 0;
    }

    // 找到查询参数部分
    const char* query_start = strchr(url, '?');
    if (!query_start) {
        return 0; // 没有查询参数
    }
    query_start++;
    
    // 复制查询参数部分以便修改
    size_t query_len = strlen(query_start);
    char* query = (char*)malloc(query_len + 1);
    if (!query) {
        err = errno;
        return err;
    }
    strcpy(query, query_start);
    
    // 分割参数
    char* token = strtok(query, "&");
    while (token) {
        // 分割key和value
        char* equals = strchr(token, '=');
        if (equals) {
            *equals = '\0';
            char* key = token;
            char* value = equals + 1;
            
            printf("key = %s, value = %s\n", key, value);
        }
        token = strtok(NULL, "&");
    }
    
    free(query);
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}