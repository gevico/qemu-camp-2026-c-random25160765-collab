// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    if (!str) return;
    
    // 去除开头的空白字符
    int start = 0;
    while (str[start] && isspace((unsigned char)str[start])) {
        start++;
    }
    
    // 去除结尾的空白字符
    int end = strlen(str) - 1;
    while (end >= start && isspace((unsigned char)str[end])) {
        end--;
    }
    
    // 移动字符串
    if (start > 0) {
        memmove(str, str + start, end - start + 1);
    }
    str[end - start + 1] = '\0';
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;
  *dict_count = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    // 去除行尾的换行符
    line[strcspn(line, "\n")] = '\0';
    
    if (line[0] == '#') {
      // 新词条开始
      if (in_entry) {
        // 保存上一个词条
        trim(current_word);
        trim(current_translation);
        if (strlen(current_word) > 0 && strlen(current_translation) > 0) {
          hash_table_insert(table, current_word, current_translation);
          (*dict_count)++;
        }
      }
      // 提取新单词
      strncpy(current_word, line + 1, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      current_translation[0] = '\0';
      in_entry = 1;
    } else if (strncmp(line, "Trans:", 6) == 0) {
      // 翻译部分
      if (in_entry) {
        strncpy(current_translation, line + 6, sizeof(current_translation) - 1);
        current_translation[sizeof(current_translation) - 1] = '\0';
      }
    }
  }

  // 处理最后一个词条
  if (in_entry) {
    trim(current_word);
    trim(current_translation);
    if (strlen(current_word) > 0 && strlen(current_translation) > 0) {
      hash_table_insert(table, current_word, current_translation);
      (*dict_count)++;
    }
  }

  fclose(file);
  return 0;
}
