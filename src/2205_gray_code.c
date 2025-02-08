#include <stdio.h>

#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__))

#define MAX_BITS 16

const char *codes[3] = {
    "0\n1\n",
    "00\n01\n11\n10\n",
    "000\n001\n011\n010\n110\n100\n101\n111\n",
};

int main()
{
    int i, N;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    scanf("%d", &N);
#pragma GCC diagnostic pop
    if (N < 4)
    {
        printf("%s", codes[N - 1]);
        return 0;
    }
    char toPrint[MAX_BITS + 3];
    for (i = 0; i < MAX_BITS + 3; i++)
    {
        toPrint[i] = '0';
    }

    toPrint[N + 1] = '\n';
    toPrint[N + 2] = 0;

    const char *pString = toPrint + 1;
    printf("%s", pString); // Zero: 00..00

    const int limit = (1 << N);

    // for more than 32 we could use long long or an array storing bits
    int bits = 0;

    for (i = 1; i < limit; i++)
    {
        int value = i;

#if GCC_COMPILER
        int leastSignificantBitIndex = __builtin_ffs(value);
        bits ^= (1 << (leastSignificantBitIndex - 1));
#else
        int leastSignificantBitIndex = 0;
        // can also find the bit with built-ins
        while (value > 0 && value % 2 == 0)
        {
            value >>= 1;
            ++leastSignificantBitIndex;
        }

        bits ^= (1 << leastSignificantBitIndex);
#endif

        value = bits;
        // create binary string
        for (int stringIdx = N; stringIdx > 0; stringIdx--)
        {
            toPrint[stringIdx] = '0' + (value % 2);
            value >>= 1;
        }
        printf("%s", pString);
    }

    return 0;
}