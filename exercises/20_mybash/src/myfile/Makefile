# src/myfile/Makefile

CC = gcc
CFLAGS = -Wall -I../../include -g -fPIE

# 库名称
LIBNAME := myfile
# 静态库输出名：libmyfile.a
LIB := lib$(LIBNAME).a
# 最终库输出目录（比如 ./obj/src/myfile/）
OBJDIR ?= ./obj
# 最终 .a 文件路径
LIB_PATH := $(OBJDIR)/$(LIB)

# 源文件
SRCS := myfile.c
OBJS := $(SRCS:.c=.o)
OBJS := $(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))

all: $(LIB_PATH)

# 创建目标目录
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# 编译 .c 文件为 .o 文件
$(OBJDIR)/%.o: %.c $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 创建静态库
$(LIB_PATH): $(OBJS)
	ar rcs $@ $^

# 清理生成的文件
clean:
	@rm -rf $(OBJDIR)/*.o $(LIB_PATH)