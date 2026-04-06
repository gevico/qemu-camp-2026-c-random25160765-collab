# src/mybash/Makefile - 硬编码链接版

CC ?= gcc
CFLAGS ?= -Wall -I../../include

TARGET := mybash
OBJDIR ?= ./obj

SRCS := main.c
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

all: $(PWD)/bin/mybash

# 创建目录
$(PWD)/bin $(OBJDIR):
	@mkdir -p $@

# 编译 .o
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 链接 —— 使用顶层传来的完整命令（一行写死）
$(PWD)/bin/mybash: $(OBJS) | $(PWD)/bin
	$(LINKCMD)

clean:
	@rm -rf $(OBJDIR) $(PWD)/bin/mybash

.PHONY: all clean