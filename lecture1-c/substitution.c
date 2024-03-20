#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int NUMBERS_OF_ALPHABETS = 26;
int LOWER_A_OFFSET = 97;

int compute_score(string word);
int validate_key(string key);
string get_ciphertext(string substitution, string plain_text);

int main(int argc, string argv[]) {
  if (argc != 2) {
    printf("Usage: ./substitution key");
    return 1;
  }

  string key = argv[1];
  if (validate_key(key) == 1) {
    return 1;
  }

  string plaintext = get_string("plaintext: ");

  string ciphertext = get_ciphertext(key, plaintext);
  printf("ciphertext: %s\n", ciphertext);
  free(ciphertext);
  return 0;
}

int validate_key(string key) {
  if (strlen(key) != 26) {
    printf("Key must contain 26 characters.");
    return 1;
  }

  // detect invalid character
  for (int i = 0; i < strlen(key); i++) {
    if (!isalpha(key[i])) {
      return 1;
    }
  }

  // detect duplicates
  for (int i = 0; i < strlen(key); i++) {
    for (int j = 0; j < strlen(key); j++) {
      if (i == j) {
        continue;
      }
      if (key[i] == key[j]) {
        return 1;
      }
    }
  }

  return 0;
}

string get_ciphertext(string key, string plain_text) {
  char *result = malloc(strlen(plain_text) + 1);
  if (result == NULL) {
    return NULL;
  }

  for (int i = 0; i < strlen(plain_text); i++) {
    char c = plain_text[i];
    if (!isalpha(c)) {
      result[i] = c;
      continue;
    }

    int alphabet_index = (int)tolower(c) - LOWER_A_OFFSET;
    char substituted_c = key[alphabet_index];

    result[i] = islower(c) ? tolower(substituted_c) : toupper(substituted_c);
  }

  result[strlen(plain_text)] = '\0';
  return result;
}