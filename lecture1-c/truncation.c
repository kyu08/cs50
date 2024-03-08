#include <cs50.h>
#include <stdio.h>

int main(void) {
  int x = get_int("x: ");
  int y = get_int("y: ");

  float z = (float)x / (float)y;

  if (x > y) {
    printf("x is bigger\n");
  } else if (y > x) {
    printf("y is bigger\n");
  } else {
    printf("x and y are equal\n");
  }
}