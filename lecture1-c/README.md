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

## 文字列
文字列を操作する際は終端文字に注意。
この関数のように、最初に値を束縛せずに文字列を初期化した際は終端文字を自分で追加する必要がある。

```c
void print_bricks(int height) {
  char separator[] = "  ";
  for (int i = 0; i < height; i++) {
    char left_bricks[height + 1];  // 終端文字の分で+1している
    char right_bricks[height + 1];

    for (int j = 0; j < height; j++) {
      if ((height - j) <= (i + 1)) {
        // 末尾からi+1番目までを#で埋める。height - j: 末尾から何番目かを示す
        left_bricks[j] = '#';
        // leftの逆
        right_bricks[height - j - 1] = '#';
      } else {
        left_bricks[j] = ' ';
      }
    }

    left_bricks[height] = '\0';   // 終端文字を追加
    right_bricks[height] = '\0';  // 終端文字を追加

    printf("%s%s%s\n", left_bricks, separator, right_bricks);
  }
}
```

## `make hello.c`を実行すると何が起きるか
1. プリプロセス
  - マクロの展開や`#include`や`#ifdef`などのディレクティブの処理が行われる
1. コンパイル
  - アセンブリに変換
1. アセンブル
  - アセンブラソースからオブジェクトファイルを生成
1. リンク
  - 必要なライブラリとの結合を行い実行可能な形式のファイルを生成する

## 配列
C言語の配列は（他の言語と同様に）メモリに格納された連続した値を順に並べたもの