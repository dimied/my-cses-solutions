#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200001

typedef struct
{
    int songId;
    int index;
} Item;

Item playlist[MAX_N];
Item playlist2[MAX_N];

int cmp(const void *pA, const void *pB)
{
    long long int diff = ((Item *)pA)->songId - ((Item *)pB)->songId;
    if (diff > 0)
    {
        return 1;
    }
    if (diff < 0)
    {
        return -1;
    }
    if (((Item *)pA)->index < ((Item *)pB)->index)
    {
        return -1;
    }
    return 1;
}

int main()
{
    int i = 0, maxLength = 1, currentValue, numStations;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d", &numStations);
    if (numStations == 1)
    {
        printf("%d\n", maxLength);
        return 0;
    }

    // We could use a map and store indices
    // But we do it with 2 lists and subtract indices

    for (; i < numStations; i++)
    {
        scanf("%d", &currentValue);
        playlist[i].songId = currentValue;
        playlist[i].index = i;
    }
#pragma GCC diagnostic pop

    qsort(playlist, numStations, sizeof(Item), cmp);

    // Create second array
    //- Place items at right positions
    //- Iterate and check for previous indices ()
    int lastValue = -1, lastIndex;

    for (i = 0; i < numStations; i++)
    {
        int index = playlist[i].index;
        playlist2[index].songId = playlist[i].songId;
        // previous+1 or -1 for first occurence
        playlist2[index].index = (lastValue == playlist[i].songId) ? lastIndex + 1 : -1;
        lastValue = playlist[i].songId;
        lastIndex = index;
    }
    int leftIdx = 0, currentIndex;
    for (i = 0; i < numStations; i++)
    {
        currentIndex = playlist2[i].index;
        int len = i - leftIdx;
        if (currentIndex > 0)
        {
            if (leftIdx < currentIndex)
            {
                leftIdx = currentIndex;
            }
        }
        else
        {
            // otherwise index < 0, i.e. unique so far
            ++len;
        }
        if (maxLength < len)
        {
            maxLength = len;
        }
    }
    if (maxLength < i - leftIdx)
    {
        maxLength = i - leftIdx;
    }

    printf("%d\n", maxLength);
    return 0;
}