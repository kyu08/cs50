// Simulate genetic inheritance of blood type

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person {
  struct person *parents[2];
  char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void set_allele(person *p, int generations);
void print_family(person *p, int generation);
person *create_parent(int generations);
void free_family(person *p);
char random_allele();

int main(void) {
  // Seed random number generator
  srand(time(0));

  // Create a new family with three generations
  person *p = create_family(GENERATIONS);

  // Print family tree of blood types
  print_family(p, 0);

  // Free memory
  free_family(p);
  return 0;
}

// Create a new individual with `generations`
person *create_family(int generations) {
  // Return value  is like:
  // p(.b.o)
  //  p(.b.o)
  //    p(.a.b)
  //    p(.o.b)
  //  p(.o.o)
  //    p(.a.o)
  //    p(.a.o)
  // Generation with parent data
  if (generations > 1) {
    // Recursively create blood type histories for parents
    person *p = create_parent(generations);

    set_allele(p, generations);

    return p;
  }

  // Generation without parent data
  person *p = create_parent(generations);
  p->parents[0] = NULL;
  p->parents[1] = NULL;

  p->alleles[0] = random_allele();
  p->alleles[1] = random_allele();
  return p;
}

char const INIT_VALUE_CHAR = '0';

person *create_parent(int generations) {
  person *p = malloc(sizeof(person));
  if (generations > 1) {
    p->alleles[0] = INIT_VALUE_CHAR;  // 初期値で初期化しておく
    p->alleles[1] = INIT_VALUE_CHAR;
    p->parents[0] = create_parent(generations - 1);
    p->parents[1] = create_parent(generations - 1);
    return p;
  }

  p->parents[0] = NULL;
  p->parents[1] = NULL;
  p->alleles[0] = random_allele();
  p->alleles[1] = random_allele();
  return p;
}

void set_allele(person *p, int generations) {
  if (generations == 1) {  // ここには来ないはず
    return;
  }

  if (p->parents[0]->alleles[0] == INIT_VALUE_CHAR || p->parents[0]->alleles[1] == INIT_VALUE_CHAR) {
    set_allele(p->parents[0], generations - 1);
  }
  if (p->parents[1]->alleles[0] == INIT_VALUE_CHAR || p->parents[1]->alleles[1] == INIT_VALUE_CHAR) {
    set_allele(p->parents[1], generations - 1);
  }

  if ((sizeof(p->parents[0]->alleles) / sizeof(p->parents[0]->alleles[0]) < 2) ||
      sizeof(p->parents[1]->alleles) / sizeof(p->parents[1]->alleles[0]) < 2) {  // ここには来ないはず
    return;
  }

  p->alleles[0] = p->parents[0]->alleles[rand() % 2];
  p->alleles[1] = p->parents[1]->alleles[rand() % 2];
  return;
}

// Free `p` and all ancestors of `p`.
void free_family(person *p) {
  if (p->parents[0] == NULL && p->parents[1] == NULL) {
    free(p);
    return;
  }

  free_family(p->parents[0]);
  free_family(p->parents[1]);
  p->parents[0] = NULL;
  p->parents[1] = NULL;

  free(p);

  return;
}

// Print each family member and their alleles.
void print_family(person *p, int generation) {
  // Handle base case
  if (p == NULL) {
    return;
  }

  // Print indentation
  for (int i = 0; i < generation * INDENT_LENGTH; i++) {
    printf(" ");
  }

  // Print person
  printf("Generation %i, blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
  print_family(p->parents[0], generation + 1);
  print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
char random_allele() {
  int r = rand() % 3;
  if (r == 0) {
    return 'A';
  } else if (r == 1) {
    return 'B';
  } else {
    return 'O';
  }
}
