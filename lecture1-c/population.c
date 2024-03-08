#include <cs50.h>
#include <stdio.h>

int get_start_size(int);
int get_end_size(int);
int how_many_years_takes_to_end_size(int, int);

// 開始サイズと終了サイズが与えられ、何年でそのサイズになるかを標準出力に出力する
// 要件:
// - 開始サイズは9以上
// - 終了サイズは開始サイズ以上である
// - 1年後に1/3が生まれ、1/4が死ぬ
int main(void) {
  // 入力を受け取る
  int start_size = get_start_size(9);
  int end_size = get_end_size(start_size);

  // 1年後の人口を計算
  int result = how_many_years_takes_to_end_size(start_size, end_size);

  // 結果を出力
  printf("Years: %i\n", result);
}

int get_start_size(int minimum) {
  int size = minimum - 1;
  while (size < minimum) {
    size = get_int("Start size: ");
  }
  return size;
};

int get_end_size(int minimum) {
  int size = minimum - 1;
  while (size < minimum) {
    size = get_int("End size: ");
  }
  return size;
};

// 終了サイズになるまでに何年かかるか
int how_many_years_takes_to_end_size(int start_size, int end_size) {
  if (end_size < start_size) {
    return 0;
  }

  int how_many_years = 0;
  int one_year_later = start_size;

  while (one_year_later < end_size) {
    one_year_later += one_year_later / 3 - one_year_later / 4;
    how_many_years++;
  }

  // もし終了サイズを上回ったら終了。
  return how_many_years;
}