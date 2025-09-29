#include <stdio.h>

long int _factorial_tail(long int n, long int acc) {
    if (n <= 1) return acc;
    return _factorial_tail(n - 1, n * acc);
}

long int factorial_tail(long int n) {
    return _factorial_tail(n, 1);
}


int main() {
    int num = 1000000;
    printf("Tail Recursive Factorial of %d is %ld\n", num, factorial_tail(num));
    return 0;
}