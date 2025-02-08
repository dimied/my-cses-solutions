#include <stdio.h>

#define MAX_N 200001

int numbers[MAX_N];

void josephine2(int N)
{
    putchar('2');
    int i = 4;

    // First round
    int removed = 1, j = 3;
    numbers[2] = 3;

    while (i <= N)
    {
        printf(" %d", numbers[i]);
        // rearrange
        numbers[j] = i + 1;
        ++j;
        i += 2;
        ++removed;
    }
    i -= N;
    N -= removed;

    while (N > 1)
    {
        removed = 0;
        int j = i;
        while (i <= N)
        {
            //"remove"
            printf(" %d", numbers[i]);
            // rearrange
            numbers[j] = numbers[i + 1];
            ++j;
            i += 2;
            ++removed;
        }
        i -= N;
        N -= removed;
    }
    if (N == 1)
    {
        printf(" %d\n", numbers[1]);
    }
    else
    {
        putchar('\n');
    }
}

int main()
{
    int i, N;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d", &N);
#pragma GCC diagnostic pop

    if (N < 3)
    {
        printf((N == 2) ? "2 1\n" : "1\n");
        return 0;
    }
    for (i = 1; i <= N; i++)
    {
        numbers[i] = i;
    }
    // We can use a tree or some special map
    // We add values in O(n*log(n))
    // We search and remove values in O(n*log(n))
    // We don't have a built-in tree or map in C
    // We can do it in a different way
    // We remove on every round every second values, i.e. half of values
    // We rearrange the values, this would require n/2 operations
    // I.e. we would remove and rearrange 1/2^i of the initial value
    // in every round 'i' and in the sum n values and we would require log(n) rounds.
    josephine2(N);

    return 0;
}