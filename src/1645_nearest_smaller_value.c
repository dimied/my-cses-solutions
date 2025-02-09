#include <stdio.h>

#define MAX_N 200001

typedef struct
{
    int value;
    int indexOfNearestSmaller;
} NumberIndexPair;

NumberIndexPair numbersAndIndices[MAX_N];

int main()
{
    int i, N;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d", &N);
    if (N < 3)
    {
        if (N == 1)
        {
            puts("0");
        }
        else
        {
            scanf("%d", &i);
            scanf("%d", &N);
            puts((i < N) ? "0 1\n" : "0 0\n");
        }
        return 0;
    }

    numbersAndIndices[0].value = 0;
    numbersAndIndices[0].indexOfNearestSmaller = 0;
    numbersAndIndices[1].indexOfNearestSmaller = 0;

    scanf("%d", &numbersAndIndices[1].value);
    putchar('0');

    int indexOfSmaller, current;

    for (int i = 2; i <= N; i++)
    {
        scanf("%d", &current);

        indexOfSmaller = i - 1;
        while (numbersAndIndices[indexOfSmaller].value >= current)
        {
            indexOfSmaller = numbersAndIndices[indexOfSmaller].indexOfNearestSmaller;
        }

        numbersAndIndices[i].value = current;
        numbersAndIndices[i].indexOfNearestSmaller = indexOfSmaller;
        printf(" %d", indexOfSmaller);
    }
#pragma GCC diagnostic pop
    puts("");

    return 0;
}