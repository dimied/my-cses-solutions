#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

#if LOCAL_DEV_ENV == 0
#pragma GCC optimize("Ofast")
#endif

#include <stdio.h>

#if LOCAL_DEV_ENV
#define logError(...) fprintf(stderr, __VA_ARGS__)
#endif

#define MAX_N 1000000
#define INFINITY 0x7FFFFFFE

int stepCounters[MAX_N + 1];

#define ALGORITHM 2

int main()
{
    int N;
    if (1 != scanf("%d", &N))
    {
        return 1;
    }

    for (int i = 1; i < 10; i++)
    {
        stepCounters[i] = 1;
    }

#if ALGORITHM == 1
    for (int i = 10; i <= N; i++)
    {
        stepCounters[i] = INFINITY;
    }

    for (int i = 10; i <= N; i++)
    {
        int d = i, result = INFINITY;

        while (d != 0)
        {
            int candidate = stepCounters[i - (d % 10)] + 1;
            if (result > candidate)
            {
                result = candidate;
            }

            d /= 10;
        }
        stepCounters[i] = result;
    }
#endif

#if ALGORITHM == 2
    for (int i = 10; i <= N; i++)
    {
        int d = i, result = INFINITY;

        while ((d % 10) == 0)
        {
            d /= 10;
        }

        while (d != 0)
        {
            int candidate = (d % 10);
            if (candidate != 0)
            {
                candidate = stepCounters[i - candidate] + 1;
                if (result > candidate)
                {
                    result = candidate;
                }
            }

            d /= 10;
        }
        stepCounters[i] = result;
    }
#endif

    printf("%d\n", stepCounters[N]);

    return 0;
}