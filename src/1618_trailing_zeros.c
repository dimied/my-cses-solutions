#include <stdio.h>

int main()
{
    long long int N, count = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%lld", &N);
#pragma GCC diagnostic pop

#if 0
    //Very slow
    long long int num = 2 * 3 * 4;
    int i = 5;
    while (i <= N)
    {
        num *= i;
        while (num % 10 == 0)
        {
            num /= 10;
            ++count;
        }
        num = num % 1000000000;
        ++i;
        // N/=10;
    }
#else
    // We only count numbers of N/5 + N/(5^2) + N/(5^3) + ...
    // Why?
    // To get a number with some 0 at the end we need 2 numbers
    // one should have a prime factorization with ....5^p
    // and the other with a prime factorization with ....2^d
    // For any number ending with 5 (i.e. divisible by 5) there will be numbers
    // less that have 2 as prime factor
    // We know that there are more even numbers(div by 2) than those divisible by 5
    // We only need to cosider those divisible by 5.
    // We get the count of them by floor(N/5)
    // These amount of numbers are only those which are divisible by at least one 5.
    // The amount of numbers divisible by 5^2 we get by floor(N/(5^2))
    // In general we the amount of numbers divisible by 5^p by floor(N/(5^p))
    // We only need to sum up these division results.
#if 0
    long long int i = 5;
    for (; N / i >= 1; i *= 5)
    {
        count += N / i;
    }
#else
    while (N >= 5)
    {
        N /= 5;
        count += N;
    }

#endif

#endif
    printf("%lld\n", count);

    return 0;
}