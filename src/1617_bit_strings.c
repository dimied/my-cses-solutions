#include <stdio.h>

#define MODULO_VALUE 1000000007

int main()
{
    long long int N;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%lld", &N);
#pragma GCC diagnostic pop

    if (N < 63)
    {
        N = (1 << N) % MODULO_VALUE;
        printf("%lld\n", N);
        return 0;
    }
    const long long int shift30 = 1 << 30;
    long long int count = shift30;
    N -= 30;

    //We know: (a*b) mod n = ((a mod n)*(b mod n)) mod n
    while (N > 30)
    {
        count = (count * shift30) % MODULO_VALUE;
        N -= 30;
    }

    count = (count * (1 << N)) % MODULO_VALUE;

    printf("%lld\n", count);

    return 0;
}