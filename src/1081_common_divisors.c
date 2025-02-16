#include <stdio.h>
#include <stdlib.h>

#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

#if LOCAL_DEV_ENV
#include <sys/time.h>
#endif

#if LOCAL_DEV_ENV
#include <stdarg.h>

double getTime(struct timeval *startTimeval, struct timeval *endTimeval)
{
    long seconds = endTimeval->tv_sec - startTimeval->tv_sec;
    long microseconds = endTimeval->tv_usec - startTimeval->tv_usec;
    return (double)seconds + (double)microseconds * 0.000001f;
}
void storeTime(struct timeval *pTimeval)
{
    gettimeofday(pTimeval, 0);
}

#if 0
void logError(const char *pStr, ...)
{
    va_list args;
    va_start(args, pStr);
    fprintf(stderr, pStr, args);
    va_end(args);
}
#endif

#define logError(...) fprintf(stderr, __VA_ARGS__)
#endif

#define MAX_X 1000000
#define MAX_N 200000

typedef long long int ll;
typedef unsigned long long int ull;

int numbers[MAX_N];

int d[MAX_X + 1] = {0};

#define DUPLICATE_SIZE ((MAX_X + 63) / 64) + 1
// Bit set if the value recognized
// Later cleared and used to remember whether the value is finally computed
ull duplicateOrComputedBits[DUPLICATE_SIZE] = {
    0,
};

int cmpDesc(const void *pA, const void *pB)
{
    long long int diff = *((int *)pA) - *((int *)pB);
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
int gcd(int a, int b)
{
    if (b == 0)
    {
        return a;
    }

    return gcd(b, a % b);
}

#define GCD_LIMIT 1000

#define PRIME_LISTS 2 * 3 * 5
#define PRIME_BITS2_1 ((0x1 << 2) | (0x1 << 4) | (0x1 << 6) | (0x1 << 8) | (0x1 << 10))
#define PRIME_BITS2_2 ((0x1 << 12) | (0x1 << 14) | (0x1 << 16) | (0x1 << 18) | (0x1 << 20))
#define PRIME_BITS2_3 ((0x1 << 22) | (0x1 << 24) | (0x1 << 26) | (0x1 << 28) | (0x1 << 30))
#define PRIME_BITS ((0x1) | PRIME_BITS2_1 | PRIME_BITS2_2 | PRIME_BITS2_3 | (0x1 << 3) | (0x1 << 9) | (0x1 << 5) | (0x1 << 15) | (0x1 << 30))

#define PRIME_MASK(SIZE, V1, V2, V3) (SIZE | (V1 << 2) | (V2 << 5) | (V2 << 8))
//
int primeLists[PRIME_LISTS][MAX_N];
int primeListSizes[PRIME_LISTS] = {0};
int sizeAndValues[PRIME_LISTS] = {
    PRIME_MASK(3, 2, 3, 5),
    PRIME_MASK(0, 0, 0, 0),
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(1, 3, 0, 0),
    //@4
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(1, 5, 0, 0),
    PRIME_MASK(2, 2, 3, 0),
    PRIME_MASK(0, 0, 0, 0),
    //@8
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(1, 3, 0, 0),
    PRIME_MASK(2, 2, 5, 0),
    PRIME_MASK(0, 0, 0, 0),
    // 12
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(0, 0, 0, 0),
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(1, 5, 0, 0),
    // 16
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(0, 0, 0, 0),
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(0, 0, 0, 0),
    //@20
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(0, 0, 0, 0),
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(0, 0, 0, 0),
    //@24
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(1, 5, 0, 0),
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(0, 0, 0, 0),
    //@28
    PRIME_MASK(1, 2, 0, 0),
    PRIME_MASK(0, 0, 0, 0),
};
#if 0
#define PRIME_LISTS2 5 * 7
#define PRIME_LISTS2_5 ((0x1) | (0x1 << 5)(0x1 << 10) | (0x1 << 15) | (0x1 << 20) | (0x1 << 25) | (0x1 << 30))
#define PRIME_LISTS2_7 ((0x7) | (0x1 << 14) | (0x1 << 21) | (0x1 << 28))
#define PRIME_BITS2 (PRIME_LISTS2_5 | PRIME_LISTS2_7)
int primeLists2[PRIME_LISTS2][MAX_N];
int primeListSizes2[PRIME_LISTS2] = {0};
#endif

int main()
{
    int N, value, maxX = 0, j = 0, minX = MAX_X, maxDup = 0, duplicates = 0;

    if (1 != scanf("%d", &N))
    {
        return 1;
    }

    for (int i = 0; i < N; i++)
    {
        if (1 != scanf("%d", &value))
        {
            return 1;
        }

        if (maxX < value)
        {
            maxX = value;
        }
        if (value > 1 && minX > value)
        {
            minX = value;
        }
        ++d[value];
        if (d[value] > 1)
        {
            ++duplicates;
            if (maxDup < value)
            {
                // maxDupIdx = i;
                maxDup = value;
            }
            continue;
        }
        numbers[j] = value;
        j++;
    }
#if LOCAL_DEV_ENV
    logError("Dups %d | %d | %d | %d\n", duplicates, N - j, N, j);
#endif
    N = j;

    if (d[maxX] > 1)
    {
        printf("%d\n", maxX);
        return 0;
    }

    if (1 == 0 && maxDup * 2 > maxX)
    {
#if LOCAL_DEV_ENV
        logError("Max-Dups %d | %d\n", maxDup, maxX);
#endif
        printf("%d\n", maxDup);
        return 0;
    }

#if LOCAL_DEV_ENV
    logError("Dups: %d\n", duplicates);
    int checks = 0;
#endif
    int cnt = 1, res = 1;

    if (N <= GCD_LIMIT)
    {
        if (duplicates >= 10)
        {
            qsort(numbers, N, sizeof(int), cmpDesc);
            int j = 1;
            int prev = numbers[0];
            for (int i = 1; i < N; i++)
            {
                if (prev == numbers[i])
                {
                    continue;
                }
                prev = numbers[i];
                numbers[j] = prev;
                j++;
            }
            N = j;
        }
        res = 1;
        for (int i = 0; i < N; i++)
        {
            int a = numbers[i];
            for (int j = i + 1; j < N; j++)
            {
                int r = gcd(a, numbers[j]);
#if LOCAL_DEV_ENV
                ++checks;
#endif
                if (res < r)
                {
                    res = r;
                }
            }
        }
        printf("%d\n", res);
#if LOCAL_DEV_ENV
        logError("Checks: %d\n", checks);
#endif
        return 0;
    }

#if LOCAL_DEV_ENV
    int adds = 0;
#endif
#if 1
    // Simple approach, work well
    // But what if we have a list of primes?
    for (int i = maxX; i >= 1; i--)
    {
        cnt = 0;
        for (int j = i; j <= maxX; j += i)
        {
            cnt += d[j];
#if LOCAL_DEV_ENV
            ++adds;
#endif
            if (cnt >= 2)
            {
                res = i;
                i = 0;
                break;
            }
        }
    }
#endif

#if LOCAL_DEV_ENV
    logError("Adds: %d | %d | %d\n", adds, cnt, res);
    adds = 0;
#endif
#if 0
for (int i = maxX; i >= 1; i--)
    {
        cnt = 0;
        int j=i;
        //int j = i/minX;
        //j *= i;
        for (; j <= maxX; j += i)
        {
            cnt += d[j];
#if LOCAL_DEV_ENV
            ++adds;
#endif
            if (cnt >= 2)
            {
                res = i;
                i = 0;
                break;
            }
        }
    }
#endif
#if 0
    qsort(numbers, N, sizeof(int), cmpDesc);
    cnt = 1;

    for (int i = 0; i < N; i++)
    {
        int v = numbers[i];
        cnt = 0;
        int limit = maxX;
        for (int j = v; j <= limit; j += v)
        {
            cnt += d[j];
#if LOCAL_DEV_ENV
            ++adds;
#endif
            if (cnt >= 2)
            {
                res = v;
                i = N;
                break;
            }
        }
    }
#endif
#if LOCAL_DEV_ENV
    logError("Adds: %d | %d\n", adds, cnt);
    adds = 0;
#endif
    printf("%d\n", res > 1 ? res : 1);

    return 0;
}