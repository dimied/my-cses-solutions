#include <stdio.h>
#include <stdlib.h>

#define MAX_N 5000

typedef long long int ll;

typedef struct
{
    int num;
    int index;
} IntPair;

IntPair numbers[MAX_N];

int cmp(const void *pA, const void *pB)
{
    ll diff = ((IntPair *)pA)->num - ((IntPair *)pB)->num;
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
    int i = 0, j = 0, N, X, currentValue;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %d", &N, &X);

    for (; i < N; i++)
    {
        scanf("%d", &currentValue);
        // We don't need numbers which are greater than the sum
        if (currentValue < X)
        {
            numbers[j].num = currentValue;
            numbers[j].index = i;
            ++j;
        }
    }
#pragma GCC diagnostic pop

    N = j;
    if (N < 3)
    {
        printf("IMPOSSIBLE\n");
        return 0;
    }
    // printf("Skipped: %d = %d - %d", N-j, N, j);

    qsort(numbers, N, sizeof(IntPair), cmp);

    int rightLimit = N - 1, sum;
    const int leftLimit = N - 2;

    for (i = 0; i < leftLimit; i++)
    {
        int value = X - numbers[i].num;
        while (i + 1 < rightLimit && numbers[rightLimit].num >= value)
        {
            --rightLimit;
        }
        int r = rightLimit;

        for (int leftIndex = i + 1; leftIndex < r; leftIndex++)
        {
            sum = numbers[leftIndex].num + numbers[r].num;
            while (leftIndex + 1 < r)
            {
                sum = numbers[leftIndex].num + numbers[r].num;
                if (sum <= value)
                {
                    break;
                }
                --r;
            }
            if (sum == value)
            {
                printf("%d %d %d\n", numbers[i].index + 1, numbers[leftIndex].index + 1, numbers[r].index + 1);
                return 0;
            }
        }
    }

    printf("IMPOSSIBLE\n");
#if 0
    for(i=0; i < N; i++) {
        printf("%d|%d ", numbers[i].num, numbers[i].index+1);
    }
    printf("\n");
#endif
    return 0;
}