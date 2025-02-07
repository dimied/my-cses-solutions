#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200001

int weights[MAX_N];

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
    int i = 0, j = 0, hits = 0, currentValue, numChildren, maxWeight;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %d", &numChildren, &maxWeight);

    for (; i < numChildren; i++)
    {
        scanf("%d", &currentValue);
        if (currentValue <= maxWeight)
        {
            weights[j] = currentValue;
            ++j;
        }
    }
#pragma GCC diagnostic pop
    // printf("D: %d\n", numChildren - j);
    numChildren = j;
    qsort(weights, numChildren, sizeof(int), cmp);
    weights[numChildren] = 0;
    int heavyIdx = numChildren - 1;
    i = 0;
    for (; heavyIdx >= i; heavyIdx--)
    {
        ++hits;
        int w = weights[heavyIdx];
        //can we have heavy and light one in same gondola
        if (weights[i] + w <= maxWeight)
        {
            ++i;
        }
    }

    printf("%d\n", hits);
    return 0;
}