# =============================================
# 主 Makefile
# 功能：
#   - 整合所有构建规则，杜绝递归 makefile
#   - 生成 mybash 可执行文件到 bin/
#   - 生成各子模块静态库到 obj/xxx/
# =============================================

PWD := $(shell pwd)
BINDIR := $(PWD)/bin

# 编译器设置
CC := gcc
CFLAGS := -Wall -I$(PWD)/include

# 子模块目录
MODULES := mysed myfile mywc mytrans

# 各模块源文件
MYSED_SRCS := src/mysed/mysed.c
MYFILE_SRCS := src/myfile/myfile.c
MYWC_SRCS := src/mywc/mywc.c
MYTRANS_SRCS := src/mytrans/mytrans.c src/mytrans/myhash.c
MYBASH_SRCS := src/mybash/main.c

# 各模块目标文件
MYSED_OBJS := $(patsubst src/mysed/%.c,$(PWD)/obj/mysed/%.o,$(MYSED_SRCS))
MYFILE_OBJS := $(patsubst src/myfile/%.c,$(PWD)/obj/myfile/%.o,$(MYFILE_SRCS))
MYWC_OBJS := $(patsubst src/mywc/%.c,$(PWD)/obj/mywc/%.o,$(MYWC_SRCS))
MYTRANS_OBJS := $(patsubst src/mytrans/%.c,$(PWD)/obj/mytrans/%.o,$(MYTRANS_SRCS))
MYBASH_OBJS := $(patsubst src/mybash/%.c,$(PWD)/obj/mybash/%.o,$(MYBASH_SRCS))

# 静态库路径
MYSED_LIB := $(PWD)/obj/mysed/libmysed.a
MYFILE_LIB := $(PWD)/obj/myfile/libmyfile.a
MYWC_LIB := $(PWD)/obj/mywc/libmywc.a
MYTRANS_LIB := $(PWD)/obj/mytrans/libmytrans.a

# 可执行文件路径
MYBASH_BIN := $(BINDIR)/mybash

# 默认目标：构建 mybash
all: $(MYBASH_BIN)

# 创建目录
$(BINDIR) $(PWD)/obj/mysed $(PWD)/obj/myfile $(PWD)/obj/mywc $(PWD)/obj/mytrans $(PWD)/obj/mybash:
	@mkdir -p $@

# 编译 mysed
$(PWD)/obj/mysed/%.o: src/mysed/%.c | $(PWD)/obj/mysed
	$(CC) $(CFLAGS) -c $< -o $@

# 编译 myfile（特殊 CFLAGS）
$(PWD)/obj/myfile/%.o: src/myfile/%.c | $(PWD)/obj/myfile
	$(CC) $(CFLAGS) -g -fPIE -c $< -o $@

# 编译 mywc
$(PWD)/obj/mywc/%.o: src/mywc/%.c | $(PWD)/obj/mywc
	$(CC) $(CFLAGS) -c $< -o $@

# 编译 mytrans
$(PWD)/obj/mytrans/%.o: src/mytrans/%.c | $(PWD)/obj/mytrans
	$(CC) $(CFLAGS) -c $< -o $@

# 编译 mybash
$(PWD)/obj/mybash/%.o: src/mybash/%.c | $(PWD)/obj/mybash
	$(CC) $(CFLAGS) -c $< -o $@

# 创建 mysed 静态库
$(MYSED_LIB): $(MYSED_OBJS)
	ar rcs $@ $^

# 创建 myfile 静态库
$(MYFILE_LIB): $(MYFILE_OBJS)
	ar rcs $@ $^

# 创建 mywc 静态库
$(MYWC_LIB): $(MYWC_OBJS)
	ar rcs $@ $^

# 创建 mytrans 静态库
$(MYTRANS_LIB): $(MYTRANS_OBJS)
	ar rcs $@ $^

# 链接 mybash
$(MYBASH_BIN): $(MYBASH_OBJS) $(MYSED_LIB) $(MYFILE_LIB) $(MYWC_LIB) $(MYTRANS_LIB) | $(BINDIR)
	$(CC) $(CFLAGS) $(MYBASH_OBJS) -o $@ \
		-L$(PWD)/obj/mysed -L$(PWD)/obj/mywc -L$(PWD)/obj/mytrans -L$(PWD)/obj/myfile \
		-lmysed -lmywc -lmytrans -lmyfile

# 清理所有生成的文件
clean:
	@echo "Cleaning all..."
	@rm -rf $(PWD)/obj $(PWD)/bin

.PHONY: all clean