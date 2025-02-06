#include <stdio.h>

int main()
{
    int N;
    long long int lastNum, currentNum;
    unsigned long long int sumOfAdditions = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d", &N);  

    scanf("%lld", &lastNum);

    for (--N; N > 0; --N)
    {
        scanf("%lld", &currentNum);
        if (currentNum < lastNum)
        {
            sumOfAdditions += (lastNum - currentNum);
            currentNum = lastNum;
        }
        else
        {
            lastNum = currentNum;
        }
    }

#pragma GCC diagnostic pop

    printf("%lld\n", sumOfAdditions);

    return 0;
}