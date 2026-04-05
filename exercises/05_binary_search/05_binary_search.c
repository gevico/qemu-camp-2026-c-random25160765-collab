#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 50
#define NAME_LEN     50

// 学生结构体
typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];
int n;

int binary_search(const char *target_name) {
    int left = 0;
    int right = n - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(students[mid].name, target_name);
        
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1;
}

int main(void) {
    // 打开文件读取已排序的学生信息
    FILE *file = fopen("05_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 05_students.txt\n");
        return 1;
    }

    // 读取学生人数
    fscanf(file, "%d", &n);
    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d\n", n);
        fclose(file);
        return 1;
    }

    // 读取每个学生信息
    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    char query_name[NAME_LEN] = "David";

    int index = binary_search(query_name);

    printf("\n折半查找出的排序后的学生信息：\n");
    if (index != -1) {
        printf("姓名：%s，成绩：%d\n", students[index].name, students[index].score);
    } else {
        printf("未找到该学生\n");
    }

    return 0;
}