#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200000

int numbers[MAX_N];

int main()
{
    int i = 0, j = 0, N, X, currentValue, count = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d %d", &N, &X);
    for (; i < N; i++)
    {
        scanf("%d", &currentValue);
        // One element subarray?
        if (currentValue == X)
        {
            ++count;
            continue;
        }
        if (currentValue < X)
        {
            numbers[j] = currentValue;
            ++j;
        }
        // else: all other values can't be part of any subarray with given sum
    }
#pragma GCC diagnostic pop
    N = j;
    if (N < 2)
    {
        printf("%d\n", count);
        return 0;
    }
    j = 0;
    long long int sum = 0;
    for (i = 0; i < N; i++)
    {
        sum += numbers[i];
        while (j < i && sum > X)
        {
            sum -= numbers[j++];
        }
        if (sum == X)
        {
            ++count;
        }
    }

    printf("%d\n", count);
    return 0;
}