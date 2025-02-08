#include <stdio.h>
#include <stdlib.h>
//
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

    if (N > 1)
    {
        if (N > 2)
        {
            qsort(numbers, N, sizeof(int), cmp);
        }
        else if (numbers[1] < numbers[0])
        {
            i = numbers[0];
            numbers[0] = numbers[1];
            numbers[1] = i;
        }
    }

    long long int sum = 1;
    //We don't find real subset of coins
    //We search here the smallest sum for a contiguous subarray.
    for (i = 0; i < N && numbers[i] <= sum; i++)
    {
        sum += numbers[i];
    }

    printf("%lld\n", sum);

    return 0;
}