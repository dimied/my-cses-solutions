#include <stdio.h>
#include <stdlib.h>

int main()
{
    long long int N;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%lld", &N);
#pragma GCC diagnostic pop

    if (N > 1)
    {
        printf("%lld", N);
        N = (N % 2 == 0) ? (N / 2) : (3 * N + 1);
    }
    while (N > 1)
    {
        printf(" %lld", N);
        N = (N % 2 == 0) ? (N / 2) : (3 * N + 1);
    }
    printf(" 1\n");
    return 0;
}