#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int get_number_of_digits(long number) {
  long abs_number = labs(number);  // 負の数を考慮するために絶対値をとる
  if (abs_number == 0) {
    return 1;
  }

  int number_of_digits = 0;
  while (!(abs_number < 1)) {  // 1を下回ったら終了
    abs_number /= 10;
    number_of_digits++;
  }
  return number_of_digits;
}
int starts_with(long target, int number) {
  // 12
  int digit_number = get_number_of_digits(number);
  // 12345
  int target_digit_number = get_number_of_digits(target);
  int head = target / pow(10, (target_digit_number - digit_number));

  // 先頭からdigit_number桁がnumberと合致するか見たい。
  return head == number;
}

int main(void) {
  printf("\n1 == (12345, 12)%i", starts_with(12345, 12));
  printf("\n1 ==(5234128746, 5)%i", starts_with(5234128746, 5));
  printf("\n0 ==(123, 4)%i", starts_with(123, 4));
}
