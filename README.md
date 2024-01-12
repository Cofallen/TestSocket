# 一些整理

> 这是我仓库崩了很多次的思考。。
>
> 为了~~造福人类~~，为了整理下我在搞东西遇到的问题，我写下这个东西，便于以后出现问题好搞。
>
> 这玩意我还得开个备份，仓库别没了。。

> 目录
>
> [1.rm-rf 你值得拥有](#1-rm--f-的作用)

> [2.重置服务器后配置](#2-重置服务器重配置)

> [3.关联本地文件](#3-关联本地文件到远程仓库)

## 1. rm -f/* 的作用

千万不要在任何文件夹下

```bash
rm -f/*
```

或者

```bash
rm -rf/*
```

起初我还以为这玩意只在当前文件夹下起作用，直到第三次把系统文件删了我才知道这玩意是直接删除根目录下所有文件，不询问。。

要是你好奇的话，可以像我一样试试。

我现在也有点奇怪那个东西，在`makefile`中编辑时有这一行代码

```bash
clean:
    rm -f bin/*
    rm -f build/*
```

这不是只`clean`了目标文件吗，也没把系统文件删了。

谁知道，告诉我一声。

## 2. 重置服务器重配置

~~哈哈哈哈，你来了~~

### 2.1 阿里云服务器重置

*当遇到你的系统文件删了，服务器连不上什么的问题，就来重置服务器吧
在此之前，先看看能不能登录*

**2.2.1 阿里云是否能登录**

打开[阿里云服务器官网](https://cn.aliyun.com/)，找到你的系统镜像`Debian`,找到 : *远程连接*，选择第一个`Workbentch登录`，登不上就对了。

选择`救援登录`，应该能登上，但是全黑屏。

来参加光荣的重置吧！

**2.2.2 重置阿里云服务器**

点开`更多操作` -> `重置系统`，可能要验证码什么的。

重置后按照2.2.1的步骤看一下是否能默认登录(不搞也没事，肯定可以)

**注意：重置服务器后你的文件会消失，请在此之前上传到仓库或者搞好备份**

**2.2.3 更改密码和名字**

找到`重置密码`按钮，可能还需要验证码。

在`cmd`中输入`ssh root@<your_address>`，提示输入密码（看不到），输入回车键确认，看到你的名字是乱码。

输入

```bash
hostname
```

查看你的用户名.

输入

```bash
hostnamectl set-hostname <yourname>
```

修改你的用户名.

*修改后你要重新启动服务器*

**2.2.4 设置免密登录**

找到C盘文件下的`.ssh`文件夹，清空。这是我的路径。

```
C:\Users\17273\.ssh
```

打开`cmd`，输入ssh-keygen,一路回车，生成新的密钥。

找到`.pub`结尾的文件，用记事本打开，`ctrl+a`全选复制。

打开vscode,连接到你的远程`ssh`,要输入密码。

使用

```bash
ls -al
```

查看你的系统文件。（我就是把这个删了）

能看到有`.ssh`文件，删了，再建一个。

```bash
rm -rf .ssh
mkdir .ssh
cd .ssh
```

在`.ssh`下将密钥保存到`authorized_keys`中。

```bash
vi authorized_keys
```

然后在键盘输入`i`,进入插入模式，`ctrl+v`粘贴，在键盘上先后输入`Esc+:wq`保存并退出。

免密登录设置完成。

## 3. 关联本地文件到远程仓库

找到你的`gitea`用户界面，设置->SSH/GPG密钥，删除原来的密钥。

在服务器上的`.ssh`文件夹下输入

```bash
ssh-keygen
```

生成新的密钥，注意，是vscode服务器上，不是C盘那里。

刷新当前文件夹，找到`.pub`文件，全选复制，粘贴到`gitea`设置密钥那里，密钥名称可以不填，没用。

至此，配置完成。
