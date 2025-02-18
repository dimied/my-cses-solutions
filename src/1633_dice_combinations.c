#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

#if LOCAL_DEV_ENV == 0
#pragma GCC optimize("Ofast")
#endif

#include <stdio.h>

#define MODULO_VALUE 1000000007
#define MAX_N 1000000
typedef long long ll;

int combinations[MAX_N + 1];

#define ALGORITHM 3

int main()
{
    int N;
    if (1 != scanf("%d", &N))
    {
        return 1;
    }

    combinations[1] = 1;
    combinations[2] = 2;
    combinations[3] = 4;
    combinations[4] = 8;
    combinations[5] = 16;
    combinations[6] = 32;

#if ALGORITHM == 1
    for (int i = 7; i <= N; i++)
    {
        ll result = 0;
        for (int j = 1; j <= 6; j++)
        {
            result += combinations[i - j];
        }
        combinations[i] = (int)(result % MODULO_VALUE);
    }
#endif

#if ALGORITHM == 2
    for (int i = 7; i <= N; i++)
    {
        ll result = (ll)combinations[i - 1] + (ll)combinations[i - 2] + (ll)combinations[i - 3] + (ll)combinations[i - 4] + (ll)combinations[i - 5] + (ll)combinations[i - 6];
        combinations[i] = (int)(result % MODULO_VALUE);
    }
#endif
#if ALGORITHM == 3
    int sum = 32;
    int *pFirst = &combinations[0], *pResult = &combinations[7];
    for (int i = 7; i <= N; i++)
    {
        //sum += sum;
        sum <<= 1;
        sum -= (*pFirst);
        if(sum < 0) {
            sum += MODULO_VALUE;
        } else {
            sum %= MODULO_VALUE;
        }      

        ++pFirst;
        *pResult = (int)sum;
        ++pResult;
    }
#endif
    printf("%d\n", combinations[N]);

    return 0;
}