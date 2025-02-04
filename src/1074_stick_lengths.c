#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 2 * 100000
typedef long long ll;

int N;
int p[MAX_N] = {0};

ll median, sum;

int cmp(const void *pA, const void *pB)
{
    ll diff= *((int *)pA) - *((int *)pB);
    if(diff > 0) {
        return 1;
    }
    if(diff < 0) {
        return -1;
    }
    return 0;
}

int main()
{
    scanf("%d", &N);
    if (N < 2)
    {
        printf("0\n");
        return 0;
    }

    int i=0;
    int *pNum = p;

    //TODO: Scan full line and parse it
    for (; i < N; i++)
    {
        scanf("%d", pNum);
        ++pNum;
    }

    if (N == 2)
    {
        ll diff = p[1] - p[0];
        if (diff < 0)
        {
            diff = -diff;
        }
        printf("%lld\n", diff);
        return 0;
    }

    qsort(p, N, 4, cmp); //compare ints(4 bytes)

    median = p[(N - 1) / 2];
    for (int i = 0; i < N; i++)
    {
        ll diff = p[i] - median;
        if (diff < 0)
        {
            sum -= diff;
        } else {
            sum += diff;
        }
        
    }

    printf("%lld\n", sum);
    return 0;
}