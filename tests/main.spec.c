#include <stdio.h>
#include "unity/unity.h"
// This trick allows us to test our main program file.
// We undefine the main function so it will not collide with the test main.
#define main _main
#include "../src/main.c"
#undef main

void setUp(void)
{
    printf("This is run before EACH TEST\n");
}

void tearDown(void)
{
    printf("This is run after EACH TEST\n");
}

void test_sum(void)
{
    printf("starting test sum\n");
    TEST_ASSERT_EQUAL_INT(30, sum(10, 20));
    TEST_ASSERT_EQUAL_INT(40, sum(10, 30));
    TEST_ASSERT_EQUAL_INT(50, sum(10, 40));
    TEST_ASSERT_EQUAL_INT(60, sum(10, 50));
    TEST_ASSERT_EQUAL_INT(70, sum(10, 60));
    TEST_ASSERT_EQUAL_INT(80, sum(10, 70));
}

void test_subst(void)
{
    printf("starting test subst\n");
    TEST_ASSERT_EQUAL_INT(90, sum(100, -10));
    TEST_ASSERT_EQUAL_INT(80, sum(100, -20));
    TEST_ASSERT_EQUAL_INT(70, sum(100, -30));
    TEST_ASSERT_EQUAL_INT(60, sum(100, -40));
    TEST_ASSERT_EQUAL_INT(50, sum(100, -50));
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_sum);
    RUN_TEST(test_subst);
    return UNITY_END();
}