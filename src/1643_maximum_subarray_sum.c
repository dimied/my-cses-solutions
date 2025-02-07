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
    int i = 0, N, currentValue, negativeCount = 0, zeroCount = 0, lastNegIndex;
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
            lastNegIndex = i;
        }
        if (currentValue == 0)
        {
            ++zeroCount;
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

    if ((N < 3) || (negativeCount + zeroCount == N))
    {
        printf("%lld\n", maxSum);
        return 0;
    }

    long long int currentSum = 0;
    if (negativeCount > 0 && lastNegIndex < N - 1000)
    {
        long long int runningSum = 0, current;
        for (i = 0; i < N; i++)
        {
            current = numbers[i];
            currentSum += current;
            runningSum += current;

            if (current < 0)
            {
                --negativeCount;
                if (negativeCount == 0)
                {
                    runningSum = allSum - runningSum;
                    if (maxSum < runningSum + currentSum)
                    {
                        maxSum = runningSum + currentSum;
                    }
                    if (maxSum < runningSum)
                    {
                        maxSum = runningSum;
                    }
                    break;
                }
            }

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

    // Seems simple, but why does it work?
    for (i = 0; i < N; i++)
    {
        currentSum += numbers[i];

        if (maxSum < currentSum)
        {
            maxSum = currentSum;
        }
        // If it becomes negative then
        //- either all values are negative and then maxSum is maximal
        //- or some last added numbers made it negative, i.e. sum before was maximum
        if (currentSum < 0)
        {
            currentSum = 0;
        }
    }

    printf("%lld\n", maxSum);
    return 0;
}