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

我的`src`下的`Test.cpp`是能发到上位机的。我用的`RawData`协议，等会搞一个`justfloat`的。

新学的命令：

#### 2.1 快速编译

```c
g++ -o Test Test.cpp &&./Test
```

将`Test.cpp`编译成`binary`文件，同时运行。

#### 2.2 查看是否连接到端口号

在写好`socket`代码后，运行后如果程序跑的和想象中的不一样，那么需要看一下端口号是不是配对了。

注意，一定要运行了你的`socket`程序后再看：

```c
netstat -lp
```

查看端口号。

我的程序运行后：

```c
root@Cofallen:~/YU_Repo/gitea_repo/Socket# netstat -lp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name    
tcp        0      0 0.0.0.0:ssh             0.0.0.0:*               LISTEN      658/sshd: /usr/sbin 
tcp        0      0 localhost:36325         0.0.0.0:*               LISTEN      43039/node          
tcp        0      0 localhost:55878         0.0.0.0:*               LISTEN      44887/node          
udp        0      0 0.0.0.0:12345           0.0.0.0:*                           46760/./Test        
udp        0      0 0.0.0.0:bootpc          0.0.0.0:*                           521/dhclient        
udp        0      0 localhost:323           0.0.0.0:*                           687/chronyd         
udp6       0      0 localhost:323           [::]:*                              687/chronyd         
Active UNIX domain sockets (only servers)
Proto RefCnt Flags       Type       State         I-Node   PID/Program name     Path
unix  2      [ ACC ]     SEQPACKET  LISTENING     2983     1/init               /run/udev/control
unix  2      [ ACC ]     STREAM     LISTENING     1658504  44785/systemd        /run/user/1000/systemd/private
unix  2      [ ACC ]     STREAM     LISTENING     1586173  42960/systemd        /run/user/0/systemd/private
unix  2      [ ACC ]     STREAM     LISTENING     1658509  44785/systemd        /run/user/1000/gnupg/S.dirmngr
unix  2      [ ACC ]     STREAM     LISTENING     1658511  44785/systemd        /run/user/1000/gnupg/S.gpg-agent.browser
unix  2      [ ACC ]     STREAM     LISTENING     1586176  42960/systemd        /run/user/0/gnupg/S.dirmngr
unix  2      [ ACC ]     STREAM     LISTENING     1658513  44785/systemd        /run/user/1000/gnupg/S.gpg-agent.extra
unix  2      [ ACC ]     STREAM     LISTENING     1587202  42960/systemd        /run/user/0/gnupg/S.gpg-agent.browser
unix  2      [ ACC ]     STREAM     LISTENING     1658515  44785/systemd        /run/user/1000/gnupg/S.gpg-agent.ssh
unix  2      [ ACC ]     STREAM     LISTENING     1587204  42960/systemd        /run/user/0/gnupg/S.gpg-agent.extra
unix  2      [ ACC ]     STREAM     LISTENING     1658517  44785/systemd        /run/user/1000/gnupg/S.gpg-agent
unix  2      [ ACC ]     STREAM     LISTENING     1587206  42960/systemd        /run/user/0/gnupg/S.gpg-agent.ssh
unix  2      [ ACC ]     STREAM     LISTENING     1587208  42960/systemd        /run/user/0/gnupg/S.gpg-agent
unix  2      [ ACC ]     STREAM     LISTENING     11284    213/systemd-journal  /run/systemd/journal/io.systemd.journal
unix  2      [ ACC ]     STREAM     LISTENING     1660935  44887/node           /run/user/0/vscode-ipc-93da2809-e1aa-4697-9649-6b544a463386.sock
unix  2      [ ACC ]     STREAM     LISTENING     1660704  44887/node           /run/user/0/vscode-git-e28d2bd16a.sock
unix  2      [ ACC ]     STREAM     LISTENING     1661395  43039/node           /run/user/0/vscode-ipc-4e3be23e-441f-44dd-8a06-688a9a0314ce.sock
unix  2      [ ACC ]     STREAM     LISTENING     1671175  43039/node           /run/user/0/vscode-ipc-37d2acab-7f36-4503-9d27-1b20f021bc09.sock
unix  2      [ ACC ]     STREAM     LISTENING     1634065  44316/aliyun-servic  /tmp/aliyun_assist_service.sock
unix  2      [ ACC ]     STREAM     LISTENING     13353    1/init               /run/dbus/system_bus_socket
unix  2      [ ACC ]     STREAM     LISTENING     13977    853/AliYunDun        /usr/local/aegis/Aegis-<Guid(5A2C30A2-A87D-490A-9281-6765EDAD7CBA)>
unix  2      [ ACC ]     STREAM     LISTENING     2959     1/init               /run/systemd/private
unix  2      [ ACC ]     STREAM     LISTENING     2961     1/init               /run/systemd/userdb/io.systemd.DynamicUser
unix  2      [ ACC ]     STREAM     LISTENING     2962     1/init               /run/systemd/io.system.ManagedOOM
unix  2      [ ACC ]     STREAM     LISTENING     2972     1/init               /run/systemd/fsck.progress
unix  2      [ ACC ]     STREAM     LISTENING     2980     1/init               /run/systemd/journal/stdout
```

能看到我的`UDP`端口号是`12345`。
