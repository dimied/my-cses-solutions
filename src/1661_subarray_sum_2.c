#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200001

#define ALLOC_SIZE MAX_N * 64

void** pValues[256] = {0};
void* mapMemory[ALLOC_SIZE][4];
int counters[MAX_N][4];
int mapMemoryFreeIndex = 0;
int lowestByteFreeIndex = 0;

int addToKey(long long int value, int toAdd)
{
    long long int highByte = (value >> 56) + 128;
    void **p = pValues[highByte];

    if (p == 0)
    {
        if (mapMemoryFreeIndex == ALLOC_SIZE)
        {
            return 1;
        }
        p = &mapMemory[mapMemoryFreeIndex][0];
        mapMemory[mapMemoryFreeIndex][0] = 0;
        mapMemory[mapMemoryFreeIndex][1] = 0;
        mapMemory[mapMemoryFreeIndex][2] = 0;
        mapMemory[mapMemoryFreeIndex][3] = 0;
        pValues[highByte] = p;
        ++mapMemoryFreeIndex;
    }
    if (value < 0)
    {
        value = value & ((0x1UL << 56) - 1);
    }
    int idx, shift = 54;
    void *pSub;
    while (shift > 2)
    {
        idx = (value >> shift) & 0x3;
        pSub = p[idx];
        if (pSub == 0)
        {
            if (mapMemoryFreeIndex == ALLOC_SIZE)
            {
                return 1;
            }
            pSub = &mapMemory[mapMemoryFreeIndex][0];
            mapMemory[mapMemoryFreeIndex][0] = 0;
            mapMemory[mapMemoryFreeIndex][1] = 0;
            mapMemory[mapMemoryFreeIndex][2] = 0;
            mapMemory[mapMemoryFreeIndex][3] = 0;
            p[idx] = pSub;
            ++mapMemoryFreeIndex;
        }
        p = (void **)pSub;

        shift -= 2;
    }
    idx = value & 0x3;
    int *pInt = (int *)p[idx];
    if (pInt == 0)
    {
        if (lowestByteFreeIndex == MAX_N)
        {
            return 1;
        }
        pInt = &counters[lowestByteFreeIndex][0];
        pInt[0] = 0;
        pInt[1] = 0;
        pInt[2] = 0;
        pInt[3] = 0;
        p[idx] = (void*)pInt;
        ++lowestByteFreeIndex;
    }
    pInt[0] += toAdd;

    return 0;
}

int getKeyValue(long long int key)
{
    long long int highByte = (key >> 56) + 128;
    void **p = pValues[highByte];

    if (p == 0)
    {
        return 0;
    }
    if (key < 0)
    {
        key = key & ((0x1UL << 56) - 1);
    }
    int idx, shift = 54;
    void *pSub;
    while (shift > 2)
    {
        idx = (key >> shift) & 0x3;
        pSub = p[idx];
        if (pSub == 0)
        {
            return 0;
        }
        p = (void **)pSub;

        shift -= 2;
    }
    idx = key & 0x3;
    int *pInt = (int *)p[idx];
    if (pInt == 0)
    {
        return 0;
    }
    return *pInt;
}

long long int numbers[MAX_N];

int main()
{
    int i = 0, N, X, currentValue, count = 0;
    long long int sum = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %d", &N, &X);
    addToKey(0, 1);

    for (; i < N; i++)
    {
        scanf("%d", &currentValue);
        sum += currentValue;
        count += getKeyValue(sum - X);

        int res = addToKey(sum, 1);
        if (res > 0)
        {
            printf("FAILED TO ADD: %lld @%d\n", sum, i);
            return 1;
        }
        numbers[i] = sum;

        // pre += a;
        // cnt += freq[pre-X];
        // freq[pre]++;
    }
#pragma GCC diagnostic pop
#if 0
if(count==0) {
for(i=0; i < N; i++) {
        printf("%lld : %d\n", numbers[i], getKeyValue(numbers[i]));
    }
}
#endif
    

    printf("%d\n", count);
    // printf("%d %d %d\n", count, mapMemoryFreeIndex, lowestByteFreeIndex);
    return 0;
}