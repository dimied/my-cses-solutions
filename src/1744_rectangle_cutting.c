#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

#if LOCAL_DEV_ENV == 0
#pragma GCC optimize("Ofast")
#endif

#include <stdio.h>
#include <string.h>

#if LOCAL_DEV_ENV
#define logError(...) fprintf(stderr, __VA_ARGS__)
#endif

#define ALGORITHM 2

#define MAX_SIZE 500
// This value, because it might be used twice in sums plus 1
#define INFINITY 0x3E3E3E3E

#define SWAP_VARS(V1, V2) \
    V1 = V1 ^ V2;         \
    V2 = V1 ^ V2;         \
    V1 = V1 ^ V2;

#if LOCAL_DEV_ENV
int callStackIdx = 0;
int maxCallStackIdx = 0;
long long int numCalls = 0;
#define INC_CALLSTACK ++callStackIdx;
#define DEC_CALLSTACK --callStackIdx;
#define INC_NUM_CALLS ++numCalls;
#else
#define INC_CALLSTACK
#define DEC_CALLSTACK
#define INC_NUM_CALLS
#endif

int possibilities[MAX_SIZE + 1][MAX_SIZE + 1];

#define PRECALC_LOWER_LIMIT 100

void prepare(int A, int B)
{
    memset(possibilities, 0x3E, sizeof(int) * (MAX_SIZE + 1) * (A + 1));
#if ALGORITHM == 2
    if (A != B)
    {
        if (A > B)
        {
            SWAP_VARS(A, B)
        }
        if (B < PRECALC_LOWER_LIMIT)
        {
            return;
        }
        // Otherwise we can precompute
        // and reduce the number of calls in 'solve function'
        possibilities[1][1] = 0;
        possibilities[1][2] = 1;
        for (int i = 3; i <= B; i++)
        {
            possibilities[1][i] = (i % 2 == 0) ? (i - 1) : i;
        }
        for (int i = 2; i <= A; i++)
        {
            possibilities[i][1] = i - 1;
        }
        for (int i = 2; 0 && i <= A; i++)
        {
            int res = 1;
            possibilities[i][i] = 0; // squares don't need division
            int *pResults = &possibilities[i][0];
            for (int j = (i << 1); j <= B; j += i)
            {
                pResults[j] = (res % 2 == 0) ? (res - 1) : res;
                ++res;
            }
        }
    }
#endif
}

int solve1(int a, int b)
{
    INC_CALLSTACK
    INC_NUM_CALLS
#if LOCAL_DEV_ENV
    if (maxCallStackIdx < callStackIdx)
    {
        maxCallStackIdx = callStackIdx;
    }
#endif
    if (a > b)
    {
        SWAP_VARS(a, b);
    }
    if (possibilities[a][b] != INFINITY)
    {
        DEC_CALLSTACK
        return possibilities[a][b];
    }
    if (a == b)
    {
        DEC_CALLSTACK
        possibilities[a][b] = 0;
        return 0;
    }
    if (a == 1 || b == 1)
    {
        DEC_CALLSTACK
        int v = (a == 1 ? b - 1 : a - 1);
        possibilities[a][b] = v;
        return v;
    }
    int minValue = possibilities[a][b];
    for (int i = 1; i < a; i++)
    {
        int other = solve1(i, b) + solve1(a - i, b) + 1;
        if (minValue > other)
        {
            minValue = other;
            possibilities[a][b] = minValue;
        }
    }

    for (int i = 1; i < b; i++)
    {
        int other = solve1(a, i) + solve1(a, b - i) + 1;
        if (minValue > other)
        {
            minValue = other;
            possibilities[a][b] = minValue;
        }
    }
    DEC_CALLSTACK

    return possibilities[a][b];
}

int solve2(int a, int b)
{
    INC_CALLSTACK
    INC_NUM_CALLS
#if LOCAL_DEV_ENV
    if (maxCallStackIdx < callStackIdx)
    {
        maxCallStackIdx = callStackIdx;
    }
#endif
if (a == b)
    {
        DEC_CALLSTACK
        //possibilities[a][b] = 0;
        return 0;
    }
    if (a > b)
    {
        SWAP_VARS(a, b);
    }
    
    if (possibilities[a][b] != INFINITY)
    {
        DEC_CALLSTACK
        return possibilities[a][b];
    }
    
    if (a == 1 || b == 1)
    {
        DEC_CALLSTACK
        int v = (a == 1 ? b - 1 : a - 1);
        possibilities[a][b] = v;
        return v;
    }
    int minValue = possibilities[a][b];
    for (int i = 1; i < a; i++)
    {
        int other = possibilities[i][b];
        if (other == INFINITY)
        {
            other = solve2(i, b);
        }
        int other2 = possibilities[a - i][b];
        if (other2 == INFINITY)
        {
            other2 = solve2(a - i, b);
        }
        other += other2 + 1;

        if (minValue > other)
        {
            minValue = other;
            possibilities[a][b] = minValue;
        }
    }

    for (int i = 1; i < b; i++)
    {
        int other = possibilities[a][i];
        if (other == INFINITY)
        {
            other = solve2(a, i);
        }
        int other2 = possibilities[a][b - i];
        if (other2 == INFINITY)
        {
            other2 = solve2(a, b - i);
        }
        other += other2 + 1;

        if (minValue > other)
        {
            minValue = other;
            possibilities[a][b] = minValue;
        }
    }
    DEC_CALLSTACK

    return possibilities[a][b];
}

int main()
{
    int A, B;
    if (2 != scanf("%d %d", &A, &B))
    {
        return 1;
    }
    if (A > B)
    {
        SWAP_VARS(A, B)
    }
    // This handles also the case if A==B
    if (B % A == 0)
    {
        int res = B / A;
        // for even number we only need 1 less cut
        // for 2 => 1, 4 => 3(in half and then 2 times in half)
        if (res > 1)
        {
            res -= (1 - (res % 2));
        }
        else
        {
            --res;
        }
        printf("%d\n", res);
        return 0;
    }
    prepare(A, B);
#if ALGORITHM == 1
    solve1(A, B);
#endif
#if ALGORITHM == 2
    solve2(A, B);
#endif
    printf("%d\n", possibilities[A][B]);
#if LOCAL_DEV_ENV
    logError("#call-stack: %d |#calls: %lld\n", maxCallStackIdx, numCalls);
#endif

    return 0;
}