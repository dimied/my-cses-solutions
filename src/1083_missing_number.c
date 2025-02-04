#include <stdio.h>

int main()
{
    long long int N;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%lld", &N);

    long long int currentSum = 0, sumOfAll = (N * (N + 1)) / 2;
    int n;
    --N;

    while (N > 0)
    {
        scanf("%d", &n);
        currentSum += n;
        --N;
    }
#pragma GCC diagnostic pop

    printf("%lld\n", sumOfAll - currentSum);
    return 0;
}