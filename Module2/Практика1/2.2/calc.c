#include "calc.h"

double Execute(float a, float b, char action, int *success) {
    *success = 1; 
    
    switch (action) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                *success = 0; 
                return 0.0;
            }
            return a / b; 
        default: 
            *success = 0; 
            return 0.0;
    }
}