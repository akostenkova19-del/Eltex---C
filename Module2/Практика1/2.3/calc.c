#include <ctype.h>
#include <stdlib.h>
#include "calc.h"

float add(float a, float b)
{
    return a + b;
}

float sub(float a, float b)
{
    return a - b;
}

float mul(float a, float b)
{
    return a * b;
}

float divide(float a, float b)
{
    return a / b;
}

float (*operations[])(float, float) = {
    add, sub, mul, divide
};

int priority(char op)
{
    if (op == '*' || op == '/')
        return 2;

    if (op == '+' || op == '-')
        return 1;

    return 0;
}

int to_rpn(const char *expression, Token rpn[], int *count)
{
    char ops[MAX];
    int top = 0;
    int n = 0;
    char prev = '\0';

    while (*expression) {
        if (isspace((unsigned char)*expression)) {
            expression++;
            continue;
        }

        int negative = (*expression == '-' &&
            (prev == '\0' || prev == '(' ||
             prev == '+' || prev == '-' ||
             prev == '*' || prev == '/'));

        if (isdigit((unsigned char)*expression) ||
            (negative &&
             isdigit((unsigned char)expression[1]))) {

            if (prev == 'n' || prev == ')')
                return -1;

            if (n >= MAX)
                return -1;

            char *end;
            float value = strtod(expression, &end);

            rpn[n++] = (Token){ value, 0, 1 };

            expression = end;
            prev = 'n';
            continue;
        }

        char c = *expression++;

        if (c == '(') {
            if (prev == 'n' || prev == ')')
                return -1;

            if (top >= MAX)
                return -1;

            ops[top++] = c;
        }
        else if (c == ')') {
            if (prev != 'n' && prev != ')')
                return -1;

            while (top > 0 && ops[top - 1] != '(') {
                if (n >= MAX)
                    return -1;

                rpn[n++] = (Token){ 0, ops[--top], 0 };
            }

            if (top == 0)
                return -1;

            top--;
        }
        else if (c == '+' || c == '-' ||
                 c == '*' || c == '/') {

            if (prev != 'n' && prev != ')')
                return -1;

            while (top > 0 && ops[top - 1] != '(' &&
                   priority(ops[top - 1]) >= priority(c)) {
                if (n >= MAX)
                    return -1;

                rpn[n++] = (Token){ 0, ops[--top], 0 };
            }

            if (top >= MAX)
                return -1;

            ops[top++] = c;
        }
        else {
            return -1;
        }

        prev = c;
    }

    if (prev != 'n' && prev != ')')
        return -1;

    while (top > 0) {
        if (ops[top - 1] == '(' || n >= MAX)
            return -1;

        rpn[n++] = (Token){ 0, ops[--top], 0 };
    }

    *count = n;
    return 0;
}

int calculate_rpn(const Token rpn[], int count, float *result)
{
    float stack[MAX];
    int size = 0;

    for (int i = 0; i < count; i++) {
        if (rpn[i].is_number) {
            stack[size++] = rpn[i].value;
        }
        else {
            if (size < 2)
                return -1;

            float b = stack[--size];
            float a = stack[--size];

            int index;
            switch (rpn[i].op) {
                case '+': index = 0; break;
                case '-': index = 1; break;
                case '*': index = 2; break;
                case '/':
                    if (b == 0)
                        return -1;
                    index = 3;
                    break;
                default:
                    return -1;
            }

            float value = operations[index](a, b);
            stack[size++] = value;
        }
    }

    if (size != 1)
        return -1;

    *result = stack[0];
    return 0;
}