#ifndef CALC_H
#define CALC_H

#define MAX 128

typedef struct Token{
    float value;
    char op;
    int is_number;
} Token;

float add(float, float);
float sub(float, float);
float mul(float, float);
float divide(float, float);

int priority(char);

int to_rpn(const char*, Token[], int*);
int calculate_rpn(const Token[], int, float*);

#endif