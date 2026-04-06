# src/mybash/Makefile

CC ?= gcc
CFLAGS ?= -Wall -I../../include

TARGET := mybash

BINDIR ?= ./bin
OBJDIR ?= ./obj

# 接收顶层传来的链接参数（清空默认值）
LDFLAGS ?=
LDLIBS  ?=

SRCS := main.c
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

TARGET_PATH := $(BINDIR)/$(TARGET)

all: $(TARGET_PATH)

# 创建目录
$(BINDIR) $(OBJDIR):
	@mkdir -p $@

# 编译
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 链接 —— 只使用 LDFLAGS 和 LDLIBS（最关键）
$(TARGET_PATH): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	@rm -rf $(OBJDIR) $(TARGET_PATH)

.PHONY: all clean