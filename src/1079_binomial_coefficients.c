#include <stdio.h>
#include <stdlib.h>

#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

// See:
//  https://www.geeksforgeeks.org/modular-division/
//  https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/
//  Modulo value is a prime number, i.e. we can easily compute multiplicative inverse
#define MODULO_VALUE (1000 * 1000 * 1000 + 7)

// Binary representation of 1000000005
//  is 11 1011 1001 1010 1100 1010 0000 0101
#define CALC_INVERSE(V, RES)        \
    /*0101*/                        \
    RES = V;                        \
    V = (V * V) % MODULO_VALUE;     \
    V = (V * V) % MODULO_VALUE;     \
    RES = (RES * V) % MODULO_VALUE; \
    V = (V * V) % MODULO_VALUE;     \
    V = (V * V) % MODULO_VALUE;     \
    /*0000*/                        \
    V = (V * V) % MODULO_VALUE;     \
    V = (V * V) % MODULO_VALUE;     \
    V = (V * V) % MODULO_VALUE;     \
    V = (V * V) % MODULO_VALUE;     \
    /*1010*/                        \
    V = (V * V) % MODULO_VALUE;     \
    RES = (RES * V) % MODULO_VALUE; \
    V = (V * V) % MODULO_VALUE;     \
    V = (V * V) % MODULO_VALUE;     \
    RES = (RES * V) % MODULO_VALUE; \
    V = (V * V) % MODULO_VALUE;

#define CALC_INVERSE_EXPONENT (MODULO_VALUE - 2) >> 12

#define MAX_AB 1000 * 1000
#define MAX_N 100 * 1000
//@i*2 = a, @i*2+1 = b, @i*2+1 = result
int numbers[MAX_N * 3];

#define PRECOMPUTED_SIZE 0
int firstPairs[(PRECOMPUTED_SIZE + 2) * 2] = {1, 1, 1, 1};
int factorialInversePairs[(MAX_AB + 1) * 2];

int main()
{
    int i, N, a, b, maxA = 1, j;
    factorialInversePairs[0] = 1;
    factorialInversePairs[1] = 1;
    factorialInversePairs[2] = 1;
    factorialInversePairs[3] = 1;
#if 1
    {
        maxA = PRECOMPUTED_SIZE;
        j = 4;
        long long int factorial = 1;
        long long int inverse;

        for (i = 2; i <= PRECOMPUTED_SIZE; i++)
        {
            factorial = (factorial * i) % MODULO_VALUE;
            firstPairs[j] = (int)factorial;
            // inverse computation
            inverse = 1;
            long long int forInverse = factorial;
            long long int exponent = MODULO_VALUE - 2;
            while (exponent > 0)
            {
                if ((exponent & 0x1) != 0)
                {
                    inverse = (inverse * forInverse) % MODULO_VALUE;
                }

                forInverse = (forInverse * forInverse) % MODULO_VALUE;
                exponent >>= 1;
            }
            firstPairs[j + 1] = (int)inverse;
            j += 2;
        }
        factorialInversePairs[0] = (int)((factorial) % MODULO_VALUE);
        factorialInversePairs[1] = (int)inverse;
    }
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d", &N);
    j = 0;
    long long int product;

    for (i = 0; i < N; i++)
    {
        scanf("%d %d", &a, &b);
        int result = -1;

        if (b < 2)
        {
            a = (a < 2) ? 1 : a;
            result = a;
        }
        else
        {
            int diff = a - b;

            if (diff < 2)
            {
                result = (diff == 0) ? 1 : a;
            }
            else if (a <= PRECOMPUTED_SIZE)
            {
                //(factorial%MOD) * ((a-b)!%MOD)
                product = (long long int)firstPairs[a * 2] * (long long int)firstPairs[b * 2 + 1];
                //((factorial%MOD) * ((a-b)!%MOD)%MOD)*(b!%MOD)
                product = (product % MODULO_VALUE) * (long long int)firstPairs[diff * 2 + 1];
                result = (int)(product % MODULO_VALUE);
            }
            else
            {
                if (maxA < a)
                {
                    int idx = (maxA - PRECOMPUTED_SIZE) << 1;
                    // int idx = (maxA) << 1;

                    long long factorial = factorialInversePairs[idx];
                    idx += 2;
                    for (int f = maxA + 1; f <= a; f++)
                    {
                        factorial = (factorial * f) % MODULO_VALUE;
                        factorialInversePairs[f * 2] = (int)factorial;
                        // No inverse so far
                        // Any value >=0 means a valid value
                        factorialInversePairs[f * 2 + 1] = -1;
                        idx += 2;
                    }
                    maxA = a;
                }
            }
        }

        numbers[j] = a;
        numbers[j + 1] = b;
        numbers[j + 2] = result;

        j += 3;
    }

    j = 0;
    int computedDiffs = 0, computedBs = 0, computed = 0;
    int exponent;

    for (i = 0; i < N; i++)
    {
        long long int result = numbers[j + 2];
        if (result < 0)
        {
            ++computed;
            int aOrDiff = numbers[j];
            int b = numbers[j + 1];
            result = factorialInversePairs[aOrDiff << 1];

            aOrDiff -= b;
            long long int inverse = -1;

            int offset = (b << 1) + 1;
            // Now for b
            if (b <= PRECOMPUTED_SIZE)
            {
                inverse = firstPairs[offset];
            }
            else
            {
                offset -= (PRECOMPUTED_SIZE) * 2;
                inverse = factorialInversePairs[offset];
            }
            if (inverse < 0)
            {
                ++computedBs;
                // inverse computation
                inverse = 1;
                long long int forInverse = factorialInversePairs[offset - 1];
                exponent = MODULO_VALUE - 2;
                while (exponent > 0)
                {
                    if ((exponent & 0x1) != 0)
                    {
                        inverse = (inverse * forInverse) % MODULO_VALUE;
                    }

                    forInverse = (forInverse * forInverse) % MODULO_VALUE;
                    exponent >>= 1;
                }
                factorialInversePairs[offset] = inverse;
            }
            result = (result * inverse) % MODULO_VALUE;

            offset = (aOrDiff << 1) + 1;
            if (aOrDiff <= PRECOMPUTED_SIZE)
            {
                inverse = firstPairs[offset];
            }
            else
            {
                offset -= (PRECOMPUTED_SIZE) * 2;
                inverse = factorialInversePairs[offset];
            }
            if (inverse < 0)
            {
                ++computedDiffs;
                // inverse computation
                inverse = 1;
                long long int forInverse = factorialInversePairs[offset - 1];
                exponent = MODULO_VALUE - 2;
                while (exponent > 0)
                {
                    if ((exponent & 0x1) != 0)
                    {
                        inverse = (inverse * forInverse) % MODULO_VALUE;
                    }

                    forInverse = (forInverse * forInverse) % MODULO_VALUE;
                    exponent >>= 1;
                }
                // TODO
                factorialInversePairs[offset] = inverse;
            }
            result = (result * inverse) % MODULO_VALUE;
        }
        printf("%lld\n", result);
        j += 3;
    }

#if LOCAL_DEV_ENV
    fprintf(stderr, "#diff: %d |#b: %d |#values: %d\n", computedDiffs, computedBs, computed);
#endif
    return 0;
}