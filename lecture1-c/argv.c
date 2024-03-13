#include <cs50.h>
#include <stdio.h>

// argc: argument count
// argv: argument vector コマンドライン引数だけでなく自身のプログラム名を含むすべての単語がargvに格納される。
// たとえば ./argv taro としたら argv[0]: ./argv, argv[1]: taro となる。
int main(int argc, string argv[]) {
  if (argc == 2) {
    printf("hello %s %s!\n", argv[0], argv[1]);
  } else {
    printf("hi!\n");
  }
}