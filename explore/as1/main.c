#include <stdio.h>
#include <stdlib.h>

// gcd is an external function with two parameters
// It takes two signed 64-bit integers and returns a signed 64-bit integer
extern long gcd(long a, long b);

int main() {
    long num1 = 48;
    long num2 = 18;
    
    // Call the x86-64 assembly function
    long result = gcd(num1, num2);
    
    printf("The GCD of %ld and %ld is: %ld\n", num1, num2, result);
    
    return 0;
}
