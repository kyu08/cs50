#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct {
  int winner;
  int loser;
} pair;

// Array of candidates
string candidates[MAX];
// 全ての勝敗の組み合わせ
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool is_cycled(int winner, int target_index, bool visited[]);
void print_winner(void);

int main(int argc, string argv[]) {
  // Check for invalid usage
  if (argc < 2) {
    printf("Usage: tideman [candidate ...]\n");
    return 1;
  }

  // Populate array of candidates
  candidate_count = argc - 1;
  if (candidate_count > MAX) {
    printf("Maximum number of candidates is %i\n", MAX);
    return 2;
  }
  for (int i = 0; i < candidate_count; i++) {
    candidates[i] = argv[i + 1];
  }

  // Clear graph of locked in pairs
  for (int i = 0; i < candidate_count; i++) {
    for (int j = 0; j < candidate_count; j++) {
      locked[i][j] = false;
    }
  }

  pair_count = 0;
  int voter_count = get_int("Number of voters: ");

  // Query for votes
  for (int i = 0; i < voter_count; i++) {
    // ranks[i] is voter's ith preference
    // i番目に投票した候補者のindexが入る
    int ranks[candidate_count];

    // Query for each rank
    for (int j = 0; j < candidate_count; j++) {
      string name = get_string("Rank %i: ", j + 1);

      if (!vote(j, name, ranks)) {
        printf("Invalid vote.\n");
        return 3;
      }
    }

    record_preferences(ranks);

    printf("\n");
  }

  add_pairs();
  sort_pairs();
  lock_pairs();
  print_winner();
  return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[]) {
  for (int i = 0; i < candidate_count; i++) {
    if (strcmp(name, candidates[i]) == 0) {
      ranks[rank] = i;
      return true;
    };
  }
  return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[]) {
  for (int i = 0; i < candidate_count; i++) {
    for (int j = i + 1; j < candidate_count; j++) {
      preferences[ranks[i]][ranks[j]]++;
    }
  }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void) {
  for (int i = 0; i < candidate_count; i++) {
    for (int j = i; j < candidate_count; j++) {
      if (preferences[i][j] == preferences[j][i]) {
        continue;
      };

      if (preferences[i][j] > preferences[j][i]) {
        pairs[pair_count].winner = i;
        pairs[pair_count].loser = j;
        pair_count++;
      };

      if (preferences[i][j] < preferences[j][i]) {
        pairs[pair_count].winner = j;
        pairs[pair_count].loser = i;
        pair_count++;
      };
    }
  }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void) {
  // pairsを1つ1つ走査し、winnerとloserの差を計算しながら大きい順でsortする
  for (int i = 0; i < pair_count; i++) {
    int max_index = i;
    int max = preferences[pairs[max_index].winner][pairs[max_index].loser] -
              preferences[pairs[max_index].loser][pairs[max_index].winner];
    for (int j = i + 1; j < pair_count; j++) {
      int winner = pairs[j].winner;
      int loser = pairs[j].loser;
      int diff = preferences[winner][loser] - preferences[loser][winner];
      if (max < diff) {
        max_index = j;
        max = diff;
      }
    }

    if (i == max_index) {
      continue;
    }

    // swap pairs[i] and pairs[max_index]
    pair tmp = pairs[i];
    pairs[i] = pairs[max_index];
    pairs[max_index] = tmp;
  }

  return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void) {
  for (int i = 0; i < pair_count; i++) {
    // pairをlockに追加する
    locked[pairs[i].winner][pairs[i].loser] = true;
    bool visited[MAX] = {false};
    if (is_cycled(pairs[i].winner, pairs[i].winner, visited)) {
      // 追加したことでサイクルが生じるなら削除する
      locked[pairs[i].winner][pairs[i].loser] = false;
    }
  }
  return;
}

bool is_cycled(int winner, int target_index, bool visited[]) {
  if (visited[winner]) {
    return true;
  }
  visited[winner] = true;
  for (int i = 0; i < candidate_count; i++) {
    if (i == winner) {
      continue;
    }

    if (locked[winner][i]) {
      if (i == target_index || is_cycled(i, target_index, visited)) {
        return true;
      }
    }
  }

  return false;
}

// Print the winner of the election
void print_winner(void) {
  // locked[a][b] == trueを満たすbがないaが勝者になる
  for (int i = 0; i < candidate_count; i++) {
    bool winner = true;
    for (int j = 0; j < candidate_count; j++) {
      if (locked[j][i] == true) {
        winner = false;
        break;
      }
    }
    if (winner) {
      printf("%s\n", candidates[i]);
      return;
    }
  }

  return;
}