CC = g++  # 使用g++编译器
CFLAGS = -Iinc  # 包含inc目录下的头文件
LDFLAGS = -lpthread  # 链接pthread库

SRCDIR = src
BUILDDIR = build
BINDIR = bin

# YU_SERVER需要编译的源文件
SRCS_SERVER = $(SRCDIR)/YU_SERVER.cpp
OBJS_SERVER = $(BUILDDIR)/YU_SERVER.o

# YU_CLIENT需要编译的源文件
SRCS_CLIENT = $(SRCDIR)/YU_CLIENT.cpp
OBJS_CLIENT = $(BUILDDIR)/YU_CLIENT.o

# 目标可执行文件
TARGET_SERVER = $(BINDIR)/YU_SERVER
TARGET_CLIENT = $(BINDIR)/YU_CLIENT

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(OBJS_SERVER)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TARGET_CLIENT): $(OBJS_CLIENT)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/YU_SERVER.o: $(SRCS_SERVER)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/YU_CLIENT.o: $(SRCS_CLIENT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf $(BUILDDIR) $(BINDIR)
