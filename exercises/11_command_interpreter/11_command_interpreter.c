#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ARGS 10
#define MAX_INPUT_LENGTH 256

int shell_parse(char *buf, char *argv[]);
void execute_command(int argc, char *argv[]);

// in:  format -> hello world "hello world" 1234567
// Multiple command parameters entered from the command line (number of parameters does not exceed 10)
// out:  format -> Parameter X: Content, Length: X
// Parse the command parameters through a character pointer array, and sequentially display the content and length of each parameter on the screen
//
int main(void)
{
    FILE *file;
    char input[MAX_INPUT_LENGTH];
    char *argv[MAX_ARGS + 1] = {NULL};
    int argc = 0;

    file = fopen("command_file.txt", "r");
    if (!file) {
        fprintf(stderr, "❌ Error: Cannot open input file '%s'\n", "command_file.txt");
        return 1;
    }

    printf("✅ Reading commands from '%s':\n\n", "command_file.txt");

    while (fgets(input, MAX_INPUT_LENGTH, file) != NULL) {
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0 || strspn(input, " \t") == strlen(input)) {
            continue;
        }

        printf("➡️  Input: %s\n", input);

        argc = shell_parse(input, argv);

        if (argc == 0) {
            printf("⚠️  No valid command parsed.\n\n");
            continue;
        }

        execute_command(argc, argv);
        printf("\n");
    }

    fclose(file);
    return 0;
}

// shell_parse 和 execute_command 保持不变
int shell_parse(char *buf, char *argv[])
{
    int argc = 0;
    int state = 0; // 0: 空白状态, 1: 普通参数状态, 2: 引号状态
    char *start = buf;
    
    while (*buf != '\0') {
        switch (state) {
            case 0: // 空白状态
                if (*buf == ' ') {
                    buf++;
                } else if (*buf == '"') {
                    state = 2;
                    start = buf + 1;
                    buf++;
                } else {
                    state = 1;
                    start = buf;
                    buf++;
                }
                break;
                
            case 1: // 普通参数状态
                if (*buf == ' ') {
                    *buf = '\0';
                    argv[argc++] = start;
                    state = 0;
                    buf++;
                } else if (*buf == '\0') {
                    argv[argc++] = start;
                    state = 0;
                } else {
                    buf++;
                }
                break;
                
            case 2: // 引号状态
                if (*buf == '"') {
                    *buf = '\0';
                    argv[argc++] = start;
                    state = 0;
                    buf++;
                } else if (*buf == '\0') {
                    argv[argc++] = start;
                    state = 0;
                } else {
                    buf++;
                }
                break;
        }
    }
    
    // 处理最后一个参数
    if (state == 1 || state == 2) {
        argv[argc++] = start;
    }
    
    argv[argc] = NULL;
    return argc;
}

void execute_command(int argc, char *argv[])
{
    printf("Parsing result: Total %d parameters\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("Parameter %d: Content: %s, Length: %zu\n",
               i + 1, argv[i], strlen(argv[i]));
    }

    if (strcmp(argv[0], "help") == 0) {
        printf("This is help command\n");
    } else if (strcmp(argv[0], "echo") == 0) {
        printf("Echo: ");
        for (int i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
    } else if (strcmp(argv[0], "add") == 0 && argc == 3) {
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        printf("%d + %d = %d\n", a, b, a + b);
    } else {
        printf("Unknown command: %s\n", argv[0]);
    }
}
