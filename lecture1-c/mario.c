#include <cs50.h>
#include <stdio.h>

int get_height();
void print_bricks(int);

int main(void) {
  // 高さを1~8で受け取る
  int height = get_height();

  // レンガを描画する
  print_bricks(height);
}

int get_height() {
  int height = 0;
  while (height < 1 || 8 < height) height = get_int("Height: ");
  return height;
}

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