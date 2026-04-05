#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[20];
    char name[50];
    int age;
} Student;

int main() {
    FILE *file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        return 1;
    }
    
    Student *students[3];
    
    for (int i = 0; i < 3; i++) 
    {
        // 为每个学生分配内存
        students[i] = (Student *)malloc(sizeof(Student));
        if (students[i] == NULL) {
            printf("内存分配失败\n");
            return 1;
        }
        
        // 从文件中读取学生信息
        fscanf(file, "%s %s %d", students[i]->id, students[i]->name, &students[i]->age);
    }
    fclose(file);
    
    for (int i = 0; i < 3; i++) 
    {
        printf("学号：%s, 姓名：%s, 年龄：%d\n", 
               students[i]->id, 
               students[i]->name, 
               students[i]->age);
    }
    
    for (int i = 0; i < 3; i++) 
    {
        free(students[i]);
    }
    
    return 0;
}