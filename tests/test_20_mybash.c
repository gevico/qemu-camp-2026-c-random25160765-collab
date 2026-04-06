#include "../checker/test_framework.h"

int main() {
    test_init("20_mybash");

    printf("\n🧪 测试练习题: 20_mybash\n");

    // 检查是否还包含未完成的标记
    int has_marker = file_contains_marker("../exercises/20_mybash/src/mybash/main.c", "I AM NOT DONE");
    has_marker |= file_contains_marker("../exercises/20_mybash/src/myfile/myfile.c", "I AM NOT DONE");
    has_marker |= file_contains_marker("../exercises/20_mybash/src/myfile/myfile.h", "I AM NOT DONE");
    has_marker |= file_contains_marker("../exercises/20_mybash/src/mysed/mysed.c", "I AM NOT DONE");
    has_marker |= file_contains_marker("../exercises/20_mybash/src/mysed/mysed.h", "I AM NOT DONE");
    has_marker |= file_contains_marker("../exercises/20_mybash/src/mytrans/myhash.c", "I AM NOT DONE");
    has_marker |= file_contains_marker("../exercises/20_mybash/src/mytrans/myhash.h", "I AM NOT DONE");
    has_marker |= file_contains_marker("../exercises/20_mybash/src/mytrans/mytrans.c", "I AM NOT DONE");
    has_marker |= file_contains_marker("../exercises/20_mybash/src/mywc/mywc.c", "I AM NOT DONE");
    has_marker |= file_contains_marker("../exercises/20_mybash/src/mywc/mywc.h", "I AM NOT DONE");
    ASSERT_FALSE(has_marker, "练习题应该移除 'I AM NOT DONE' 标记");
    
    if (has_marker) {
        printf("\n💡 提示: 请编辑 exercises/20_mybash/20_mybash.c 文件，移除 'I AM NOT DONE' 标记并完成代码\n");
        printf("💡 提示: 实现一个程序 mybash, 能够传入一个脚本文件或者命令进行执行");
        test_finish();
        return 1;
    }

    const char* myfile_required_strings[] = {
        "ELF Type: Shared Object/PIE (ET_DYN) (0x3)",
        "ELF Type: Relocatable (ET_REL) (0x1)",
    };

    const char* mysed_required_strings[] = {
        "linux is opensource. unix is free os."
    };

    const char* mytrans_required_strings[] = {
        "原文: code\t翻译: n. 码;密码;法规;法典@vt. 把...编码;制成法典@n. 代码",
        "原文: empowers\t未找到该单词的翻译。",
        "原文: individuals\t未找到该单词的翻译。",
        "原文: to\t翻译: prep. 到;向;趋于@ad. 向前",
        "原文: be\t翻译: prep. 是;有;在",
        "原文: creators\t未找到该单词的翻译。",
        "原文: in\t翻译: prep. 在;在...之内;从事于;按照;穿着@ad. 进入;朝里;在家@a. 在里面的;执政的@n. 执政者;入口"
    };

    const char* mywc_required_strings[] = {
        "and                  11",
        "the                  10",
        "skilled              1",
        "just                 3"  
    };

    // 编译并运行程序
    char output[100024];  // 稍微大一些，确保容纳所有输出
    int result = compile_and_run("20_mybash", "20_mybash", output, sizeof(output), 2);
    ASSERT_EQUAL_INT(0, result, "程序应该能够成功编译和运行");
    if (result != 0) {
        printf("\n❌ 编译或运行失败:\n%s\n", output);
        test_finish();
        return 1;
    }

    // printf("📝 程序实际完整输出:\n%s\n", output);
    ASSERT_TRUE(string_contains(output, myfile_required_strings[0]), "输出应该包含'ELF Type: Shared Object/PIE (ET_DYN) (0x3)'");
    ASSERT_TRUE(string_contains(output, myfile_required_strings[1]), "输出应该包含'ELF Type: Relocatable (ET_REL) (0x1)'");
    ASSERT_TRUE(string_contains(output, mysed_required_strings[0]), "输出应该包含'linux is opensource. unix is free os.'");
    ASSERT_TRUE(string_contains(output, mytrans_required_strings[0]), "输出应该包含'原文: code\t翻译: n. 码;密码;法规;法典@vt. 把...编码;制成法典@n. 代码'");
    ASSERT_TRUE(string_contains(output, mytrans_required_strings[1]), "输出应该包含'原文: empowers\t未找到该单词的翻译。");
    ASSERT_TRUE(string_contains(output, mytrans_required_strings[2]), "输出应该包含'原文: individuals\t未找到该单词的翻译。'");
    ASSERT_TRUE(string_contains(output, mytrans_required_strings[3]), "输出应该包含'原文: to\t翻译: prep. 到;向;趋于@ad. 向前'");
    ASSERT_TRUE(string_contains(output, mytrans_required_strings[4]), "输出应该包含'原文: be\t翻译: prep. 是;有;在'");
    ASSERT_TRUE(string_contains(output, mytrans_required_strings[5]), "输出应该包含'原文: creators\t未找到该单词的翻译。'");
    ASSERT_TRUE(string_contains(output, mytrans_required_strings[6]), "输出应该包含'原文: in\t翻译: prep. 在;在...之内;从事于;按照;穿着@ad. 进入;朝里;在家@");
    ASSERT_TRUE(string_contains(output, mywc_required_strings[0]), "输出应该包含'and                  11'");
    ASSERT_TRUE(string_contains(output, mywc_required_strings[1]), "输出应该包含'the                  10'");
    ASSERT_TRUE(string_contains(output, mywc_required_strings[2]), "输出应该包含'skilled              1'");
    ASSERT_TRUE(string_contains(output, mywc_required_strings[3]), "输出应该包含'just                 3'");
    
    if (g_test_stats.failed_tests == 0){
        printf("🎉 所有期望的翻译结果均已正确输出！\n");
        strncpy(g_current_exercise.program_output, output, sizeof(g_current_exercise.program_output) - 1);
        g_current_exercise.completed = 1;
    } else {
        printf("📝 程序实际完整输出:\n%s\n", output);
        printf("💡 提示: 请确保程序对每个输入的单词，按照如下格式输出翻译或未找到提示：\n");
        printf("         - 原文: <word>      翻译: <translation内容>\n");
        printf("         - 原文: <word>  未找到该单词的翻译。\n");
        printf("         并且必须严格匹配上述 7 个示例中的字符串（包括空格与标点）。\n");
        strncpy(g_current_exercise.program_output, output, sizeof(g_current_exercise.program_output) - 1);
    }

    test_finish();
    return g_test_stats.failed_tests == 0 ? 0 : 1;
}