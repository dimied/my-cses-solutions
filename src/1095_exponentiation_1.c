#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

#if LOCAL_DEV_ENV == 0
#pragma GCC optimize("Ofast")
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if LOCAL_DEV_ENV
#include <sys/time.h>
#endif

#if LOCAL_DEV_ENV
#include <stdarg.h>

double getTime(struct timeval *startTimeval, struct timeval *endTimeval)
{
    long seconds = endTimeval->tv_sec - startTimeval->tv_sec;
    long microseconds = endTimeval->tv_usec - startTimeval->tv_usec;
    return (double)seconds + (double)microseconds * 0.000001f;
}
void storeTime(struct timeval *pTimeval)
{
    gettimeofday(pTimeval, 0);
}

#define logError(...) fprintf(stderr, __VA_ARGS__)

struct timeval inputStart, inputEnd, execStart, execEnd, outputStart, outputEnd;

#define START_INPUT_MEASURE storeTime(&inputStart);
#define END_INPUT_MEASURE storeTime(&inputEnd);

#define START_EXEC_MEASURE storeTime(&execStart);
#define END_EXEC_MEASURE storeTime(&execEnd);

#define START_OUT_MEASURE storeTime(&outputStart);
#define END_OUT_MEASURE storeTime(&outputEnd);
#else

#define START_INPUT_MEASURE
#define END_INPUT_MEASURE
#define START_EXEC_MEASURE
#define END_EXEC_MEASURE

#define START_OUT_MEASURE
#define END_OUT_MEASURE

#define logError(...)
#endif

#ifdef NUM_TESTS
int testIdx = 0;
#endif

#define MAX_N 200000
#define MODULO_VALUE (1000 * 1000 * 1000 + 7)

typedef long long int ll;
typedef unsigned long long int ull;

int inputNumbers[MAX_N * 2];
int numbers[MAX_N];

#define OUTPUT_BUFFER_SIZE (MAX_N * 11 + 8)
char outputBuffer[OUTPUT_BUFFER_SIZE];
const int endianTest = 0x04030201;

#if LOCAL_DEV_ENV
int alignedPrints = 0;
#endif

void printOutput(int N, char *pOut)
{
    const int *pEndian = &endianTest;
    const char *pOutStart = pOut;
    char buf[10];
    // little?
    if (*pEndian == 0x04030201)
    {
        logError("Little-Endian:\n");
        long long int *pRes, res;
        for (int i = 0; i < N; i++)
        {
            int num = numbers[i];
            if (num >= 10)
            {
                if (num >= 100)
                {
                    if ((((unsigned long long int)pOut) & 0x7) == 0)
                    {
#if LOCAL_DEV_ENV
                        ++alignedPrints;
#endif
                        int s = 0;
                        res = 0;
                        while (num > 9 && s < 7)
                        {
                            res |= (num % 10 + '0');
                            res <<= 8;
                            num /= 10;
                            ++s;
                        }
                        res |= (num % 10 + '0');
                        ++s;
                        num /= 10;
                        if (num > 0)
                        {
                            if (num >= 10)
                            {
                                *pOut = (num / 10) + '0';
                                ++pOut;
                                num %= 10;
                            }
                            *pOut = (num + '0');
                            ++pOut;
                        }

                        pRes = (long long int *)pOut;
                        *pRes = res;
                        pOut += s;
                    }
                    else
                    {
                        int j = 10;
                        do
                        {
                            --j;
                            buf[j] = num % 10 + '0';
                        } while (num /= 10);
                        while (j < 10)
                        {
                            *pOut = buf[j++];
                            ++pOut;
                        }
                    }
                }
                else
                {
                    *pOut = (num / 10) + '0';
                    ++pOut;
                    *pOut = (num % 10) + '0';
                    ++pOut;
                }
                *pOut = '\n';
                ++pOut;
            }
            else
            {
                *pOut = num + '0';
                ++pOut;
                *pOut = '\n';
                ++pOut;
            }
        }
    }
    else
    {
        logError("Big-Endian\n");

        int j = 10;
        for (int i = 0; i < N; i++)
        {
            int num = numbers[i];
            do
            {
                --j;
                buf[j] = num % 10 + '0';
            } while (num /= 10);
            while (j < 10)
            {
                *pOut = buf[j++];
                ++pOut;
            }

            *pOut = '\n';
            ++pOut;
        }
    }
    *pOut = 0;
    printf("%s", pOutStart);
}

#if LOCAL_DEV_ENV
int lowValuesCount = 0, fastChecks = 0, calcs = 0;
#endif

#if LOCAL_DEV_ENV
#define ADD_CALCS ++calcs;
#else
#define ADD_CALCS
#endif

#define CALC_EXPONENT(RES, EXP, VAL)          \
    while (EXP > 1)                           \
    {                                         \
        switch (EXP & 0x7)                    \
        {                                     \
        case 0:                               \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            break;                            \
        case 1:                               \
            RES = (RES * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            break;                            \
        case 2:                               \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            RES = (RES * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            break;                            \
        case 3:                               \
            RES = (RES * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            RES = (RES * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            break;                            \
        case 4:                               \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            RES = (RES * VAL) % MODULO_VALUE; \
            break;                            \
        case 5:                               \
            RES = (RES * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            RES = (RES * VAL) % MODULO_VALUE; \
            break;                            \
        case 6:                               \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            RES = (RES * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            RES = (RES * VAL) % MODULO_VALUE; \
            break;                            \
        case 7:                               \
            RES = (RES * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            RES = (RES * VAL) % MODULO_VALUE; \
            VAL = (VAL * VAL) % MODULO_VALUE; \
            RES = (RES * VAL) % MODULO_VALUE; \
            break;                            \
        }                                     \
        VAL = (VAL * VAL) % MODULO_VALUE;     \
        ADD_CALCS                             \
        EXP >>= 3;                            \
    }                                         \
    if (EXP == 1)                             \
    {                                         \
        ADD_CALCS                             \
        RES = (RES * VAL) % MODULO_VALUE;     \
    }

#define REP_2(B, N) (B* N), (B * N* N)
#define REP_3(B, N) (B*N), (B * N* N), (B * N * N* N)
#define REP_4(B, N) (B*N), (B * N* N), (B * N * N* N), (B * N * N * N* N)
#define REP_4E(B, N) (B* N), (B * N* N), (B * N * N* N), (B * N* N * N * (unsigned int)N)

#define EXP_4(B) (B * B * B * B)
// Fast check:
// 2^30 = 1.073.741.824 = 4^15 = 8^10 = 16^5
// 3^20 = 3.486.784.401
// 5^12 = 244.140.625
// 6^10 = 60.466.176
// 7^8 = 5.764.801
// 9^6 = 531441

#define R2_4 EXP_4(2)
#define R2_8 (R2_4 * R2_4)

#define R3_4 EXP_4(3)
#define R3_8 (R3_4 * R3_4)

#define R4_4 EXP_4(4)
#define R4_8 (R4_4 * R4_4)

#define R5_4 EXP_4(5)
#define R5_8 (R5_4 * R5_4)

#define INT_POWERS_LIMIT 8

unsigned long long int integerPowers[30][30] = {
    {REP_4(1, 2),
     REP_4(R2_4, 2),
     REP_4(R2_8, 2),
     REP_4(R2_8 *R2_4, 2),
     REP_4(R2_8 *R2_8, 2),
     REP_4(R2_8 *R2_8 *R2_4, 2),
     REP_4(R2_8 *R2_8 *R2_8, 2),
     REP_2(R2_8 *R2_8 *R2_8 *R2_4, ((unsigned int)2))},
    {REP_4(1, 3),
     REP_4(R3_4, 3),
     REP_4(R3_8, 3),
     REP_4(R3_8 *R3_4, 3),
     REP_4E(R3_8 *R3_8, 3), 0},
    {REP_4(1, 4),
     REP_4(R4_4, 4),
     REP_4(R4_8, 4),
     REP_3(R4_8 *R4_4, 4), 0},
    {REP_4(1, 5),
     REP_4(R5_4, 5),
     REP_4(R5_8, 5),
     REP_4(R5_8 *R5_4, ((ull)5)), 0},
    {REP_4(1, 6),
     REP_4(EXP_4(6), 6),
     REP_4(EXP_4(6) * EXP_4(6), ((ull)6)), 0},
     {REP_4(1, 7),
     REP_4(EXP_4(7), 7),
     REP_3(EXP_4(7) * EXP_4(7), 7), 0},
     {REP_4(1, 8),
     REP_4(EXP_4(8), 8),
     REP_2(EXP_4(8) * EXP_4(8), 8), 0}};

void simpleApproach(int N)
{
    int exponent;
    ll value;

    int *pIn = &inputNumbers[0];

    for (int i = 0; i < N; i++)
    {
        value = *pIn;
        ++pIn;
        exponent = *pIn;
        ++pIn;

        if (value < 2 || exponent < 2)
        {
            if (exponent == 0)
            {
                value = 1;
            }

#if LOCAL_DEV_ENV
            ++lowValuesCount;
#endif
            numbers[i] = value;
            continue;
        }

        ll result;
        if (value < INT_POWERS_LIMIT && value * exponent <= 60)
        {
#if LOCAL_DEV_ENV
            ++fastChecks;
#endif
            result = integerPowers[value - 2][exponent - 1] % MODULO_VALUE;
        }
        else
        {
            result = (exponent & 0x1);
            result = result * value + 1 - result;
            value = (value * value) % MODULO_VALUE;
            exponent >>= 1;
            CALC_EXPONENT(result, exponent, value);
        }
        numbers[i] = (int)result;
    }
}

typedef struct
{
    int num;
    int exponent;
    int index;
} TestedValue;

TestedValue nums[MAX_N];

#define SORTED_LIMIT 1000

int cmp(const void *pA, const void *pB)
{
    long long int diff = ((TestedValue *)pA)->num - ((TestedValue *)pB)->num;
    if (diff > 0)
    {
        return 1;
    }
    if (diff < 0)
    {
        return -1;
    }
    diff = ((TestedValue *)pA)->exponent - ((TestedValue *)pB)->exponent;
    if (diff > 0)
    {
        return 1;
    }
    if (diff < 0)
    {
        return -1;
    }
    if (((TestedValue *)pA)->index < ((TestedValue *)pB)->index)
    {
        return -1;
    }
    return 1;
}

void sortedApproach(int N)
{
    int exponent;
    ll value;

    int *pIn = &inputNumbers[0];
    if (N <= SORTED_LIMIT)
    {
        simpleApproach(N);
        return;
    }

#ifdef NUM_TESTS
    TestedValue *pGlobalNums = (testIdx == 0) ? nums : (TestedValue *)malloc(N * sizeof(TestedValue));
#else
    TestedValue *pGlobalNums = nums;
#endif
    TestedValue *pNums = pGlobalNums;

    int N2 = 0;

    for (int i = 0; i < N; i++)
    {
        value = *pIn;
        ++pIn;
        exponent = *pIn;
        ++pIn;

        if (value < 2 || exponent < 2)
        {
            if (exponent == 0)
            {
                value = 1;
            }

#if LOCAL_DEV_ENV
            ++lowValuesCount;
#endif
            numbers[i] = value;
            continue;
        }

        if (value < INT_POWERS_LIMIT && value * exponent <= 60)
        {
#if LOCAL_DEV_ENV
            ++fastChecks;
#endif
            numbers[i] = integerPowers[value - 2][exponent - 1] % MODULO_VALUE;
            continue;
        }
        pNums->num = value;
        pNums->exponent = exponent;
        pNums->index = i;
        ++pNums;
        ++N2;
        numbers[i] = -1;
    }

    int duplicates = 0, prevNum = -1;
    pNums = pGlobalNums;
    if (N2 >= 100)
    {
        qsort(pGlobalNums, N2, sizeof(TestedValue), cmp);

        for (int i = 0; i < N2; i++)
        {
            int num = pNums->num;
            ++pNums;
            if (num == prevNum)
            {
                ++duplicates;
            }
            else
            {
                prevNum = num;
            }
        }
    }

#if LOCAL_DEV_ENV
    logError("#dups: %d\n", duplicates);
#endif
    // By subtracting exponents we can maybe reduce the number of multiplications
    if (duplicates > 10)
    {
        prevNum = -1;
        int lastResult = -1, lastExponent = 0;
        pNums = pGlobalNums;
        for (int i = 0; i < N2; i++)
        {
            int num = pNums->num;

            if (num == prevNum)
            {
                ll result, value;
                int exponent;

                if (lastResult < 0)
                {
                    --pNums;
                    value = num;
                    exponent = pNums->exponent;
                    lastExponent = exponent;
                    result = (exponent & 0x1);
                    result = result * value + 1 - result;
                    value = (value * value) % MODULO_VALUE;
                    exponent >>= 1;
                    CALC_EXPONENT(result, exponent, value);
                    lastResult = (int)result;
                    numbers[pNums->index] = lastResult;
                    ++pNums;
                }
                exponent = pNums->exponent;

                int diff = exponent - lastExponent;
                if (diff > 0)
                {
                    value = num;
                    result = lastResult;

                    result = (diff & 0x1);
                    result = result * value + 1 - result;
                    value = (value * value) % MODULO_VALUE;
                    diff >>= 1;
                    CALC_EXPONENT(result, diff, value);
                    lastResult = (int)((result * lastResult) % MODULO_VALUE);
                    lastExponent = exponent;
                }
                numbers[pNums->index] = lastResult;
                --duplicates;
                if (duplicates == 0)
                {
                    break;
                }
            }
            else
            {
                prevNum = num;
                lastResult = -1;
            }

            ++pNums;
        }
    }

    pIn = &inputNumbers[0];

    for (int i = 0; i < N; i++)
    {
        value = *pIn;
        ++pIn;
        exponent = *pIn;
        ++pIn;

        if (numbers[i] >= 0)
        {
            continue;
        }

        ll result;

        result = (exponent & 0x1);
        result = result * value + 1 - result;
        value = (value * value) % MODULO_VALUE;
        exponent >>= 1;

        CALC_EXPONENT(result, exponent, value)

        numbers[i] = (int)result;
    }
#ifdef NUM_TESTS
    if (testIdx > 0)
    {
        free(pGlobalNums);
    }
#endif
}

int main()
{
    int N, inValue, inExponent;
    int *pIn = &inputNumbers[0];
    int numSmallValues = 0;
    START_INPUT_MEASURE
    if (1 != scanf("%d", &N))
    {
        return 1;
    }

    for (int i = 0; i < N; i++)
    {
        if (2 != scanf("%d %d", &inValue, &inExponent))
        {
            return 1;
        }
        *pIn = inValue;
        ++pIn;
        *pIn = inExponent;
        ++pIn;
        if (inValue < 6)
        {
            ++numSmallValues;
        }
    }
    END_INPUT_MEASURE

    START_EXEC_MEASURE

#if 0

#ifdef NUM_TESTS
    for (int t = 0; t < NUM_TESTS; t++)
    {
        simpleApproach(N);
    }
#else
        simpleApproach(N);
#endif

#else

#ifdef NUM_TESTS
    testIdx = 0;
    for (; testIdx < NUM_TESTS; testIdx++)
    {
        sortedApproach(N);
    }
#else
    sortedApproach(N);
#endif

#endif

    END_EXEC_MEASURE

    START_OUT_MEASURE
#if 0
    for (int i = 0; i < N; i++)
    {
        printf("%d\n", numbers[i]);
    }
#else
    printOutput(N, outputBuffer);
#endif
    END_OUT_MEASURE

#if LOCAL_DEV_ENV
    double inTime = getTime(&inputStart, &inputEnd);
    double execTime = getTime(&execStart, &execEnd);
    double outTime = getTime(&outputStart, &outputEnd);
    logError("IN: %f | EXEC: %f | OUT: %f\n", inTime, execTime, outTime);
    logError("#calcs: %d | #low-values: %d | #fast-checks: %d\n", calcs, lowValuesCount, fastChecks);
    logError("#aligned-prints: %d\n", alignedPrints);
#endif

    return 0;
}

/*
    // Whar if we have duplicate exponents?
    // If any 2 numbers have the same exponent
    // We know that (a*b)^exp mod n = ((a^exp mod n)*(b^exp mod n)) mod n
    // We know that modulo value is a prime.
    // I.e. we can compute for any value a modular inverse value b^-1
    // We can skip calculations in the following way.
    //- choose first value 'A' which shares the same exponent with other values
    //- compute B = A^exp
    //- compute modular inverse B^-1 = (B^(M-2)) mod M
    //- for every other value D_i ... D_k with same exponent
    //- compute ((((D_i * A)^exp) mod M) * B^-1) mod M
    // This only makes sense (computationally)
    // if there are at least 3 values with same exponent.
    for (int i = 0; i < N2; i++)
    {
        int num = pExponents->numOrExponent;

        if (num == prevExponent)
        {
            int ed = 0;
            int j = i;
            --pExponents;
            int index =pExponents->index;
            ll value = inputNumbers[index * 2];

            for (; j < N2; j++)
            {
                num = pExponents->numOrExponent;
                if (num != prevExponent)
                {
                    break;
                }
                ++ed;
                ++pExponents;
            }
            if (ed > 2)
            {
                exponent = prevExponent;
                ll result;
                ll firstValue = value;

                result = (exponent & 0x1);
                result = result * value + 1 - result;
                value = (value * value) % MODULO_VALUE;
                exponent >>= 1;

                CALC_EXPONENT(result, exponent, value)

                exponent = MODULO_VALUE - 2;
                numbers[i-1]=value;

                //value = result;
                ll inverse = 1;
                CALC_EXPONENT(inverse, exponent, result)
                int l = j;
                j = i;
                for(;j <l; j++) {

                }
                i = j;

            }
            i = j;
        } else {
            ++pExponents;
        }

        prevExponent = num;
    }
    */