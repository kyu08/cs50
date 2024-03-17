#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/*
https://cs50.jp/x/2021/week2/problem-set/readability/
- 文字:[a-zA-Z]
- 作成されたインデックス番号が16以上 (大学4年生以上) の場合は"Grade
16+"と出力され、正確なインデックス番号は出力されません。インデックス番号が1より小さい場合、プログラムは "Before Grade 1"
を出力します。
- プログラムは、テキスト内の文字、単語、および文の数をカウントする必要があります。
  -
文字は、aからzまでの任意の小文字またはAからZまでの任意の大文字であり、スペースで区切られた任意の文字列は単語としてカウントされ、ピリオド、感嘆符、または疑問符が出現すると文の終わりを示します。
- index = 0.0588 * L - 0.296 * S - 15.8
- ここで、L は100語あたりの平均字数、Sは100語あたりの平均文数です。
*/

const int MAX_GRADE = 16;
void print_grade(int score);
int calc_score(string input);
int count_sentences(string input);
int count_words(string input);
int count_chars(string input);

int main(void) {
  string input = get_string("Text: ");
  int score = calc_score(input);
  print_grade(score);
}

void print_grade(int score) {
  if (score < 1) {
    printf("Before Grade 1\n");
    return;
  } else if (MAX_GRADE <= score) {
    printf("Grade 16+\n");
    return;
  }
  printf("Grade %i\n", score);
}

// 可読性を重視して数えたい各関数内でループを回しているがパフォーマンス的にはもちろんよくない(がパフォーマンスを追求する必要がないのでそうしていない)
int calc_score(string input) {
  int sentence_count = count_sentences(input);
  int word_count = count_words(input);
  int char_count = count_chars(input);

  float average_char_count_per_100_words = ((float)100 / (float)word_count) * char_count;
  float average_char_sentence_per_100_words = ((float)100.0 / (float)word_count) * sentence_count;

  return round(0.0588 * average_char_count_per_100_words - 0.296 * average_char_sentence_per_100_words - 15.8);
}

int count_sentences(string input) {
  int count = 0;

  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == '.' || input[i] == '!' || input[i] == '?') {
      count++;
    }
  }

  return count;
}

int count_words(string input) {
  int count = 1;  // スペースを数えるだけだと文末の単語をカウントできないので雑だが1スタートにする。
  // ちゃんとやるならinputの先頭と末尾のspaceをtrimしたり連続したスペースをtrimするとよさそう。

  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == ' ') {
      count++;
    }
  }
  return count;
}

int count_chars(string input) {
  int count = 0;
  for (int i = 0; i < strlen(input); i++) {
    if (isalpha(input[i])) {
      count++;
    }
  }
  return count;
}
