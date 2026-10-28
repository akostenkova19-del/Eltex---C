#include "unity.h"
#include "calc.h"

void setUp(void) {}
void tearDown(void) {}

void test_ToRpn_SimpleExpression(void)
{
    Token rpn[MAX];
    int count = 0;

    int result = to_rpn("2 + 3", rpn, &count);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(3, count);

    TEST_ASSERT_EQUAL_INT(1, rpn[0].is_number);
    TEST_ASSERT_EQUAL_FLOAT(2.0f, rpn[0].value);

    TEST_ASSERT_EQUAL_INT(1, rpn[1].is_number);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, rpn[1].value);

    TEST_ASSERT_EQUAL_INT(0, rpn[2].is_number);
    TEST_ASSERT_EQUAL_CHAR('+', rpn[2].op);
}

void test_ToRpn_Priority(void)
{
    Token rpn[MAX];
    int count = 0;

    int result = to_rpn("2 + 3 * 4", rpn, &count);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(5, count);

    TEST_ASSERT_EQUAL_FLOAT(2.0f, rpn[0].value);
    TEST_ASSERT_EQUAL_FLOAT(3.0f, rpn[1].value);
    TEST_ASSERT_EQUAL_FLOAT(4.0f, rpn[2].value);
    TEST_ASSERT_EQUAL_CHAR('*', rpn[3].op);
    TEST_ASSERT_EQUAL_CHAR('+', rpn[4].op);
}

void test_ToRpn_Brackets(void)
{
    Token rpn[MAX];
    int count = 0;

    int result = to_rpn("(2 + 3) * 4", rpn, &count);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(5, count);

    TEST_ASSERT_EQUAL_CHAR('+', rpn[2].op);
    TEST_ASSERT_EQUAL_CHAR('*', rpn[4].op);
}

void test_ToRpn_NegativeNumber(void)
{
    Token rpn[MAX];
    int count = 0;

    int result = to_rpn("2 * -3", rpn, &count);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(3, count);

    TEST_ASSERT_EQUAL_FLOAT(2.0f, rpn[0].value);
    TEST_ASSERT_EQUAL_FLOAT(-3.0f, rpn[1].value);
    TEST_ASSERT_EQUAL_CHAR('*', rpn[2].op);
}

void test_ToRpn_InvalidExpression(void)
{
    Token rpn[MAX];
    int count = 0;

    TEST_ASSERT_EQUAL_INT(-1, to_rpn("2 + * 3", rpn, &count));
    TEST_ASSERT_EQUAL_INT(-1, to_rpn("(2 + 3", rpn, &count));
    TEST_ASSERT_EQUAL_INT(-1, to_rpn("2 + 3)", rpn, &count));
    TEST_ASSERT_EQUAL_INT(-1, to_rpn("2 3", rpn, &count));
}

void test_CalculateRpn_Addition(void)
{
    Token rpn[MAX];
    int count = 0;
    float result = 0.0f;

    TEST_ASSERT_EQUAL_INT(0, to_rpn("2 + 3", rpn, &count));
    TEST_ASSERT_EQUAL_INT(0, calculate_rpn(rpn, count, &result));

    TEST_ASSERT_EQUAL_FLOAT(5.0f, result);
}

void test_CalculateRpn_Priority(void)
{
    Token rpn[MAX];
    int count = 0;
    float result = 0.0f;

    TEST_ASSERT_EQUAL_INT(0, to_rpn("2 + 3 * 4", rpn, &count));
    TEST_ASSERT_EQUAL_INT(0, calculate_rpn(rpn, count, &result));

    TEST_ASSERT_EQUAL_FLOAT(14.0f, result);
}

void test_CalculateRpn_DivisionByZero(void)
{
    Token rpn[MAX];
    int count = 0;
    float result = 0.0f;

    TEST_ASSERT_EQUAL_INT(0, to_rpn("5 / 0", rpn, &count));
    TEST_ASSERT_EQUAL_INT(-1, calculate_rpn(rpn, count, &result));
}

void test_CalculateRpn_FractionalNumbers(void)
{
    Token rpn[MAX];
    int count = 0;
    float result = 0.0f;

    TEST_ASSERT_EQUAL_INT(0, to_rpn("0.5 + 1.25", rpn, &count));
    TEST_ASSERT_EQUAL_INT(0, calculate_rpn(rpn, count, &result));

    TEST_ASSERT_EQUAL_FLOAT(1.75f, result);
}

void test_CalculateRpn_NegativeResult(void)
{
    Token rpn[MAX];
    int count = 0;
    float result = 0.0f;

    TEST_ASSERT_EQUAL_INT(0, to_rpn("3 - 8", rpn, &count));
    TEST_ASSERT_EQUAL_INT(0, calculate_rpn(rpn, count, &result));

    TEST_ASSERT_EQUAL_FLOAT(-5.0f, result);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_ToRpn_SimpleExpression);
    RUN_TEST(test_ToRpn_Priority);
    RUN_TEST(test_ToRpn_Brackets);
    RUN_TEST(test_ToRpn_NegativeNumber);
    RUN_TEST(test_ToRpn_InvalidExpression);
    RUN_TEST(test_CalculateRpn_Addition);
    RUN_TEST(test_CalculateRpn_Priority);
    RUN_TEST(test_CalculateRpn_DivisionByZero);
    RUN_TEST(test_CalculateRpn_FractionalNumbers);
    RUN_TEST(test_CalculateRpn_NegativeResult);

    return UNITY_END();
}
