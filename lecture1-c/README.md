https://www.youtube.com/watch?v=JBu-F9S7cQo

- ソースコードをコンミュータが理解できるマシンコードに変換するためにコンパイラを用いてコンパイルを行う必要がある。
- `make hello`でコンパイルすることができる

> Ｃ言語には文字列型という型はない。文字列は，char 型の配列で表される。ただし，単なる配列ではなく，「文字列の最後には終端を表す文字 '\0'（ヌル文字）がついている」という約束の下に取り扱われる文字配列である。文字列の最後に付けられたヌル文字を終端文字と呼ぶ。
> https://www.cc.kyoto-su.ac.jp/~yamada/programming/string.html

## プロトタイプ宣言
c言語では関数は呼び出す前に宣言されている必要がある。しかし慣習として可読性のためにmainのようなエントリポイントとなる関数は可能な限り先頭に近い位置に記述したい。

そこで回避策としてプロトタイプ宣言（L5）を行うことでmainをできる限り先頭に近い位置に記述しつつ、それ以外の関数定義をmain関数の後ろに移動することができる。

```c
#include <stdio.h>

// Prototype
void meow(void);

int main(void) {
  int count = 0;
  int max = 10;

  while (count < max) {
    meow();
    count++;
  };
}

void meow(void) { printf("meow\n"); }
```
