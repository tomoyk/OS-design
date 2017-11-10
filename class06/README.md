# 6回

## OSSプロジェクト

- 元祖OSSは`GNU`
- Linux `kernel.org`
- Android `Googleの意向が強い`
- Apache `tomcat, httpServer, openMeetings...`
- etc

## vmlinuxファイル

Linuxの主要部分の機械命令がすべて収められているひとつのファイル（モジュール）。

vmlinuxとinitrdの内容をあわせてbzImageが作られる。

## スイッチを入れた時

BIOSがHDDの特定領域をメモリ上に展開する。

## Linuxカーネルを構成するファイル

vmlinux -> スタティックリンクされた中核部分。

**Dynamic Link**

必要なものにマークをつけて必要に応じてあとからプログラム呼び出す。

## カーネルモジュール

必要に応じてカーネルに読み込まれる。

デバイスドライバ、プロトコル、ファイルシステム

`modprobeコマンド`かな?

## ソースを覗いてみる

	tako@tako-virtual-machine:~/デスクトップ/linux-4.13.9$ ls
	COPYING  Documentation  Kconfig      Makefile  arch   certs   drivers   fs       init  kernel  mm   samples  security  tools  virt
	CREDITS  Kbuild         MAINTAINERS  README    block  crypto  firmware  include  ipc   lib     net  scripts  sound     usr

- arch アーキテクチャ
- fs ファイルシステム
- include ヘッダファイル郡
- init 初期化時(起動する時)
- kernel OSカーネルの中核部分のコード
- net 通信系TCP/IPなど

## memo

main関数だけ探してみた。

	tako@tako-virtual-machine:~/デスクトップ/linux-4.13.9$ find . | grep /main.c | grep -v /drivers
	./net/mac80211/main.c
	./net/mac802154/main.c
	./net/batman-adv/main.c
	./fs/fscache/main.c
	./fs/cachefiles/main.c
	./fs/ecryptfs/main.c
	./fs/afs/main.c
	./fs/dlm/main.c
	./fs/gfs2/main.c
	./kernel/power/main.c
	./tools/testing/radix-tree/main.c
	./tools/power/cpupower/bench/main.c
	./tools/virtio/ringtest/main.c
	./arch/x86/boot/main.c
	./arch/x86/kernel/cpu/mtrr/main.c
	./arch/alpha/boot/main.c
	./arch/um/os-Linux/main.c
	./arch/powerpc/boot/main.c
	./init/main.c

---

コンパイル時に表示される`  CC [M]  drivers/gpu/drm/nouveau/nvkm/subdev/bios/mxm.o`の`[M]`はモジュールを表す。コンパイルしてリンクすることで一つのプログラムになる。

`bc`と`カーセス`を入れればビルドが進む。

## ソースコードを理解する

Linuxのソースコードからみる切り口

- 全体構造から理解
	- システムコールを処理する関数
	- プロセスを実現する関数
- 実際の動作を理解
	- システムコールが発生した時
	- 計算機の電源を入れた時の挙動

## ファイルシステム

一番簡単ななファイルシステムは`ramfs`

### VFS

- システムコールを受け付ける
- ファイル名の処理を行う
- etc

システムコールを発行するプログラム -> VFS

**あとはソースを読んで見る**

### fs/*

- fs/*.c 論理ファイルシステムが配置されている
- fs/\*/*.c 物理ファイルシステムが配置されている

### fs/read_write.c:542

`read()`の実装
	
	SYSCALL_DEFINE3(read, unsigned int, fd, char __user *, buf, size_t, count)
	{
	  struct fd f = fdget_pos(fd);
	  ssize_t ret = -EBADF;
	
	  if (f.file) {
	    loff_t pos = file_pos_read(f.file);
	    ret = vfs_read(f.file, buf, count, &pos);
	    if (ret >= 0)
	      file_pos_write(f.file, pos);
	    fdput_pos(f);
	  }
	  return ret;
	}

## システムコールの処理

アプリケーション・プログラム--(プログラム)-->[system_call]-->sysread(), syswrite()

## ファイルディスクリプタ

オープンされているファイルの状態に関する情報が保存されている。

- IOの現在位置
- etc

`vfs_read()`を見つけてみる.

	koya-mac:NormalLinuxProgramming tkoyama$ grep -R "vfs_read(" *
	exec.c:	result = vfs_read(file, (void __user *)addr, count, &pos);
	exec.c:	ssize_t res = vfs_read(file, (void __user *)addr, len, &pos);
	read_write.c:ssize_t __vfs_read(struct file *file, char __user *buf, size_t count,
	read_write.c:ssize_t vfs_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
	read_write.c:		ret = __vfs_read(file, buf, count, pos);
	read_write.c:		ret = vfs_read(f.file, buf, count, &pos);
	read_write.c:			ret = vfs_read(f.file, buf, count, &pos);
	
### ramfs()

簡単なので読んでみるとよい