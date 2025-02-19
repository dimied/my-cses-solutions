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

    int *pCoins = coins, value, newSize = 0;
    for (int i = 0; i < numCoins; i++)
    {
        if (1 != scanf("%d", &value))
        {
            return 2;
        }
        if (value <= coinSum)
        {
            *pCoins = value;
            ++pCoins;
            ++newSize;
        }
    }
    numCoins = newSize;

    // by sorting we can mayby get better cache access patterns
    qsort(coins, numCoins, sizeof(int), cmp);
    //
    numOfWays[0] = 1;
    for (int i = 1; i <= coinSum; i++)
    {
        numOfWays[i] = 0;
    }

    for (int i = 0; i < numCoins; i++)
    {
        const int coin = coins[i];
        const int limit = coinSum - coin;
        int *pCoin = &numOfWays[coin];
        *pCoin = (*pCoin) + 1;
        ++pCoin;
        for (int j = 1; j <= limit; j++)
        {
            *pCoin = (*pCoin + numOfWays[j]) % MODULO_VALUE;
            ++pCoin;
        }
    }

    printf("%d\n", numOfWays[coinSum]);

    return 0;
}