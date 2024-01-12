# socket有关整理

* 目录
* [1.分文件编译 Makefile](#1-分文件编译makefile)
* [2.socket文件分析](#2-socket-文件分析)

## 1. 分文件编译Makefile

这个Makefile可以分为以下几个部分：

### 1. 定义变量

```makefile
CC = g++  # 使用g++编译器
CFLAGS = -Iinc  # 包含inc目录下的头文件
LDFLAGS = -lpthread  # 链接pthread库

SRCDIR = src
BUILDDIR = build
BINDIR = bin
```

在这一部分中，我们定义了编译器（CC）、编译选项（CFLAGS）、链接选项（LDFLAGS）以及源代码目录（SRCDIR）、编译文件目录（BUILDDIR）、可执行文件目录（BINDIR）等变量。

### 2. 定义需要编译的源文件和目标文件

```makefile
SRCS_SERVER = $(SRCDIR)/YU_SERVER.cpp
OBJS_SERVER = $(BUILDDIR)/YU_SERVER.o

SRCS_CLIENT = $(SRCDIR)/YU_CLIENT.cpp
OBJS_CLIENT = $(BUILDDIR)/YU_CLIENT.o
```

在这一部分中，我们定义了YU_SERVER.cpp和YU_CLIENT.cpp的源文件路径，以及它们对应的目标文件路径。

### 3. 定义目标

```makefile
TARGET_SERVER = $(BINDIR)/YU_SERVER
TARGET_CLIENT = $(BINDIR)/YU_CLIENT
```

我们定义了YU_SERVER和YU_CLIENT的目标可执行文件路径。

### 4. 定义构建规则

```makefile
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
```

这一部分定义了构建规则，包括了两个目标`all`、`clean`和四个规则。`all`目标表示默认构建的目标，它依赖于`$(TARGET_SERVER)`和`$(TARGET_CLIENT)`。`$(TARGET_SERVER)`和`$(TARGET_CLIENT)`分别依赖于对应的目标文件，通过编译器将目标文件链接成可执行文件。目标文件则依赖于对应的源文件，通过编译器将源文件编译成目标文件。

### 5. 定义清理规则

```makefile
.PHONY: clean

clean:
 rm -rf $(BUILDDIR) $(BINDIR)
```

这一部分定义了清理规则，通过`make clean`命令可以删除编译生成的目标文件和可执行文件。

这样，整个Makefile文件按照这个逻辑组织，实现了对YU_SERVER.cpp和YU_CLIENT.cpp的分别编译和生成可执行文件的需求。

## 2. socket 文件分析
