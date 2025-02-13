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

int main()
{
    int N, value, maxX = 0;

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
        numbers[i] = value;
        if (maxX < value)
        {
            maxX = value;
        }
        d[value]++;
    }
    int cnt = 1;
#if LOCAL_DEV_ENV
    int adds = 0;
#endif

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
                cnt = i;
                i=0;
                break;
            }
        }
    }
    printf("%d\n", cnt);

#if LOCAL_DEV_ENV
    logError("Adds: %d\n", adds);
#endif

    return 0;
}