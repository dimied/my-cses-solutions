#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200000

int numbers[MAX_N];
long long int numbersMerged[MAX_N];

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
    int i = 0, numMachines, currentValue;
    long long int numProducts;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %lld", &numMachines, &numProducts);
    for (; i < numMachines; i++)
    {
        scanf("%d", &currentValue);
        numbers[i] = currentValue;
    }
#pragma GCC diagnostic pop

    if (numMachines == 1)
    {
        printf("%lld\n", (long long int)numProducts * (long long int)numbers[0]);
        return 0;
    }

    qsort(numbers, numMachines, sizeof(int), cmp);
    // We can have multiple machine having same working speed
    // We can treat them as being just one with

    long long int minTimeLowerRange = 1;
    long long int numProductsCanProduce, newTime;
    long long int minTime = numProducts * numbers[numMachines - 1];
#if 0

    int j = 0, added = 0, lastValue = numbers[0];
    long long int lastSum = lastValue;
    for (i = 1; i < numMachines; i++)
    {
        if (numbers[i] == lastValue)
        {
            lastSum += lastValue;
            ++added;
        }
        else
        {
            numbersMerged[j] = lastSum;
            ++j;
            added = 0;
            lastValue = numbers[i];
            lastSum = lastValue;
        }
    }
    if (added > 0)
    {
        numbersMerged[j] = lastSum;
        ++j;
    }
    // Use merged version
    if (numMachines > j)
    {

        //int t = numMachines;
        numMachines = j;

        // minTime = numProducts * numbersMerged[numMachines - 1];

        while (minTimeLowerRange <= minTime)
        {
            newTime = minTimeLowerRange + (minTime - minTimeLowerRange) / 2;

            numProductsCanProduce = 0;

            for (i = 0; i < numMachines; i++)
            {
                numProductsCanProduce += newTime / numbersMerged[i];
                if (numProductsCanProduce > numProducts)
                {
                    break;
                }
            }

            if (numProductsCanProduce >= numProducts)
            {
                minTime = newTime - 1;
            }
            else
            {
                minTimeLowerRange = newTime + 1;
            }
        }

        if (minTime < minTimeLowerRange)
        {
            minTime = minTimeLowerRange;
        }

        //numMachines = t;
        numProductsCanProduce = 0;
        for (i = 0; i < numMachines; i++)
        {
            long long int toAdd = minTime / numbersMerged[i];

            numProductsCanProduce += toAdd;
            if (numProductsCanProduce > numProducts)
            {
                break;
            }
        }

        // fine tune
        while (numProductsCanProduce > numProducts)
        {
            numProductsCanProduce = 0;
            --minTime;
            for (i = 0; i < numMachines; i++)
            {
                long long int toAdd = minTime / numbersMerged[i];

                numProductsCanProduce += toAdd;
                if (numProductsCanProduce > numProducts)
                {
                    break;
                }
            }
        }
        if (numProductsCanProduce < numProducts)
        {
            ++minTime;
        }

        // printf("%lld %d %d\n", minTime > 0 ? minTime : 1, numMachines, j);
        printf("%lld\n", minTime > 0 ? minTime : 1);
        return 0;
    }
#endif

    while (minTimeLowerRange <= minTime)
    {
        newTime = minTimeLowerRange + (minTime - minTimeLowerRange) / 2;

        numProductsCanProduce = 0;

        for (i = 0; i < numMachines; i++)
        {
            numProductsCanProduce += newTime / numbers[i];
            if (numProductsCanProduce > numProducts)
            {
                break;
            }
        }

        if (numProductsCanProduce >= numProducts)
        {
            minTime = newTime - 1;
        }
        else
        {
            minTimeLowerRange = newTime + 1;
        }
    }

    if (minTime < minTimeLowerRange)
    {
        minTime = minTimeLowerRange;
    }

    numProductsCanProduce = 0;
    for (i = 0; i < numMachines; i++)
    {
        long long int toAdd = minTime / numbers[i];

        numProductsCanProduce += toAdd;
        if (numProductsCanProduce > numProducts)
        {
            break;
        }
    }

    // fine tune
    while (numProductsCanProduce > numProducts)
    {
        numProductsCanProduce = 0;
        --minTime;
        for (i = 0; i < numMachines; i++)
        {
            long long int toAdd = minTime / numbers[i];

            numProductsCanProduce += toAdd;
            if (numProductsCanProduce > numProducts)
            {
                break;
            }
        }
    }
    if (numProductsCanProduce < numProducts)
    {
        ++minTime;
    }

    // printf("%lld %d %d\n", minTime > 0 ? minTime : 1, numMachines, j);
    printf("%lld\n", minTime > 0 ? minTime : 1);
    return 0;
}