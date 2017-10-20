# 5回

- 50% デバイスドライバ
- 50% プロセスを作り出す仕組み

## これからやること(予定)

- OSの提供するサービスはどのように提供されているのか
- プロセスはどのようにつくるのか

## 復習

### ファイルシステムの階層

|レイヤ	|+++|
|---	|---|
|低		|デバイスドライバ|
|		|物理ファイルシステム|
|高		|論理ファイルシステム(1つしかない。システムコールを受け取る。)|


### ページング

フラグメンテーションを避けるために、分割して保存する。連続して保存しない。

### メタデータ

ファイルオープンするときにやることは`open()`をすること。

メタデータはデータのためのデータ。

Linuxではメタデータを`inode`と呼ぶ。

`linux-2.6.9/include/linux/fs.h:420`に定義する構造体が存在する。

	  struct inode {
	    struct hlist_node	i_hash;
	    struct list_head	i_list;
	    struct list_head	i_dentry;
	    unsigned long		i_ino;
	    atomic_t		i_count;
	    umode_t			i_mode;
	    unsigned int		i_nlink;
	    uid_t			i_uid;
	    gid_t			i_gid;
	    dev_t			i_rdev;
	    loff_t			i_size;
	    struct timespec		i_atime;
	    struct timespec		i_mtime;
	    struct timespec		i_ctime;
	    unsigned int		i_blkbits;
	    unsigned long		i_blksize;
	    unsigned long		i_version;
	    unsigned long		i_blocks;
	    unsigned short          i_bytes;
	    unsigned char		i_sock;
	    spinlock_t		i_lock;	/* i_blocks, i_bytes, maybe i_size */
	    struct semaphore	i_sem;
	    struct rw_semaphore	i_alloc_sem;
	    struct inode_operations	*i_op;
	    struct file_operations	*i_fop;	/* former ->i_op->default_file_ops */
	    struct super_block	*i_sb;
	    struct file_lock	*i_flock;
	    struct address_space	*i_mapping;
	    struct address_space	i_data;
	  #ifdef CONFIG_QUOTA
	    struct dquot		*i_dquot[MAXQUOTAS];
	  #endif
	    /* These three should probably be a union */
	    struct list_head	i_devices;
	    struct pipe_inode_info	*i_pipe;
	    struct block_device	*i_bdev;
	    struct cdev		*i_cdev;
	    int			i_cindex;
	
	    __u32			i_generation;
	
	    unsigned long		i_dnotify_mask; /* Directory notify events */
	    struct dnotify_struct	*i_dnotify; /* for directory notifications */
	
	    unsigned long		i_state;
	    unsigned long		dirtied_when;	/* jiffies of first dirtying */
	
	    unsigned int		i_flags;
	
	    atomic_t		i_writecount;
	    void			*i_security;
	    union {
	      void		*generic_ip;
	    } u;
	  #ifdef __NEED_I_SIZE_ORDERED
	    seqcount_t		i_size_seqcount;
	  #endif
	  };

## 論理ファイルシステム

キャッシュを使うことでレスポンスを飛躍的にあげている。

### ファイルの名前

メタデータの中にファイル名はない。ファイル名を保管する特別なファイルで保存されている。

ディレクトリファイルが多く存在する。`/usr/bin/yum`の場合、usr, bin, の1つずつにディレクトリファイルが存在する。

### ディレクトリの作り方

一つのディレクトリはひとつの物理ファイルによって作られる。（ディレクトリファイル）

- ディレクトリ一つひとつもファイルである
- すべてのファイル、ディレクトリには一意な番号(inode)が振られている。

何もないまっさらなファイルシステムでも存在するのは`ルートディレクトリ`。

つまり、`/`というディレクトリファイルが存在する。

Macで見てみると以下になる。

    $ ls -i /
        87 Applications/			      5 home/
     75669 Library/			 420193 installer.failurerequests
    105946 Network/			1950048 macOS_SDK/
        54 System/				      3 net/
    420148 Users/				2179841 opt/
    420152 Volumes/			     40 private/
    420153 bin/				 420851 sbin/
    420190 cores/				 420913 tmp@
       301 dev/				 420914 usr/
    420192 etc@				 439682 var@

### リンク

同じファイルに対して別の名前をつけることができる。リンクカウンタでinodeとファイル名の対応付けをしている。リンクカウントが0になるとファイルの実体を消す。

システムコール`unlink()`を発行しているが表向きは`rmコマンド`である。

### キャッシュ

                  |システムコール[read, write]
    User          v
    ------------------------------------------------
    Kernel    +   |キャッシュ     あり 
              +   |------------------>[キャッシュ]  一時記憶上のファイル部分（ページ)
              +   |
      VFS++++++   |
                  vなし
         [物理ファイルシステム]

先読みで常に物理ファイルシステムからキャッシュにいれておく。例えば動画や画像など。CPUで完結する（キャッシュ利用）時とそうでないとき（物理ファイルシステムまでアクセス）の差は10^6くらいになる。つまり、大きなレスポンスの差につながる。

テーブル系のアルゴリズムとして定番の`hash()`関数を使ってページ管理を実現している。

[ハッシュテーブル(Hash Table)を簡単に理解しよう - Qiita](https://qiita.com/tenten1010/items/da4084f937ad07e70164)


