#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200000
#define MIN_VALUE -(1000 * 1000 * 1000 + 1)

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
    int i = 0, N, currentValue, negativeCount = 0;
    long long int allSum = 0, maxSum = MIN_VALUE;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d", &N);
    for (; i < N; i++)
    {
        scanf("%d", &currentValue);
        numbers[i] = currentValue;
        if (currentValue < 0)
        {
            ++negativeCount;
        }
        if (maxSum < currentValue)
        {
            maxSum = currentValue;
        }
        allSum += currentValue;
    }
#pragma GCC diagnostic pop
    if (negativeCount == 0)
    {
        printf("%lld\n", allSum);
        return 0;
    }

    if (N < 3)
    {
        printf("%lld\n", maxSum);
        return 0;
    }

    long long int currentSum = 0;

    //Seems simple, but why does it work?

    for (i = 0; i < N; i++)
    {
        currentSum += numbers[i];
        if (maxSum < currentSum)
        {
            maxSum = currentSum;
        }
        if (currentSum < 0)
        {
            currentSum = 0;
        }
    }

    printf("%lld\n", maxSum);
    return 0;
}