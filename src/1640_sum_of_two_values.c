#include <stdio.h>
#include <stdlib.h>

#define USE_INDICES 1
#define USE_RANGE_MAX_VALUES 1
#define BRUTE_FORCE_LIMIT 500
//
#define MAX_N 200000
#define INDEX_ARRAY_LENGTH 128 * 256

typedef struct
{
    int num;
    int index;
} IntPair;

IntPair numbers[MAX_N];

#if USE_INDICES
// To store indices in sorted numbers
//@indices[high 15 bits of] = index of the first number having given 15 bits
int indices[INDEX_ARRAY_LENGTH] = {0};
// Min-Max-Pairs
#if USE_RANGE_MAX_VALUES
int maxValues[INDEX_ARRAY_LENGTH];
#endif
#endif

int cmp(const void *pA, const void *pB)
{
    long long int diff = ((IntPair *)pA)->num - ((IntPair *)pB)->num;
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

const char *pImpossible = "IMPOSSIBLE\n";
const char *p12 = "1 2\n";

void bruteForceAlgorithm(int X, int N)
{
    int diff, rightLimit = N, lastValue = -1;
    const int leftLimit = N - 1;

    if (N <= BRUTE_FORCE_LIMIT)
    {
        for (int i = 0; i < leftLimit; i++)
        {
            int value = X - numbers[i].num;

            if (lastValue == value)
            {
                continue;
            }
            lastValue = value;
            int j = i + 1;
            for (; j < rightLimit; j++)
            {
                diff = value - numbers[j].num;
                if (diff < 0)
                {
                    rightLimit = j;
                    break;
                }
                if (diff == 0)
                {
                    printf("%d %d\n", numbers[i].index + 1, numbers[j].index + 1);
                    return;
                }
            }
        }
        printf("%s", pImpossible);
    }
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
        if (currentValue <= X)
        {
            numbers[j].num = currentValue;
            numbers[j].index = i;
            ++j;
        }
    }
#pragma GCC diagnostic pop

    N = j;
    if (N < 2)
    {
        printf("%s", pImpossible);
        return 0;
    }
    else if (N == 2)
    {
        printf("%s", (numbers[0].num + numbers[1].num == X) ? p12 : pImpossible);
        return 0;
    }

    qsort(numbers, N, sizeof(IntPair), cmp);

    if (numbers[N - 1].num + numbers[N - 2].num < X)
    {
        printf("%s", pImpossible);
        return 0;
    }

    int diff, rightLimit = N, lastValue = -1;
    const int leftLimit = N - 1;

    if (N <= BRUTE_FORCE_LIMIT)
    {
        bruteForceAlgorithm(X, N);
        return 0;
    }

#if USE_INDICES

    // store indices to have faster access to good candidates
    // add 1 to distinguish from unused
    for (i = 0; i < N; i++)
    {
        currentValue = numbers[i].num;
        j = currentValue >> 16;
#if USE_RANGE_MAX_VALUES
        currentValue = currentValue & 0xFFFF;
#endif
        if (indices[j] == 0)
        {
            indices[j] = i + 1;
#if USE_RANGE_MAX_VALUES
            maxValues[j] = currentValue;
#endif
        }
#if USE_RANGE_MAX_VALUES
        else
        {
            if (maxValues[j] < currentValue)
            {
                maxValues[j] = currentValue;
            }
        }
#endif
    }
#endif

    for (i = 0; i < leftLimit; i++)
    {
        int value = X - numbers[i].num;

        if (lastValue == value)
        {
            continue;
        }
        lastValue = value;

#if USE_INDICES
        currentValue = value >> 16;
        j = indices[currentValue] - 1;
        if (j < 0)
        {
            continue;
        }

#if USE_RANGE_MAX_VALUES
        // Check ranges
        if (maxValues[currentValue] < (value & 0xFFFF))
        {
            continue;
        }
#endif

        if (j <= i)
        {
            j = i + 1;
        }
#else
        j = i + 1;
#endif

        for (; j < rightLimit; j++)
        {
            diff = value - numbers[j].num;
            if (diff < 0)
            {
                rightLimit = j;
                break;
            }
            if (diff == 0)
            {
                printf("%d %d\n", numbers[i].index + 1, numbers[j].index + 1);
                return 0;
            }
        }
    }

    printf("%s", pImpossible);
#if 0
    for(i=0; i < N; i++) {
        printf("%d|%d ", numbers[i].num, numbers[i].index+1);
    }
    printf("\n");
#endif
    return 0;
}