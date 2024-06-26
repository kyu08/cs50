#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <CUnit/TestDB.h>
#include <string.h>

#include "CUnit/Basic.h"
#include "fizzbuzz.h"

// https://qiita.com/Hiroya_W/items/dd2e682718fac5b44db2 を参考にした。
void fizzbuzz_test_1(void);
void fizzbuzz_test_3(void);
void fizzbuzz_test_5(void);
void fizzbuzz_test_15(void);

int main(void) {
  CU_pSuite suite;
  CU_initialize_registry();

  suite = CU_add_suite("FizzBuzz Test", NULL, NULL);
  CU_add_test(suite, "FizzBuzz_Test_1", fizzbuzz_test_1);
  CU_add_test(suite, "FizzBuzz_Test_3", fizzbuzz_test_3);
  CU_add_test(suite, "FizzBuzz_Test_5", fizzbuzz_test_5);
  CU_add_test(suite, "FizzBuzz_Test_15", fizzbuzz_test_15);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}

void fizzbuzz_test_1(void) {
  char result[256];
  memset(result, '\0', sizeof(result));

  fizzbuzz(1, result);

  CU_ASSERT_STRING_EQUAL("1", result);
  return;
}

void fizzbuzz_test_3(void) {
  char result[256];
  memset(result, '\0', sizeof(result));

  fizzbuzz(3, result);

  CU_ASSERT_STRING_EQUAL("Fizz", result);
  return;
}

void fizzbuzz_test_5(void) {
  char result[256];
  memset(result, '\0', sizeof(result));

  fizzbuzz(5, result);

  CU_ASSERT_STRING_EQUAL("Buzz", result);
  printf("\nresult: %s\n", result);
  return;
}

void fizzbuzz_test_15(void) {
  char result[256];
  memset(result, '\0', sizeof(result));

  fizzbuzz(15, result);

  CU_ASSERT_STRING_EQUAL("FizzBuzz", result);
  return;
}
