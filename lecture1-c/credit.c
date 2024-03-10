#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
要件
- クレジットカードの番号が正しいか検証する
- 検証手順
    1. その数字の最後から2番目の数字から始め、1桁おきに2を掛け、それらの数を合計する
    2. 2.で乗算されなかった桁の数と1.を合計する
    3. 2.の最後の桁が0の場合、その番号は有効である
- 桁数
    - AMEX: 15
    - MasterCard: 16
    - Visa: 13 or 16
- 先頭の数字
    - AMEX: 34 or 37
    - MasterCard: 51 or 52 or 53 or 54 or 55
    - Visa: 4
*/

const int AMEX_DIGIT_NUMBER = 15;
const int MASTER_CARD_DIGIT_NUMBER = 16;
const int VISA_DIGIT_NUMBER0 = 13;
const int VISA_DIGIT_NUMBER1 = 16;

int get_number_of_digits(long number);
int is_valid_credit_card_number(long card_number);
int get_digit(long number, int position);
int starts_with(long target, int number);
int is_amex(long credit_card_number);
int is_master_card(long credit_card_number);
int is_visa(long credit_card_number);

int main(void) {
  long credit_card_number = get_long("Number: ");

  // カードの桁数を検証
  int number_of_digits = get_number_of_digits(credit_card_number);
  if (number_of_digits != 13 && number_of_digits != 15 && number_of_digits != 16) {
    printf("INVALID\n");
    return 0;
  }

  // カード番号の検証
  if (!is_valid_credit_card_number(credit_card_number)) {
    printf("INVALID\n");
    return 0;
  }

  // カード番号からissuerを判定
  if (is_amex(credit_card_number)) {
    printf("AMEX\n");
  } else if (is_master_card(credit_card_number)) {
    printf("MASTERCARD\n");
  } else if (is_visa(credit_card_number)) {
    printf("VISA\n");
  } else {
    printf("INVALID\n");
    return 0;
  }
}

// get_number_of_digits 渡された数値の桁数を返す
// n回10で割ったときに0より小さくなるnを返す
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

int is_valid_credit_card_number(long card_number) {
  int sum = 0;
  int number_of_digit = get_number_of_digits(card_number);
  for (int i = 1; i <= number_of_digit; i++) {
    int digit = get_digit(card_number, i);

    if (i % 2 == 0) {  // i == 2nならばその数に2をかけてsumにincrementしていく
      int doubled = 2 * digit;
      if (get_number_of_digits(doubled) == 2) {  // 2桁だったら分解してそれぞれ足す
        sum += get_digit(doubled, 2) + get_digit(doubled, 1);
        continue;
      }
      sum += doubled;
    } else {  // i == 2n + 1ならばその数をsumにincrementしていく
      sum += digit;
    }
  }

  // sumの最後の桁が0かどうかを返す
  return get_digit(sum, 1) == 0;
}

// n桁目の数字を返す
// i.g., number = 12345, position = 4のとき2が返る
int get_digit(long number, int position) {
  // 12345を123にして
  long temp = number / (pow(10, (position - 1)));

  // 10で割った剰余が求めたい値になる
  return temp % 10;
}

// starts_with targetの先頭がnumberで始まるかを返す
int starts_with(long target, int number) {
  int target_digit_number = get_number_of_digits(target);
  int digit_number = get_number_of_digits(number);
  int head = target / pow(10, (target_digit_number - digit_number));

  // 先頭からdigit_number桁がnumberと合致するか見たい。
  return head == number;
}

int is_amex(long credit_card_number) {
  return get_number_of_digits(credit_card_number) == AMEX_DIGIT_NUMBER &&
         (starts_with(credit_card_number, 34) || starts_with(credit_card_number, 37));
}

int is_master_card(long credit_card_number) {
  return get_number_of_digits(credit_card_number) == MASTER_CARD_DIGIT_NUMBER &&
         (starts_with(credit_card_number, 51) || starts_with(credit_card_number, 52) ||
          starts_with(credit_card_number, 53) || starts_with(credit_card_number, 54) ||
          starts_with(credit_card_number, 55));
}

int is_visa(long credit_card_number) {
  return (get_number_of_digits(credit_card_number) == VISA_DIGIT_NUMBER0 ||
          (get_number_of_digits(credit_card_number) == VISA_DIGIT_NUMBER1)) &&
         starts_with(credit_card_number, 4);
}
