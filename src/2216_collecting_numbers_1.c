#include <stdio.h>
#include <stdlib.h>

// Using one entry more to avoid subtractions
#define MAX_N 200000 + 1

int indices[MAX_N];

int main()
{
    int i = 1, N, currentValue;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d", &N);
    if (N == 1)
    {
        printf("1\n");
        return 0;
    }
    int rounds = 1;

    // store indices
    for (; i <= N; i++)
    {
        scanf("%d", &currentValue);
        indices[currentValue] = i;
    }
#pragma GCC diagnostic pop
    // count how many new round we'll need to start.
    for (i = 2; i <= N; i++)
    {
        if (indices[i - 1] > indices[i])
        {
            ++rounds;
        }
    }

    printf("%d\n", rounds);

    return 0;
}