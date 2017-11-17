## 8回

サンプルソースを実行する

	$ gcc execlp-test.c
	$ ./a.out
	test exec: start
	a.out  execlp-test.c

execlpがプログラムを乗っ取るので元のプログラムに帰ってこない。

実行中にプロンプトが返ることがある。案外、順番は意識されていない。

	fork test0
	fork test1
	fork test1
	fork test2
	tako@tako-virtual-machine:~/OS-design/class08$ fork test2
	fork test2
	fork test2

メモリ空間が32bitあると2**32=4Gバイトのメモリ空間が存在する。

	#include <stdio.h>
	int g;
	int main(){
	    int l;
	    printf("g=%x, l=%x, main=%x\n", &g, &l, main);
	}

実行すると以下

	> ./a.out 
	g=601044, l=8ebc31e4, main=400596

この結果からグローバル変数とmain関数は手前の部分に位置している。

OSは4GBのメモリの使用には許可を出す。ローカル変数は下の方（後ろのほう）に割り当てる。

ローカル変数は動かすたびにアドレスが変化する。

ローカル変数はメモリの後ろからスタックされていく。

	0 -------------
	|
	|-------------- 4M
	| main
	|-------------- 6M
	| g
	|
	| 
	|
	| 
	| l			スタック
	4G -------------

`fork()`をすると上記の空間を複製する。これが繰り返されるとシステム全体がメモリ不足になって止まる。

プログラムがメモリにアクセスする時にはMMUを経由してアクセスをする。つまり直接、メモリにアクセスすることはしない。

ページング(4KB)によるメモリ管理機構を搭載。

### メモリ管理

それぞれのプロセスにどれくらいメモリを割り当てるか管理。メモリ使ってない場合はメモリを取り上げる（開放）。メモリがほしい場合は割り当てる。

### プロセス実行

1つのプロセッサでどうやって複数のプロセスを同時に実行できるのか。

**=> プロセス切り替え(process switch)により実現**

MMUと一時記憶（レジスタ）にもすこし保管する部分がある。

**レジスタの内容とMMUの内容を保存しておけば後でまた実行を再開出来る**

-> 保存先はメモリ上

#### 入出力の待ち合わせ

キャッシュが無いとHDDに対してI/Oをする必要がある。そうすると、時間かかる。OSは命令をしたまま別の処理を行う。


