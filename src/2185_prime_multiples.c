#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_PRIMES 20
typedef long long ll;

long long int primes[MAX_NUM_PRIMES];

int reverseCmp(const void *pA, const void *pB)
{
    long long int diff = *((ll *)pA) - *((ll *)pB);
    if (diff > 0)
    {
        return -1;
    }
    if (diff < 0)
    {
        return 1;
    }
    return 0;
}

int main()
{
    int numPrimes;
    long long int N;

    scanf("%lld %d", &N, &numPrimes);
    if (N < 2)
    {
        printf("0\n");
        return 0;
    }
    long long int *p = &primes[0];

    for (int i = 0; i < numPrimes; i++)
    {
        scanf("%lld", p);
        ++p;
    }
    if (numPrimes == 1)
    {
        printf("%lld\n", N / primes[0]);
        return 0;
    }
    long long int count = 0;
    qsort(primes, numPrimes, sizeof(long long int), reverseCmp);
    long long int counts[MAX_NUM_PRIMES];
    counts[0] = 0;

    int j = 0;
    for (int i = 0; i < numPrimes; i++)
    {
        count = N / primes[i];
        if (count > 0)
        {
            counts[j] = count;
            ++j;
        }
    }
    numPrimes = j;
    if (numPrimes < 2)
    {
        printf("%lld\n", counts[0]);
        return 0;
    }

    return 0;
}