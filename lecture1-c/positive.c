#include <cs50.h>
#include <stdio.h>

int get_positive_int(void);

int main(void) {
  int i = get_positive_int();
  printf("%i\n", i);
}

int get_positive_int(void) {
  int i = -1;
  while (i <= 0) {
    i = get_int("Positive Integer: ");
  };
  return i;
};

// 別解 while使わないのでこっちの方が好き
int get_positive_int2(void) {
  int i = get_int("Positive Integer: ");
  if (i <= 0) {
    return get_positive_int();
  }
  return i;
};