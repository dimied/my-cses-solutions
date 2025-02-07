#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200000

int sizes[MAX_N];
int appartmentSizes[MAX_N];

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
    int i = 0, currentValue, numApplicants, numAppartments, maxDiff;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %d %d", &numApplicants, &numAppartments, &maxDiff);
    for (; i < numApplicants; i++)
    {
        scanf("%d", &currentValue);
        sizes[i] = currentValue;
    }
    qsort(sizes, numApplicants, sizeof(int), cmp);

    int j = 0, maxS = sizes[numApplicants - 1] + maxDiff, s = sizes[0] - maxDiff;
    if (s < 0)
    {
        s = 0;
    }

    // try to ignore invalid appartment sizes
    for (i = 0; i < numAppartments; i++)
    {
        scanf("%d", &currentValue);

        if (currentValue >= s && currentValue <= maxS)
        {
            appartmentSizes[j] = currentValue;
            ++j;
        }
    }
#pragma GCC diagnostic pop
    // printf("D: %d\n", numAppartments - j);
    numAppartments = j;

    qsort(appartmentSizes, numAppartments, sizeof(int), cmp);
    int hits = 0, applicantIdx = 0, appartmentIdx = 0;

    while (applicantIdx < numApplicants)
    {
        s = sizes[applicantIdx];
        maxS = s + maxDiff;
        s -= maxDiff;

        while (appartmentIdx < numAppartments && appartmentSizes[appartmentIdx] < s)
        {
            ++appartmentIdx;
        }
        if (appartmentIdx < numAppartments && s <= appartmentSizes[appartmentIdx] && appartmentSizes[appartmentIdx] <= maxS)
        {
            ++hits;
            ++appartmentIdx;
        }

        if (appartmentIdx == numAppartments)
        {
            break;
        }
        ++applicantIdx;
    }

    printf("%d\n", hits);
    return 0;
}