# src/mywc/Makefile

CC = gcc
CFLAGS = -Wall -I../../include

# 库名称
LIBNAME := mywc
# 静态库输出名：libmywc.a
LIB := lib$(LIBNAME).a
# 最终库输出目录（比如 ./obj/src/mywc/）
OBJDIR ?= ./obj
# 最终 .a 文件路径
LIB_PATH := $(OBJDIR)/$(LIB)

# 源文件
SRCS := mywc.c
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