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

#define MAX_NUMBER_OF_COINS 100
#define MAX_COIN_SUM 1000000
#define INFINITY -1
typedef long long int ll;

int coins[MAX_NUMBER_OF_COINS];
int minCoinNumbers[MAX_COIN_SUM + 1];

#define ALGORITHM 2

#if ALGORITHM >= 2
int cmp(const void *pA, const void *pB)
{
    ll diff = *((int *)pA) - *((int *)pB);
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
#endif

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
        coinSum = (coinSum % (coins[0]) == 0) ? (coinSum / coins[0]) : -1;
        printf("%d\n", coinSum);
        return 0;
    }
    if (coinSum % maxValue == 0)
    {
        printf("%d\n", coinSum / maxValue);
        return 0;
    }

#if LOCAL_DEV_ENV
    int numCalcs = 0;
#endif

#if ALGORITHM == 1
    minCoinNumbers[0] = 0;
    // Initially we don't know so mark as invalid
    for (int i = 1; i <= coinSum; i++)
    {
        minCoinNumbers[i] = INFINITY;
    }

    //
    pCoins = coins;
    for (int i = 0; i < numCoins; i++)
    {
        minCoinNumbers[*pCoins] = 1;
        ++pCoins;
    }

    if (minCoinNumbers[coinSum] != INFINITY)
    {
        printf("1\n");
        return 0;
    }

    for (int i = 0; i < numCoins; i++)
    {
        int coin = coins[i];
        const int limit = coinSum - coin;
        //+1 because we already assigned @coin to 1
        int *pCoinNum = &minCoinNumbers[coin] + 1;
        for (int j = 1; j <= limit; j++)
        {
#if LOCAL_DEV_ENV
            ++numCalcs;
#endif
            int numCoinsToCheck = minCoinNumbers[j];
            if (numCoinsToCheck != INFINITY)
            {
                ++numCoinsToCheck;
                if (*pCoinNum == INFINITY)
                {
                    *pCoinNum = numCoinsToCheck;
                }
                else if (*pCoinNum > numCoinsToCheck)
                {
                    *pCoinNum = numCoinsToCheck;
                }
            }
            ++pCoinNum;
        }
    }
#endif

#if ALGORITHM == 2
    // sorting coins may lead to less computations later
    qsort(coins, numCoins, sizeof(int), cmp);

    minCoinNumbers[0] = 0;

    if (coins[0] == 1)
    {
        for (int i = 1; i < coinSum; i++)
        {
            minCoinNumbers[i] = i;
        }
        minCoinNumbers[coinSum] = INFINITY;
    }
    else
    {
        // Initially we don't know so mark as invalid
        for (int i = 1; i <= coinSum; i++)
        {
            minCoinNumbers[i] = INFINITY;
        }
    }

    //
    pCoins = &coins[0];
    for (int i = 0; i < numCoins; i++)
    {
        int coinValue = *pCoins;
#if 0
        for(int j = i+1; j < numCoins; j++) {
            int sum = coinValue + coins[j];
            if(sum <= coinSum) {
                minCoinNumbers[sum] = 2;
                continue;
            }
            break;
        }
#endif
        minCoinNumbers[coinValue] = 1;
        ++pCoins;
    }

    if (minCoinNumbers[coinSum] != INFINITY)
    {
        printf("1\n");
        return 0;
    }

    for (int i = 0; i < numCoins; i++)
    {
        int coin = coins[i];
        const int limit = coinSum - coin;
        //+1 because we already assigned @coin to 1
        int *pCoinNum = &minCoinNumbers[coin] + 1;
        for (int j = 1; j <= limit; j++)
        {
#if LOCAL_DEV_ENV
            ++numCalcs;
#endif
            int numCoinsToCheck = minCoinNumbers[j];
            if (numCoinsToCheck != INFINITY)
            {
                ++numCoinsToCheck;
                if (*pCoinNum == INFINITY)
                {
                    *pCoinNum = numCoinsToCheck;
                }
                else if (*pCoinNum > numCoinsToCheck)
                {
                    *pCoinNum = numCoinsToCheck;
                }
            }
            ++pCoinNum;
        }
    }
#endif

    printf("%d\n", minCoinNumbers[coinSum]);
#if LOCAL_DEV_ENV
    logError("#calcs: %d\n", numCalcs);
    ++numCalcs;
#endif

    return 0;
}