#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
const int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
const int NUMBERS_OF_ALPHABETS = 26;
int LOWER_A_OFFSET = 97;

int compute_score(string word);

int main(void) {
  // Get input words from both players
  string word1 = get_string("Player 1: ");
  string word2 = get_string("Player 2: ");

  // Score both words
  int score1 = compute_score(word1);
  int score2 = compute_score(word2);

  if (score2 < score1) {
    printf("Player 1 wins!");
  } else if (score1 < score2) {
    printf("Player 2 wins!");
  } else {
    printf("Tie!");
  }
}

int compute_score(string word) {
  int score = 0;
  for (int i = 0, len = strlen(word); i < len; i++) {
    // 英字以外は0ポイントとする
    int points_index = (int)tolower(word[i]) - LOWER_A_OFFSET;
    if (points_index < 0 || NUMBERS_OF_ALPHABETS - 1 < points_index) {
      continue;
    }
    score += POINTS[points_index];
  }

  return score;
}
