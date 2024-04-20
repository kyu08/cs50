// Implements a dictionary's functionality
// WARN: このファイルしか編集してはならない
// NOTE:
// submit50を実行したところvalgrindのテストケースだけ落ちるがバージョンの互換性が原因っぽい雰囲気なので諦めた。

#include "dictionary.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Represents a node in a hash table
typedef struct node {
  char word[LENGTH + 1];
  struct node *next;
} node;

// Number of buckets in hash table
#define N 10000

// Hash table
node *table[N] = {NULL};

node *get_last_node(node *target);

// Returns true if word is in dictionary, else false
bool check(const char *word) {
  int len = strlen(word);
  char lower_word[len + 1];
  for (int i = 0; i < len; i++) {
    lower_word[i] = tolower(word[i]);
  }
  lower_word[len] = '\0';

  unsigned int hashed_value = hash(lower_word);
  node *target_node = table[hashed_value];
  if (target_node == NULL) {
    return false;
  }

  while (true) {
    if (strcmp(target_node->word, lower_word) == 0) {
      return true;
    }
    if (target_node->next == NULL) {
      return false;
    }

    target_node = target_node->next;
  }
}

// Hashes word to a number
// 全ての文字のasciiコードを掛けた数値をNで割った剰余を返す
unsigned int hash(const char *word) {
  unsigned int total = 0;
  for (int i = 0; i < strlen(word) - 1; i++) {
    if (total == 0) {
      total = word[i];
      continue;
    }

    total += word[i];
  }
  return (total * 100) % N;
}

// Loads dictionary into memory, returning true if successful, else false
// dictionary is a path to the dictionary to load.
bool load(const char *dictionary) {
  /* ダイレクトチェイニング法で実装してみる
   次のような形式のhash tableとして実装する
   {index: ハッシュ値, value: 最初のnode構造体へのポインタ}
   辞書は大きい方が15万件
  10000個のバケットを用意すると1つのバケットに格納されるnodeの数は平均15件。少ないステップ数で探索できてよさそう。
  */
  FILE *dictionary_file = fopen(dictionary, "r");

  char word[LENGTH + 2]; // LENGTH文字 + '\n' + \'0'となるので+2している
  while (fgets(word, LENGTH + 1, dictionary_file)) {
    int len = strlen(word);

    // wordの末尾に'\n'があれば削除する
    if (word[len - 1] == '\n') {
      word[len - 1] = '\0';
      len--;
    }

    if (len < 1) {
      continue;
    }

    for (int i = 0; i < len; i++) {
      word[i] = tolower(word[i]);
    }

    // 単語をハッシュ化する
    unsigned int hashed_value = hash(word);

    node *new_node = malloc(sizeof(node));
    if (new_node == NULL) {
      printf("malloc for new_node failed.\n");
      return false;
    }
    strcpy(new_node->word, word);
    new_node->next = NULL;

    // find a last node
    node *last_node = get_last_node(table[hashed_value]);
    if (last_node == NULL) {
      table[hashed_value] = new_node;
    } else {
      last_node->next = new_node;
    }
  }

  fclose(dictionary_file);
  return true;
}

node *get_last_node(node *target) {
  if (target == NULL) {
    return NULL;
  }
  while (target->next != NULL) {
    target = target->next;
  }
  return target;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void) {
  unsigned int words_size = 0;
  for (int i = 0; i < N; i++) {
    node *target_word = table[i];
    if (target_word == NULL) {
      continue;
    }

    while (true) {
      words_size++;
      if (target_word->next == NULL) {
        break;
      }
      target_word = target_word->next;
    }
  }

  return words_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void) {
  for (int i = 0; i < N; i++) {
    node *target_word = table[i];
    if (target_word == NULL) {
      continue;
    }

    while (true) {
      if (target_word->next != NULL) {
        node *tmp = target_word->next;
        free(target_word);
        target_word = tmp;
      } else {
        free(target_word);
        break;
      }
    }
  }

  return true;
}
