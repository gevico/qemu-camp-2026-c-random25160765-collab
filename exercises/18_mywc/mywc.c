#include "mywc.h"

// 创建哈希表
WordCount **create_hash_table() {
  WordCount **hash_table = calloc(HASH_SIZE, sizeof(WordCount *));
  return hash_table;
}

// 简单的哈希函数
unsigned int hash(const char *word) {
  unsigned long hash = 5381;
  int c;
  while ((c = *word++))
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash % HASH_SIZE;
}

// 检查字符是否构成单词的一部分
bool is_valid_word_char(char c) { return isalpha(c) || c == '\''; }

// 转换为小写
char to_lower(char c) { return tolower(c); }

// 添加单词到哈希表
void add_word(WordCount **hash_table, const char *word) {
  unsigned int index = hash(word);
  WordCount *current = hash_table[index];
  
  // 检查是否已存在相同的单词
  while (current) {
    if (strcmp(current->word, word) == 0) {
      current->count++;
      return;
    }
    current = current->next;
  }
  
  // 创建新的 WordCount 节点
  WordCount *new_word = malloc(sizeof(WordCount));
  if (!new_word) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  
  strncpy(new_word->word, word, MAX_WORD_LEN - 1);
  new_word->word[MAX_WORD_LEN - 1] = '\0';
  new_word->count = 1;
  new_word->next = hash_table[index];
  
  // 将新节点添加到哈希表
  hash_table[index] = new_word;
}

// 打印单词统计结果
void print_word_counts(WordCount **hash_table) {
  printf("Word Count Statistics:\n");
  printf("======================\n");

  for (int i = 0; i < HASH_SIZE; i++) {
    WordCount *current = hash_table[i];
    while (current) {
      printf("%-20s %d\n", current->word, current->count);
      current = current->next;
    }
  }
}

// 释放哈希表内存
void free_hash_table(WordCount **hash_table) {
  for (int i = 0; i < HASH_SIZE; i++) {
    WordCount *current = hash_table[i];
    while (current) {
      WordCount *next = current->next;
      free(current);
      current = next;
    }
  }
  free(hash_table);
}

// 处理文件并统计单词
void process_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  WordCount **hash_table = create_hash_table();
  char word[MAX_WORD_LEN];
  int word_pos = 0;
  int c;

  while ((c = fgetc(file)) != EOF) {
    if (is_valid_word_char(c)) {
      if (word_pos < MAX_WORD_LEN - 1) {
        word[word_pos++] = to_lower(c);
      }
    } else {
      if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(hash_table, word);
        word_pos = 0;
      }
    }
  }

  // 处理文件末尾的最后一个单词
  if (word_pos > 0) {
    word[word_pos] = '\0';
    add_word(hash_table, word);
  }

  fclose(file);
  print_word_counts(hash_table);
  free_hash_table(hash_table);
}
