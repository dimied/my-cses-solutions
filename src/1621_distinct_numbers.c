#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200000

int numbers[MAX_N];

int cmp(const void *pA, const void *pB)
{
    long long int diff = *((int *)pA) - *((int *)pB);
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

int main()
{
    int i = 0, N, currentValue;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d", &N);
    for (; i < N; i++)
    {
        scanf("%d", &currentValue);
        numbers[i] = currentValue;
    }
#pragma GCC diagnostic pop
    if (N < 3)
    {
        printf("%d\n", numbers[0] != numbers[N - 1] ? 2 : 1);
        return 0;
    }

    qsort(numbers, N, sizeof(int), cmp);
    int distinctCount = 0, lastValue = -1;
    for (i = 0; i < N; i++)
    {
        if (lastValue != numbers[i])
        {
            ++distinctCount;
            lastValue = numbers[i];
        }
    }
    printf("%d\n", distinctCount);
    return 0;
}