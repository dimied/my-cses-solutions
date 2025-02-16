#ifndef LOCAL_DEV_ENV
#define LOCAL_DEV_ENV 0
#endif

#if LOCAL_DEV_ENV == 0
#pragma GCC optimize("Ofast")
#endif

#include <stdio.h>
#include <stdlib.h>

#if LOCAL_DEV_ENV
#include <sys/time.h>
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

#define MAX_N 100000
#define MODULO_VALUE (1000 * 1000 * 1000 + 7)
// Because MODULO_VALUE is a prime
// Any exponent as multiple of (MODULO_VALUE-1) would lead to 1 as final result
// I.e. we apply (MODULO_VALUE-1) to computed exponents
#define EXP_MODULO_VALUE (1000 * 1000 * 1000 + 6)

typedef long long int ll;
typedef unsigned long long int ull;

int inputNumbers[MAX_N * 3];
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
    // little endian?
    if (1 == 0 && *pEndian == 0x04030201)
    {
        long long int *pRes, res;
        for (int i = 0; i < N; i++)
        {
            int num = numbers[i];
            if (num >= 10)
            {
                if (num >= 100)
                {
                    // Aligned?
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

#define CALC_EXPONENT(RES, EXP, VAL, MOD_VAL) \
    while (EXP > 1)                           \
    {                                         \
        switch (EXP & 0x7)                    \
        {                                     \
        case 0:                               \
            VAL = (VAL * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            break;                            \
        case 1:                               \
            RES = (RES * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            break;                            \
        case 2:                               \
            VAL = (VAL * VAL) % MOD_VAL;      \
            RES = (RES * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            break;                            \
        case 3:                               \
            RES = (RES * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            RES = (RES * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            break;                            \
        case 4:                               \
            VAL = (VAL * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            RES = (RES * VAL) % MOD_VAL;      \
            break;                            \
        case 5:                               \
            RES = (RES * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            RES = (RES * VAL) % MOD_VAL;      \
            break;                            \
        case 6:                               \
            VAL = (VAL * VAL) % MOD_VAL;      \
            RES = (RES * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            RES = (RES * VAL) % MOD_VAL;      \
            break;                            \
        case 7:                               \
            RES = (RES * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            RES = (RES * VAL) % MOD_VAL;      \
            VAL = (VAL * VAL) % MOD_VAL;      \
            RES = (RES * VAL) % MOD_VAL;      \
            break;                            \
        }                                     \
        VAL = (VAL * VAL) % MOD_VAL;          \
        ADD_CALCS                             \
        EXP >>= 3;                            \
    }                                         \
    if (EXP == 1)                             \
    {                                         \
        ADD_CALCS                             \
        RES = (RES * VAL) % MOD_VAL;          \
    }

void simpleApproach(int N)
{
    int *pIn = &inputNumbers[0];

    for (int i = 0; i < N; i++)
    {
        ll value = *pIn;
        ++pIn;
        ll exponent = *pIn;
        ++pIn;
        int exponent2 = *pIn;
        ++pIn;

        if (value < 2 || exponent < 2)
        {
            if (exponent2 == 0)
            {
                exponent = 1;
            }

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

        ll expResult = (exponent2 & 0x1);
        expResult = expResult * exponent + 1 - expResult;
        exponent = (exponent * exponent) % EXP_MODULO_VALUE;
        exponent2 >>= 1;
        CALC_EXPONENT(expResult, exponent2, exponent, EXP_MODULO_VALUE);

        ll result = (expResult & 0x1);
        result = result * value + 1 - result;
        value = (value * value) % MODULO_VALUE;
        expResult >>= 1;
        CALC_EXPONENT(result, expResult, value, MODULO_VALUE);

        numbers[i] = (int)result;
    }
}

typedef struct
{
    int num;
    int exponent;
    int exponent2;
    int index;
} TestedValue;

TestedValue nums[MAX_N];

// #define SORTED_LIMIT 1000
#define SORTED_LIMIT 50

/// @brief
/// @param pA
/// @param pB
/// @return
int cmp(const void *pA, const void *pB)
{
    ll diff = ((TestedValue *)pA)->num - ((TestedValue *)pB)->num;
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
    diff = ((TestedValue *)pA)->exponent2 - ((TestedValue *)pB)->exponent2;
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

/// @brief
/// @param pA
/// @param pB
/// @return
int cmpExponents(const void *pA, const void *pB)
{
    ll diff = ((TestedValue *)pA)->exponent - ((TestedValue *)pB)->exponent;
    if (diff > 0)
    {
        return 1;
    }
    if (diff < 0)
    {
        return -1;
    }
    diff = ((TestedValue *)pA)->exponent2 - ((TestedValue *)pB)->exponent2;
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
    int *pIn = &inputNumbers[0];

    for (int i = 0; i < N; i++)
    {
        int value = *pIn;
        ++pIn;
        int exponent = *pIn;
        ++pIn;
        int exponent2 = *pIn;
        ++pIn;

        if (value < 2 || exponent < 2)
        {
            if (exponent2 == 0)
            {
                exponent = 1;
            }

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

        pNums->num = value;
        pNums->exponent = exponent;
        pNums->exponent2 = exponent2;
        pNums->index = i;
        ++pNums;
        ++N2;
        numbers[i] = -1;
    }

    int exponentDuplicates = 0, prevExponent = -1;
    pNums = pGlobalNums;
    if (N2 >= 100)
    {
        qsort(pGlobalNums, N2, sizeof(TestedValue), cmpExponents);

        for (int i = 0; i < N2; i++)
        {
            int exponent = pNums->exponent;
            ++pNums;
            if (exponent == prevExponent)
            {
                ++exponentDuplicates;
            }
            else
            {
                prevExponent = exponent;
            }
        }
    }

#if LOCAL_DEV_ENV
    logError("#exp-dups: %d\n", exponentDuplicates);
#endif
    // By subtracting exponents we can maybe reduce the number of multiplications
    if (exponentDuplicates > 10)
    {
        int prevExponent = -1;
        int lastResult = -1, lastExponent = 0;
        pNums = pGlobalNums;
        for (int i = 0; i < N2; i++)
        {
            int num = pNums->exponent;

            if (num == prevExponent)
            {
                ll result, value;
                int exponent;

                if (lastResult < 0)
                {
                    --pNums;
                    value = num;
                    exponent = pNums->exponent2;
                    lastExponent = exponent;
                    result = (exponent & 0x1);
                    result = result * value + 1 - result;
                    value = (value * value) % EXP_MODULO_VALUE;
                    exponent >>= 1;
                    CALC_EXPONENT(result, exponent, value, EXP_MODULO_VALUE);
                    lastResult = (int)result;
                    pNums->exponent = lastResult;
                    pNums->exponent2 = -1;
                    ++pNums;
                }
                exponent = pNums->exponent2;

                int diff = exponent - lastExponent;
                if (diff > 0)
                {
                    value = num;
                    result = lastResult;

                    result = (diff & 0x1);
                    result = result * value + 1 - result;
                    value = (value * value) % EXP_MODULO_VALUE;
                    diff >>= 1;
                    CALC_EXPONENT(result, diff, value, EXP_MODULO_VALUE);
                    lastResult = (int)((result * lastResult) % EXP_MODULO_VALUE);
                    lastExponent = exponent;
                }
                pNums->exponent = lastResult;
                pNums->exponent2 = -1;
                --exponentDuplicates;

                if (exponentDuplicates == 0)
                {
                    break;
                }
            }
            else
            {
                prevExponent = num;
                lastResult = -1;
            }

            ++pNums;
        }
    }

    // Now sort by value
        qsort(pGlobalNums, N2, sizeof(TestedValue), cmp);


    pNums = pGlobalNums;
    ll lastResult = -1, lastExponent=-1, lastExponent2=-1, lastValue=-1;

    for (int i = 0; i < N2; i++)
    {
        ll value = pNums->num;
        ll exponent = pNums->exponent;
        int exponent2 = pNums->exponent2;
        int index = pNums->index;
        ++pNums;

        if (numbers[index] >= 0)
        {
            continue;
        }
        if(lastValue == value && lastExponent == exponent && lastExponent2 == exponent2) {
            numbers[index] = (int)lastResult;
            continue;
        }
        lastExponent=exponent, lastExponent=exponent2, lastValue=value;
        //TODO: same value calc

        ll expResult;
        if (exponent2 >= 0)
        {
            expResult = (exponent2 & 0x1);
            expResult = expResult * exponent + 1 - expResult;
            exponent = (exponent * exponent) % EXP_MODULO_VALUE;
            exponent2 >>= 1;
            CALC_EXPONENT(expResult, exponent2, exponent, EXP_MODULO_VALUE);
        }
        else
        {
            expResult = exponent;
        }

        ll result = (expResult & 0x1);
        result = result * value + 1 - result;
        value = (value * value) % MODULO_VALUE;
        expResult >>= 1;
        CALC_EXPONENT(result, expResult, value, MODULO_VALUE);

        numbers[index] = (int)result;
        lastResult = result;
    }
#ifdef NUM_TESTS
    if (testIdx > 0)
    {
        free(pGlobalNums);
    }
#endif
}

#if 1
int main()
{
    int N, inValue, inExponent, inExponent2;
    int *pIn = &inputNumbers[0];
    int numSmallValues = 0;
    START_INPUT_MEASURE
    if (1 != scanf("%d", &N))
    {
        return 1;
    }

    for (int i = 0; i < N; i++)
    {
        if (3 != scanf("%d %d %d", &inValue, &inExponent, &inExponent2))
        {
            return 1;
        }
        *pIn = inValue;
        ++pIn;
        *pIn = inExponent;
        ++pIn;
        *pIn = inExponent2;
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
#else
// generate numbers
int main()
{

    for (int i = 2; i <= EXP1_PRECALC_LIMIT; i++)
    {
        ll value = 1;
        printf("/*%d^{1, ... , %d} MOD EXP_MODULO_VALUE */\n", i, EXP2_PRECALC_LIMIT);
        for (int j = 1; j <= EXP2_PRECALC_LIMIT; j++)
        {
            value = (value * i) % EXP_MODULO_VALUE;
            printf("%lld, ", value);
        }
        printf("\n");
    }
    return 0;
}
#endif
