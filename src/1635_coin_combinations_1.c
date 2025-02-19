#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

#if LOCAL_DEV_ENV == 0
#pragma GCC optimize("Ofast")
#endif

#include <stdio.h>
#include <stdlib.h>

#if LOCAL_DEV_ENV
#define logError(...) fprintf(stderr, __VA_ARGS__)
#endif

#define MODULO_VALUE 1000000007
#define MAX_NUMBER_OF_COINS 100
#define MAX_COIN_SUM 1000000
#define INFINITY 0x7FFFFFFF

int coins[MAX_NUMBER_OF_COINS];
int numOfWays[MAX_COIN_SUM + 1];

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
    int numCoins, coinSum;
    if (2 != scanf("%d %d", &numCoins, &coinSum))
    {
        return 1;
    }

    int *pCoins = coins, value, newSize = 0, maxValue = 0;
    *pCoins = INFINITY;
    for (int i = 0; i < numCoins; i++)
    {
        if (1 != scanf("%d", &value))
        {
            return 2;
        }
        if (value <= coinSum)
        {
            if (maxValue < value)
            {
                maxValue = value;
            }
            *pCoins = value;
            ++pCoins;
            ++newSize;
        }
    }
    numCoins = newSize;
    if (numCoins < 2)
    {
        coinSum = (coinSum % (coins[0]) == 0) ? 1 : 0;
        printf("%d\n", (numCoins == 0) ? 0 : coinSum);
        return 0;
    }

    // by sorting we can return faster from inner loop later
    qsort(coins, numCoins, sizeof(int), cmp);
    //
    int j = 0;
    numOfWays[0] = 1;
    for (int i = 0; i < numCoins; i++)
    {
        int coin = coins[i];
        for (; j < coin; j++)
        {
            numOfWays[j] = 0;
        }
        ++j;
        numOfWays[coin] = 1;
    }

    for (int i = 1; i < coinSum; i++)
    {
        int count = numOfWays[i];
        if (count != 0)
        {
            for (int j = 0; j < numCoins; j++)
            {
                int currentSum = i + coins[j];
                if (currentSum <= coinSum)
                {
                    numOfWays[currentSum] = (numOfWays[currentSum] + count) % MODULO_VALUE;
                }
                else
                {
                    // we are going forward, which means
                    // that on the next iteration (i+1)+coins[j]
                    // will also be greater than the sum
                    numCoins = j;
                    if(numCoins==0) {
                        i = coinSum;
                    }
                }
            }
        }
    }

    printf("%d\n", numOfWays[coinSum]);

    return 0;
}