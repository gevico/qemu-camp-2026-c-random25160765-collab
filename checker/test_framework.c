/**
 * test_framework.c
 * 
 * 简单的C语言测试框架实现
 */

 #include "test_framework.h"

 // 全局测试统计
 test_stats_t g_test_stats = {0, 0, 0};
 
 // 当前练习题结果
 exercise_result_t g_current_exercise;
 
 // 开始时间
 static clock_t start_time;
 
 /**
  * 初始化测试框架
  */
 void test_init(const char* exercise_name) {
     g_test_stats.total_tests = 0;
     g_test_stats.passed_tests = 0;
     g_test_stats.failed_tests = 0;
     
     // 初始化当前练习题结果
     memset(&g_current_exercise, 0, sizeof(g_current_exercise));
     strncpy(g_current_exercise.exercise_name, exercise_name, sizeof(g_current_exercise.exercise_name) - 1);
     g_current_exercise.individual_test_count = 0;
     
     start_time = clock();
     
     printf("\n" COLOR_BLUE "🧪 开始运行测试..." COLOR_RESET "\n");
     printf("==================================================\n");
 }
 
 /**
  * 完成测试并显示结果
  */
 void test_finish(void) {
     clock_t end_time = clock();
     g_current_exercise.test_duration_ms = (int)((end_time - start_time) * 1000 / CLOCKS_PER_SEC);
     
     // 更新练习题结果统计
     g_current_exercise.stats = g_test_stats;
     
     if (g_test_stats.failed_tests == 0) {
         strcpy(g_current_exercise.status, "PASSED");
         g_current_exercise.compiled_successfully = 1;
     } else {
         strcpy(g_current_exercise.status, "FAILED");
     }
     
     printf("\n" "==================================================\n");
     printf(COLOR_BLUE "📊 测试结果统计:" COLOR_RESET "\n");
     printf("总测试数: %d\n", g_test_stats.total_tests);
     printf(COLOR_GREEN "通过: %d" COLOR_RESET "\n", g_test_stats.passed_tests);
     printf(COLOR_RED "失败: %d" COLOR_RESET "\n", g_test_stats.failed_tests);
     
     if (g_test_stats.failed_tests == 0) {
         printf("\n" COLOR_GREEN "🎉 所有测试都通过了！" COLOR_RESET "\n");
         exit(0);
     } else {
         printf("\n" COLOR_RED "❌ 有 %d 个测试失败" COLOR_RESET "\n", g_test_stats.failed_tests);
         exit(1);
     }
 }
 
 /**
  * 编译并运行程序，捕获输出
  */
 int compile_and_run(const char* source_file, const char* executable, char* output, size_t output_size, int is_make) {
     char compile_cmd[512];
     char run_cmd[256];
     FILE* fp;
     
     // 清空输出缓冲区
     memset(output, 0, output_size);
     
    switch (is_make) {
        case 1:
        case 2: {
            snprintf(compile_cmd, sizeof(compile_cmd), "cd ../exercises/%s && make 2>&1", executable);
            fp = popen(compile_cmd, "r");
            if (fp == NULL) {
                strncpy(output, "无法执行编译命令", output_size - 1);
                return -1;
            }
        }
        break;
        default: {
            // 构建编译命令
            if (strstr(source_file, "prime") != NULL) {
                // 包含数学函数的程序需要链接数学库
                snprintf(compile_cmd, sizeof(compile_cmd), 
                        "cd ../exercises/%s && gcc -Wall -Wextra -std=c11 -o %s %s.c -lm 2>&1", executable, executable, executable);
            } else {
                snprintf(compile_cmd, sizeof(compile_cmd), 
                        "cd ../exercises/%s && gcc -Wall -Wextra -std=c11 -o %s %s.c 2>&1", executable, executable, executable);
            }
            
            // 编译程序
            fp = popen(compile_cmd, "r");
            if (fp == NULL) {
                strncpy(output, "无法执行编译命令", output_size - 1);
                return -1;
            }
        }
    }
    
     
     char compile_output[4096] = {0};
     size_t bytes_read = fread(compile_output, 1, sizeof(compile_output) - 1, fp);
     int compile_status = pclose(fp);
     
     // 检查编译是否成功
     if (compile_status != 0) {
         snprintf(output, output_size, "编译失败:\n%s", compile_output);
         return -1;
     }
     
     // 运行程序
     switch (is_make) {
        case 2: {
            snprintf(run_cmd, sizeof(run_cmd), "../exercises/%s/bin/mybash ./test_%s.sh", executable, executable);
            fp = popen(run_cmd, "r");
            if (fp == NULL) {
                strncpy(output, "无法执行程序", output_size - 1);
                return -1;
            }
        }
        break;
        case 1:
        default: {
            snprintf(run_cmd, sizeof(run_cmd), "cd ../exercises/%s && ./%s", executable, executable);
            fp = popen(run_cmd, "r");
            if (fp == NULL) {
                strncpy(output, "无法执行程序", output_size - 1);
                return -1;
            }
        }
     }

     
     
     // 读取程序输出
     bytes_read = fread(output, 1, output_size - 1, fp);
     output[bytes_read] = '\0';
     
     int run_status = pclose(fp);
     return run_status;
 }
 
 /**
  * 检查字符串是否包含子字符串
  */
 int string_contains(const char* haystack, const char* needle) {
     return strstr(haystack, needle) != NULL;
 }
 
 /**
  * 检查文件是否包含特定标记
  */
 int file_contains_marker(const char* filename, const char* marker) {
     FILE* file = fopen(filename, "r");
     if (file == NULL) {
         return 0; // 文件不存在，返回false
     }
     
     char line[256];
     int found = 0;
     
     while (fgets(line, sizeof(line), file)) {
         if (strstr(line, marker) != NULL) {
             found = 1;
             break;
         }
     }
     
     fclose(file);
     return found;
 }
 
 /**
  * 添加单个测试结果
  */
 void add_test_result(const char* name, const char* status, const char* message, int expected, int actual, int has_expected_actual) {
     if (g_current_exercise.individual_test_count >= 32) {
         return; // 防止数组越界
     }
     
     test_result_t* result = &g_current_exercise.individual_tests[g_current_exercise.individual_test_count];
     strncpy(result->name, name, sizeof(result->name) - 1);
     strncpy(result->status, status, sizeof(result->status) - 1);
     strncpy(result->message, message, sizeof(result->message) - 1);
     result->expected = expected;
     result->actual = actual;
     result->has_expected_actual = has_expected_actual;
     
     g_current_exercise.individual_test_count++;
 }
 
 /**
  * 生成JSON测试报告
  */
 void generate_json_report(const char* filename, exercise_result_t* result) {
     FILE* file = fopen(filename, "w");
     if (file == NULL) {
         printf(COLOR_RED "警告: 无法创建JSON报告文件 %s" COLOR_RESET "\n", filename);
         return;
     }
     
     // 获取当前时间
     time_t now = time(NULL);
     struct tm* tm_info = gmtime(&now);
     char timestamp[64];
     strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", tm_info);
     
     // 开始JSON输出
     fprintf(file, "{\n");
     fprintf(file, "  \"test_session\": {\n");
     fprintf(file, "    \"timestamp\": \"%s\",\n", timestamp);
     fprintf(file, "    \"exercise_name\": \"%s\",\n", result->exercise_name);
     fprintf(file, "    \"test_duration_ms\": %d\n", result->test_duration_ms);
     fprintf(file, "  },\n");
     
     // 练习题结果
     fprintf(file, "  \"exercise\": {\n");
     fprintf(file, "    \"name\": \"%s\",\n", result->exercise_name);
     fprintf(file, "    \"status\": \"%s\",\n", result->status);
     fprintf(file, "    \"test_results\": {\n");
     fprintf(file, "      \"total_tests\": %d,\n", result->stats.total_tests);
     fprintf(file, "      \"passed_tests\": %d,\n", result->stats.passed_tests);
     fprintf(file, "      \"failed_tests\": %d\n", result->stats.failed_tests);
     fprintf(file, "    },\n");
     fprintf(file, "    \"details\": {\n");
     fprintf(file, "      \"completed\": %s,\n", result->completed ? "true" : "false");
     fprintf(file, "      \"compiled_successfully\": %s\n", result->compiled_successfully ? "true" : "false");
     fprintf(file, "    },\n");
     
     // 单个测试结果
     fprintf(file, "    \"individual_tests\": [\n");
     for (int i = 0; i < result->individual_test_count; i++) {
         test_result_t* test = &result->individual_tests[i];
         fprintf(file, "      {\n");
         fprintf(file, "        \"name\": \"%s\",\n", test->name);
         fprintf(file, "        \"status\": \"%s\"", test->status);
         
         if (test->has_expected_actual) {
             fprintf(file, ",\n        \"expected\": %d,\n", test->expected);
             fprintf(file, "        \"actual\": %d", test->actual);
         }
         
         if (strlen(test->message) > 0) {
             fprintf(file, ",\n        \"message\": \"%s\"", test->message);
         }
         
         fprintf(file, "\n      }");
         if (i < result->individual_test_count - 1) {
             fprintf(file, ",");
         }
         fprintf(file, "\n");
     }
     fprintf(file, "    ]\n");
     fprintf(file, "  },\n");
     
     // 总结
     fprintf(file, "  \"summary\": {\n");
     if (result->stats.failed_tests == 0) {
         fprintf(file, "    \"overall_status\": \"SUCCESS\",\n");
         fprintf(file, "    \"message\": \"所有测试都通过了！\"\n");
     } else {
         fprintf(file, "    \"overall_status\": \"FAILED\",\n");
         fprintf(file, "    \"message\": \"有 %d 个测试失败\"\n", result->stats.failed_tests);
     }
     fprintf(file, "  }\n");
     
     fprintf(file, "}\n");
     fclose(file);
 }