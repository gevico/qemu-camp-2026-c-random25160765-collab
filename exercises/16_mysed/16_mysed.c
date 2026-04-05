#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    // 找到第一个和第二个 '/' 分隔符
    const char* start = cmd + 2;
    const char* mid = strchr(start, '/');
    if (!mid) {
        return -1;
    }
    
    const char* end = strchr(mid + 1, '/');
    if (!end) {
        return -1;
    }
    
    // 提取 old_str
    int old_len = mid - start;
    *old_str = (char*)malloc(old_len + 1);
    if (!*old_str) {
        return -1;
    }
    strncpy(*old_str, start, old_len);
    (*old_str)[old_len] = '\0';
    
    // 提取 new_str
    int new_len = end - (mid + 1);
    *new_str = (char*)malloc(new_len + 1);
    if (!*new_str) {
        free(*old_str);
        return -1;
    }
    strncpy(*new_str, mid + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    char* pos = strstr(str, old);
    if (!pos) {
        return; // 未找到旧字符串
    }
    
    int old_len = strlen(old);
    int new_len = strlen(new);
    int str_len = strlen(str);
    
    // 计算需要移动的字符数
    int shift = new_len - old_len;
    
    if (shift > 0) {
        // 向后移动字符
        memmove(pos + new_len, pos + old_len, str_len - (pos - str) - old_len + 1);
    } else if (shift < 0) {
        // 向前移动字符
        memmove(pos + new_len, pos + old_len, str_len - (pos - str) - old_len + 1);
    }
    
    // 复制新字符串
    memcpy(pos, new, new_len);
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    
    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
