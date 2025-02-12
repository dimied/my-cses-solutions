#include <stdio.h>
#include <stdlib.h>

#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

// 110111100000101101101011001110100111011001000000000000000000
#define MAX_VALUE (1000000L * 1000000L * 1000000L)

#define MAX_NUM_PRIMES 20
typedef long long ll;

ll primes[MAX_NUM_PRIMES];
ll counts[MAX_NUM_PRIMES];
//ll sums[MAX_NUM_PRIMES * MAX_NUM_PRIMES * MAX_NUM_PRIMES];

ll divLongInts(ll N, ll a, ll b)
{
    if (a < 0 || b < 0 || a > N || b > N)
    {
        return 0;
    }
    if ((N + a) / a < b)
    {
        return 0;
    }
    ll prod = a * b;
    if (prod < 0)
    {
        return 0;
    }
    return N / prod;
}

int cmp(const void *pA, const void *pB)
{
    long long int diff = *((ll *)pA) - *((ll *)pB);
    if (diff > 0)
    {
        return 1;
    }
    if (diff < 0)
    {
        return -1;
    }
    return 0;
}

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

int checkAndAddToSums(int N, ll prod, int i, int numPrimes, int sign, int j)
{
    return j;
}

int main()
{
    int numPrimes, j = 0;
    long long int N, value;

    if (2 != scanf("%lld %d", &N, &numPrimes))
    {
        return 1;
    }
    if (N < 2)
    {
        printf("0\n");
        return 0;
    }

    long long int count = 0;

    // collect valid ones
    for (int i = 0; i < numPrimes; i++)
    {
        if (1 != scanf("%lld", &value))
        {
            return 1;
        }

        if (value > N)
        {
            continue;
        }
        ll divResult = N / value;
        if (divResult > 1)
        {
            primes[j] = value;
            ++j;
        }
        else
        {
            count += divResult;
#if LOCAL_DEV_ENV
            fprintf(stderr, "Skip: %lld\n", value);
#endif
        }
    }
    if (j < 2)
    {
        if (j == 1)
        {
            count += N / primes[0];
        }
        printf("%lld\n", count);
        return 0;
    }
    numPrimes = j;
    qsort(primes, numPrimes, sizeof(long long int), reverseCmp);

#if 0
// long long int lowestPrime = 2; // primes[numPrimes - 1];
    j = 0;
    // Sums of N/prime
    for (int i = 0; i < numPrimes; i++)
    {
        long long int c = N / primes[i];
        if (c >= lowestPrime)
        {
            counts[j] = c;
            primes[j] = primes[i];
            sums[j] = c;
            ++j;
        }
        else
        {
            ++count;
        }
    }
    numPrimes = j;
    if (j < 2)
    {
        count += j * counts[0];
        printf("%lld\n", count);
        return 0;
    }
    
    ll products[MAX_NUM_PRIMES * MAX_NUM_PRIMES];
    int pSize = 0;
    // Sums of N/(prime * another prime)
    for (int i = 0; i < numPrimes - 1; i++)
    {
        ll a = primes[i];
        pSize = 0;
        for (int k = i + 1; k < numPrimes; k++)
        {
            ll res = divLongInts(N, a, primes[k]);
            if (res > 0)
            {
                sums[j] = -res;
                ++j;
                products[pSize] = a * primes[k];
                ++pSize;
            }
        }
    }

    qsort(sums, j, sizeof(long long int), cmp);
#endif
    // Now check N / (product of primes)
    const int limit = 0x1 << numPrimes;
    int lastInvalid = 0;
    for (int bits = 1; bits < limit; bits++)
    {
        int todo = bits, pIdx = 0, numBits = 0;
        ll divisor = 1, doDivide = 1;

        if (lastInvalid > 0 && (bits & lastInvalid) == lastInvalid)
        {
            continue;
        }

        while (todo > 0)
        {
            if (todo & 0x1)
            {
                ++numBits;
                if (divisor == 1)
                {
                    divisor = primes[pIdx];
                }
                else if (doDivide > 0)
                {
                    doDivide = divLongInts(N, divisor, primes[pIdx]);
                    if (doDivide > 0)
                    {
                        divisor *= primes[pIdx];
                    } else {
                        break;
                    }
                }
            }
            todo >>= 1;
            ++pIdx;
        }
        if (doDivide > 0)
        {
            count += (numBits % 2 == 1) ? N / divisor : -N / divisor;
        }
        else
        {
            lastInvalid = bits;
        }
    }

    printf("%lld\n", count);

    return 0;
}