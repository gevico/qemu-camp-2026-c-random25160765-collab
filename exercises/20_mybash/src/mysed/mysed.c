#include "mysed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    // 检查输入参数有效性
    if (!cmd || !old_str || !new_str) {
        return -1;
    }
    
    // 初始化输出参数
    *old_str = NULL;
    *new_str = NULL;
    
    // 检查命令是否以 "s/" 开头
    if (strlen(cmd) < 3 || strncmp(cmd, "s/", 2) != 0) {
        return -1;
    }
    
    // 找到第一个 "/" 分隔符
    const char* first_slash = cmd + 2;
    const char* second_slash = strchr(first_slash, '/');
    if (!second_slash) {
        return -1;
    }
    
    // 找到第三个 "/" 分隔符（可选）
    const char* third_slash = strchr(second_slash + 1, '/');
    if (!third_slash) {
        third_slash = second_slash + strlen(second_slash);
    }
    
    // 提取 old_str
    size_t old_len = second_slash - first_slash;
    *old_str = (char*)malloc(old_len + 1);
    if (!*old_str) {
        return -1;
    }
    strncpy(*old_str, first_slash, old_len);
    (*old_str)[old_len] = '\0';
    
    // 提取 new_str
    size_t new_len = third_slash - (second_slash + 1);
    *new_str = (char*)malloc(new_len + 1);
    if (!*new_str) {
        free(*old_str);
        *old_str = NULL;
        return -1;
    }
    strncpy(*new_str, second_slash + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // 检查输入参数有效性
    if (!str || !old || !new) {
        return;
    }
    
    // 找到 old 字符串在 str 中的第一个出现位置
    char* pos = strstr(str, old);
    if (!pos) {
        return; // 未找到，直接返回
    }
    
    // 计算替换后的字符串长度
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    size_t str_len = strlen(str);
    size_t new_str_len = str_len - old_len + new_len;
    
    // 为新字符串分配内存
    char* new_str = (char*)malloc(new_str_len + 1);
    if (!new_str) {
        return;
    }
    
    // 复制 old 字符串之前的部分
    size_t prefix_len = pos - str;
    strncpy(new_str, str, prefix_len);
    new_str[prefix_len] = '\0';
    
    // 复制 new 字符串
    strcat(new_str, new);
    
    // 复制 old 字符串之后的部分
    strcat(new_str, pos + old_len);
    
    // 将新字符串复制回原字符串
    strcpy(str, new_str);
    
    // 释放内存
    free(new_str);
}

int __cmd_mysed(const char* rules, const char* str) {
    // 检查输入参数有效性
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    printf("rules: %s\n", rules);
    printf("str: %s\n", str);

    char* old_str = NULL;
    char* new_str = NULL;

    // 解析规则，例如 "s/old/new/"
    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[1024];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}